#!/usr/bin/env python3

import os
import platform
import argparse
import json
import re
from termcolor import colored

def replace_macro(raw_paths):
    paths = {}
    tokens = {
        'WORKSPACE': os.path.dirname(os.path.realpath(__file__))
    }
    
    for k1,v1 in raw_paths.items():
        for k2,v2 in tokens.items():
            p = '\\$\\{' + k2 + '\\}'
            paths[k1] = re.sub(string=v1, pattern=p, repl=v2.replace('\\', '/'))

    return paths

def main():
    # Handle command line arguments
    parser = argparse.ArgumentParser(description='Build Script for Verbatim')
    parser.add_argument('actions', type=str, nargs='+', default='build-test',
                        help='which actions to perform')
    parser.add_argument('--cfg', metavar='str', type=str, nargs='?', default='default',
                        help='which configure command to execute, specified in compile settings.json')
    parser.add_argument('--build', metavar='str', type=str, nargs='?', default='default',
                        help='which build command to execute, specified in compile settings.json')
    parser.add_argument('--test', metavar='str', type=str, nargs='?', default='default',
                        help='cmake test command to execute, specified in compile settings.json')
    parser.add_argument('--target', metavar='str', type=str, nargs='?', default='ALL_BUILD',
                        help='cmake build target, default="ALL_BUILD"')
    parser.add_argument('--settings', metavar='str', type=str, nargs='?', default='./settings.json',
                        help='location of project configuration file. default="./settings.json"')
    args = parser.parse_args()

    # Paths
    cwd = os.path.dirname(os.path.realpath(__file__))

    # Handle environment
    system = 'unknown'
    if (platform.system() == 'Darwin'):
        system = 'macosx'
    elif (platform.system() == 'Windows'):
        system = 'win32'
    elif (platform.system() == 'Linux'):
        system = 'linux'

    settings = json.load(open(args.settings, 'r'))
    paths = replace_macro(settings['global'])
    cmake_cfg = settings['cmake'][system]['configure']
    cmake_build = settings['cmake'][system]['build']
    cmake_test = settings['cmake'][system]['test']

    cfg_success = False
    build_success = False

    cfg_entries = [x for x in cmake_cfg if x['name'] == args.cfg]
    if (len(cfg_entries) > 1):
        raise f'duplicate configure in {args.settings} items with name {args.cfg}'

    elif (len(cfg_entries) == 1 and 'cfg' in args.actions):
        cmd = 'cmake'
        cfg = cfg_entries[0]

        flags = cfg['flags']
        for f in flags:
            cmd += f' {f}'

        defs = cfg['definitions']
        for d in defs:
            n = d['name']
            t = d['type']
            v = d['value']
            cmd += f' -D{n}:{t}={v}'

        toolset = cfg['toolset']
        for t in toolset:
            n = t['name']
            v = t['value']
            cmd += f' -T {n}={v}'

        gen = cfg['generator']
        if (gen != 'default'): cmd += f' -G {gen}'

        cmd += f' -A {cfg["arch"]}'
        cmd += f' -B {paths["build_path"]}'
        cmd += f' -S {cwd}'

        print(colored(f'executing expression: {cmd}', 'yellow'))
        cfg_success = bool(os.system(cmd))

    build_entries = [x for x in cmake_build if x['name'] == args.build]
    if (len(build_entries) > 1):
        raise f'duplicate build in {args.settings} items with name {args.build}'
    elif(len(build_entries) == 1 and 'build' in args.actions):
        cmd = 'cmake'
        build = build_entries[0]

        # Concat build path
        build_path = paths['build_path']
        cmd += f' --build {build_path}'

        # Concat build config
        build_cfg = build['type']
        cmd += f' --config {build_cfg}'

        # Concat build target
        target = build['target']
        cmd += f' --target {target}'

        # Concat flags
        flags = build['flags']
        for f in flags:
            cmd += f' {f["name"]} {f["value"]}'

        # Concat bikeshedder
        cmd += " --"

        print(colored(f'executing expression: {cmd}', 'yellow'))
        os.system(cmd)
        build_success = bool(os.system(cmd))

    test_entries = [x for x in cmake_test if x['name'] == args.test]
    if (len(test_entries) > 1):
        raise f'duplicate test in {args.settings} items with name {args.test}'
    elif (len(test_entries) == 1 and 'test' in args.actions):
        cmd = "ctest"
        test = test_entries[0]
        cmd += f' -C {test["cfg"]}'
        cmd += f' -T {test["action"]}'

        for f in test['flags']:
            if (not 'value' in f.keys()):
                cmd += f' {f["name"]}'
            else:
                cmd += f' {f["name"]} {f["value"]}'

        cmd += f' --test-dir {paths["test_path"]}'

        print(colored(f'executing expression: {cmd}', 'yellow'))
        os.system(cmd)

if __name__ == '__main__':
    main()
