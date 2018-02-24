# Add shebang path

# Jamie Kirkwin
# Seng 265 Assignment 2
# V00875987
# kwic2.c
# Feb 23, 2018
# Intro to Python, indexing a text file by keyword with basic formatting

import fileinput
import sys

index_words = list()
excluded = list()
input_lines = list()
output_lines = list()

# for debugging
# stackoverflow.com/questions/12214801/print-a-string-as-hex-bytes
def print_hex(l):
    for s in l:
        print(":".join("{:02x}".format(ord(c)) for c in s))

# for debugging
def print_lines(l):
    for s in l:
        print(s)


# for debugging
# should output the exact input file
def print_input():
    print(2)
    print('::')
    print_lines(excluded)
    print('::')
    print_lines(input_lines)


# Prints the fully indexed list of words with the formatting specified in 
# a2_writeup.pdf
def print_output():
    return


# Fills the output_lines list with the indexed compliment of input_lines
# according to the content of the lists of excluded and index words 
def index():
    return


# Records input line-by-line in a input_lines
# Fills and the list of excluded words
# Fills and sorts (lexicographically) the list of index words
def get_input():
    version = input()
    seperator = input()

    flag = False
    for line in fileinput.input():
        line = line.strip();
        
        if line == seperator: 
            flag = True
            continue
        
        elif line == '':
            continue
        
        elif flag:
            # non-empty input line
            input_lines.append(line)

            # check each word against excluded words and previously added index words
            words = line.split()
            for w in words:
                if w not in (excluded + index_words):
                    index_words.append(w)  

        else:
            # still getting excluded words
            excluded.append(line)

        index_words.sort()

    return


def main():
    get_input()
    
    #print_input()
    print()
    print_lines(index_words)
    
    index()
    print_output()

if __name__ == '__main__':
    main()