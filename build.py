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
            paths[k1] = os.path.abspath(re.sub(pattern=p, repl=v2, string=v1))

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
    parser.add_argument('--settings', metavar='s', type=str, nargs='?', default='"./settings.json:"',
                        help='location of project configuration file. default=./settings.json')
    args = parser.parse_args()

    # Paths
    cwd = os.path.dirname(os.path.realpath(__file__))

    # Handle environment
    system = 'unknown'
    if (platform.system() == 'Darwin'):
        system = 'macosx'
    elif (platform.system() == 'Win32'):
        system = 'win32'
    elif (platform.system() == 'Linux'):
        system = 'linux'

    settings = json.load(open('settings.json', 'r'))
    cmake_cfg = settings['cmake'][system]['configure']
    paths = replace_macro(settings['global'])

    if ('configure' in args.action):
        cmd = 'cmake'

        flags = cmake_cfg['flags']
        for f in flags:
            cmd += f' {f}'

        defs = cmake_cfg['definitions']
        for d in defs:
            n = d['name']
            t = d['type']
            v = d['value']
            cmd += f' -D{n}:{t}={v}'

        defs = cmake_cfg['toolset']
        for d in defs:
            n = d['name']
            v = d['value']
            cmd += f' -T {n}={v}'

        gen = cmake_cfg['generator']
        if (gen != 'default'): cmd += f' -G {gen}'

        cmd += f' {cmake_cfg["arch"]}'
        cmd += f' -B {paths["build_path"]}'
        cmd += f' -S {cwd}'

        print(cmd)
        os.system(cmd)

if __name__ == '__main__':
    main()
