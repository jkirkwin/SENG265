#!//usr/bin/env python3

# Jamie Kirkwin
# Seng 265 Assignment 2
# V00875987
# kwic2.c
# Feb 24, 2018
# Intro to Python, indexing a text file by keyword with basic output formatting

import fileinput
import sys

# formatting pad values
BEGINNING_COL = 10
CENTRE_COL = 30
ENDING_COL = 60 


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
def print_output(output_lines):
    
    for entry in output_lines:
        line = entry[0]
        ind = entry[1]

        li = line.split()
        index_word = li[ind]

        pre_buffer = ""
        max_pre_len = (CENTRE_COL) - BEGINNING_COL 
        post_buffer = ""
        max_post_len = ENDING_COL - (CENTRE_COL + len(index_word) -1)

        # create pre_buffer
        pre_words = li[:ind]
        for w in pre_words[::-1]:
            if len(w) + 1 + len(pre_buffer) > max_pre_len:
                break
            pre_buffer = w + " " + pre_buffer                

        # pad to centre
        pre_buffer = " "*(CENTRE_COL-1 - len(pre_buffer)) + pre_buffer

        #create post_buffer
        post_words = li[ind+1:]
        for w in post_words:
            if len(post_buffer) + 1 + len(w) > max_post_len:
                break
            post_buffer = post_buffer + " " + w

        print(pre_buffer + index_word + post_buffer)

    return


# Fills the output_lines list with the indexed compliment of input_lines,
# paired with the list-index of the index_word used 
def index(index_words, excluded, input_lines, input_lowercase, 
        output_lines):
    
    for word in index_words:
        for i in range(len(input_lines)):

            raw_line = input_lines[i]
            lower_line = input_lowercase[i]
            
            low_li = lower_line.split()
            raw_li = raw_line.split()

            if word in low_li:
                # record index of indexint word
                index = low_li.index(word)
                
                #capitalize indexing word
                raw_li[index] = raw_li[index].upper()
                
                # package the indexed line with the value of the index
                # which gives the index word when the string is split 
                s = " ".join(raw_li)
                output_lines.append((s, index))

    return


# Records input line-by-line in input_lines and a copy of that input in 
# lowercase is stored in input_lowercase
#
# Fills and the list of excluded words
#
# Fills and sorts (lexicographically) the list of index words
def get_input(index_words, excluded, input_lines, input_lowercase):
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
            input_lowercase.append(line.lower())

            # check each word against excluded words and previously added index words
            words = line.split()
            for w in words:
                if w.lower() not in (excluded + index_words):
                    index_words.append(w.lower())  

        else:
            # still getting excluded words as input
            excluded.append(line.lower())

        index_words.sort()

    return


def main():
    index_words = list()
    excluded = list()
    input_lines = list()
    input_lowercase = list()
    output_lines = list()

    get_input(index_words, excluded, input_lines, input_lowercase)
    index(index_words, excluded, input_lines, input_lowercase,output_lines)
    print_output(output_lines)

if __name__ == '__main__':
    main()
