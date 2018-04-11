'''
Jamie Kirkwin

April 1, 2018

This will be a place to practice making and understanding regular 
expressions and their usage in python to help study for the 
SENG265 final exam
'''

import fileinput as io
import re

def main():
    input = io.input()
    for line in input:
        print('{}|'.format(strip(line)))
    

def strip(s):
    '''
    remove leading and trailing whitespace from s
    '''
    # pattern = r'(\s*)?((\b.*\b)\s*$)?'
    pattern = r'\s*(\S.*\S)?\s*'
    matchobj = re.search(pattern, s)
    return matchobj.group(1)


def pad(s, padSize = 10):
    '''
    give the string s a left-hand-size pad of whitespace 
    '''
    pattern = r'^\s*'
    return re.sub(pattern, " "*padSize, s)

if __name__ == '__main__':
    main()