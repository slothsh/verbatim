#!/usr/bin/env python3

import os
import platform
import argparse
import json
import re

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
    parser.add_argument('--cfg', metavar='str', type=str, nargs='?', default='',
                        help='which configure command to execute, specified in compile settings.json')
    parser.add_argument('--build', metavar='str', type=str, nargs='?', default='',
                        help='which build command to execute, specified in compile settings.json')
    parser.add_argument('--target', metavar='str', type=str, nargs='?', default='ALL_BUILD',
                        help='cmake build target, default="ALL_BUILD"')
    parser.add_argument('--settings', metavar='s', type=str, nargs='?', default='./settings.json',
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

    cfg_entries = [x for x in cmake_cfg if x['name'] == args.cfg]
    if (len(cfg_entries) > 1):
        raise f'duplicate configure in {args.settings} items with name {args.cfg}'

    elif (len(cfg_entries) == 1):
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

        defs = cfg['toolset']
        for d in defs:
            n = d['name']
            v = d['value']
            cmd += f' -T {n}={v}'

        gen = cfg['generator']
        if (gen != 'default'): cmd += f' -G {gen}'

        cmd += f' {cfg["arch"]}'
        cmd += f' -B {paths["build_path"]}'
        cmd += f' -S {cwd}'

        print(f'executing expression: {cmd}')
        # os.system(cmd)

if __name__ == '__main__':
    main()
