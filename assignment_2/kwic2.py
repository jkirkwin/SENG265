# Add shebang path

# Jamie Kirkwin
# Seng 265 Assignment 2
# V00875987
# kwic2.c
# Feb 24, 2018
# Intro to Python, indexing a text file by keyword with basic formatting

import fileinput
import sys

index_words = list()
excluded = list()
input_lines = list()
input_lowercase = list()
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
    
    for entry in output_lines:
        line = entry[0]
        ind = entry[1]

        li = line.split()

        # the capitalized word to begin at column 30 is at li[ind]
        buffer = li[ind] + " "

        # add words following index word
        for word in li[ind+1:]:
            if len(buffer) + len(word) > 30:
                break
            buffer = buffer + word + " "

        # pad to 30 characters
        buffer = buffer + " " * (30-len(buffer))

        # add words preceding index word
        buffer = buffer + " " 
        for word in li[:ind][::-1]: # does not work for index = 0
            if len(buffer) + len(word) > 50:
                break
            buffer = word + " " + buffer

        # pad to centre at column 30
        buffer = " "*(60 -len(buffer))  + buffer

        print(buffer)

    return


# Fills the output_lines list with the indexed compliment of input_lines,
# paired with the list-index of the index_word used 
def index():
    for word in index_words:
        for i in range(len(input_lines)):

            raw_line = input_lines[i]
            lower_line = input_lowercase[i]
            
            low_li = lower_line.split()
            raw_li = raw_line.split()

            if word in low_li:
                # capitalize index word
                index = low_li.index(word)
                
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
            input_lowercase.append(line.lower())

            # check each word against excluded words and previously added index words
            words = line.split()
            for w in words:
                if w.lower() not in (excluded + index_words):
                    index_words.append(w.lower())  

        else:
            # still getting excluded words
            excluded.append(line.lower())

        index_words.sort()

    return


def main():
    get_input()
    index()
    print_output()

if __name__ == '__main__':
    main()