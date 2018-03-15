#!/usr/bin/env python3

'''
Jamie Kirkwin
Seng 265 Assignment 3
classykwic.py
March 13, 2018

Refactor kwic2 using regular expressions and a Kwic class

The program using this class is kwic3.py
'''

import sys

class Kwic:

    # formatting pad values (class variables)
    start = 10
    centre = 30
    end = 60

    def get_index_words(self):
        '''Creates a list of all non-excluded words'''
        
        index_words = []
        for line in self.raw_lines:
            for word in line.split():
                if word not in (self.excluded + index_words):
                    index_words.append(word) 

        return index_words


    def __init__(self, excluded, lines):
        self.excluded = excluded
        self.raw_lines = lines
        self.index_words = self.get_index_words()


    def output(self):
        '''returns fully indexed and formatted list of lines'''
        
        # create the list of lines to be formatted
        lines = []
        for ind_word in self.index_words:
            for line in self.raw_lines:
                li = line.split()
                if ind_word in li:
                    index = li.index(ind_word)
                    li[index] = li[index].upper()

                    lines.append( (" ".join(li), index) )

        # use lines[] to create a fully formatted list
        output = []

        for entry in lines:
            line = entry[0]
            ind = entry[1]

            li = line.split()
            index_word = li[ind]

            pre_buffer = ""
            max_pre_len = Kwic.centre - Kwic.start 
            post_buffer = ""
            max_post_len = Kwic.end - (Kwic.centre + len(index_word) -1)

            # create pre_buffer
            pre_words = li[:ind]
            for w in pre_words[::-1]:
                if len(w) + 1 + len(pre_buffer) > max_pre_len:
                    break
                pre_buffer = w + " " + pre_buffer                

            # pad to centre
            pre_buffer = " "*(Kwic.centre-1 - len(pre_buffer)) + pre_buffer

            #create post_buffer
            post_words = li[ind+1:]
            for w in post_words:
                if len(post_buffer) + 1 + len(w) > max_post_len:
                    break
                post_buffer = post_buffer + " " + w

            output.append(pre_buffer+index_word+post_buffer)

        return output