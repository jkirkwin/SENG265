#!/usr/bin/env python3

'''
Jamie Kirkwin
Seng 265 Assignment 3
classykwic.py
March 12, 2018

Refactor kwic2 using regular expressions and a Kwic class
'''

import sys
import classykwic

class Kwic:

    def __init__(self, excluded, lines):
        self.excluded = []
        self.index_lines = []


    def output(self):
        return []


def main():
''' code provided by Dr Zastre with assignment '''

    count = 0
    mode = 0   # 0 = version; 1 = exclusion; 2 = actual lines
    exclusion = 0

    excluded = []
    input_lines = []

    for line in sys.stdin:
        line = line.rstrip()
        if (mode == 0 and line != "::"):
            kwic_version = int(line)
            if (kwic_version != 2):
                print("File has version ", kwic_version,
                    ", expected 2")
                sys.exit(1)

            continue
        elif (line == "::" and mode == 0):
            mode = 1
            continue
        elif (line == "::" and mode == 1):
            mode = 2
            continue

        if (mode == 1):
            excluded.append(line.lower())
        elif (mode == 2):
            if (line != ""):
                input_lines.append(line)

    k = classykwic.Kwic(excluded, input_lines)

    for l in k.output():
        print(l)


if __name__ == "__main__":
    main()
