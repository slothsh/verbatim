#!/usr/bin/env python3

import os
import platform
import argparse
import json
import re
import shutil
from termcolor import colored


def get_cwd():
    return os.path.dirname(os.path.realpath(__file__)).replace('\\', '/')


def is_build_path(path):
    pattern = f'{get_cwd()}/build/[A-z0-9\\/\\-\\_]+$'
    return re.search(pattern, path) is not None


def replace_macro(raw_paths):
    paths = {}
    tokens = {
        'WORKSPACE': os.path.dirname(os.path.realpath(__file__))
    }
    
    for k1, v1 in raw_paths.items():
        for k2, v2 in tokens.items():
            p = '\\$\\{' + k2 + '\\}'
            paths[k1] = re.sub(string=v1,
                               pattern=p,
                               repl=v2.replace('\\', '/'))

    return paths

def main():
    # Handle command line arguments
    parser = argparse.ArgumentParser(description='Build Script for Verbatim')
    parser.add_argument('actions', type=str, nargs='+', default='build test',
                        help='which actions to perform')
    parser.add_argument('--cfg', metavar='str', type=str, nargs=1, default='default',
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
    cwd = get_cwd()

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

    cfg_entries = [x for x in cmake_cfg if x['name'] == args.cfg[0]]
    build_entries = [x for x in cmake_build if x['name'] == args.build]
    test_entries = [x for x in cmake_test if x['name'] == args.test]

    if ('clean' in args.actions):
        clean_path = paths['build_path']

        if (len(build_entries) == 1 and 'path' in build_entries[0]):
            raw_path = {'path': build_entries[0]['path']}
            clean_path = replace_macro(raw_path)['path']

        if (is_build_path(clean_path) and os.path.isdir(clean_path)):
            print(colored(f'cleaning up items in directory: {clean_path}', 'yellow'))
            try:
                shutil.rmtree(clean_path)
            except OSError as e:
                print(f'error: {i} : {e.strerror}')
        else:
            print(colored(f'invalid clean path: {clean_path}', 'red'))

    if (len(cfg_entries) > 1):
        raise f'duplicate configure in {args.settings} items with name {args.cfg}'
    elif (len(cfg_entries) == 1 and 'cfg' in args.actions):
        cmd = 'cmake'
        cfg = cfg_entries[0]
        cfg_path = paths['build_path']

        if ('path' in cfg):
            raw_path = {'path': cfg['path']}
            cfg_path = replace_macro(raw_path)['path']

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
        if (gen != 'default'):
            cmd += f' -G {gen}'

        if ('arch' in cfg):
            cmd += f' -A {cfg["arch"]}'
        cmd += f' -B {cfg_path}'
        cmd += f' -S {cwd}'

        print(colored(f'executing expression: {cmd}', 'yellow'))
        os.system(cmd)

    if (len(build_entries) > 1):
        raise f'duplicate build in {args.settings} items with name {args.build}'
    elif(len(build_entries) == 1 and 'build' in args.actions):
        cmd = 'cmake'
        build = build_entries[0]

        # Concat build path
        build_path = paths['build_path']
        if ('path' in build):
            raw_path = {'path': build['path']}
            build_path = replace_macro(raw_path)['path']

        cmd += f' --build {build_path}'

        # Concat build config
        build_cfg = build['type']
        cmd += f' --config {build_cfg}'

        # Concat build target
        if ('target' in build):
            cmd += f' --target {build["target"]}'

        # Concat flags
        flags = build['flags']
        for f in flags:
            cmd += f' {f["name"]} {f["value"]}'

        # Concat bikeshedder
        cmd += " --"

        print(colored(f'executing expression: {cmd}', 'yellow'))
        os.system(cmd)
        os.system(cmd)

    if (len(test_entries) > 1):
        raise f'duplicate test in {args.settings} items with name {args.test}'
    elif (len(test_entries) == 1 and 'test' in args.actions):
        cmd = "ctest"
        test = test_entries[0]
        test_path = f'{paths["build_path"]}/tests'

        if ('path' in test):
            raw_path = {'path': test['path']}
            clean_path = replace_macro(raw_path)['path']
            if (is_build_path(clean_path) and os.path.isdir(clean_path)):
                test_path = clean_path

        cmd += f' -C {test["cfg"]}'
        cmd += f' -T {test["action"]}'

        for f in test['flags']:
            if ('value' not in f.keys()):
                cmd += f' {f["name"]}'
            else:
                cmd += f' {f["name"]} {f["value"]}'

        cmd += f' --test-dir {test_path}'

        print(colored(f'executing expression: {cmd}', 'yellow'))
        os.system(cmd)


if __name__ == '__main__':
    main()
