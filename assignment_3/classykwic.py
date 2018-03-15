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


    def _get_index_words(self):
        '''Creates a list of all non-excluded words'''

        index_words = []
        for line in self.raw_lines:
            for word in line.split():
                word = word.lower()
                if word not in (self.excluded + index_words):
                    index_words.append(word) 

        index_words.sort()
        return index_words


    def _get_lower_lines(self):
        lower_lines = []
        for x in self.raw_lines:
            lower_lines.append(x.lower())
        return lower_lines


    def __init__(self, excluded, lines):
        self.excluded = excluded
        self.raw_lines = lines
        self.index_words = self._get_index_words()
        self.lower_lines = self._get_lower_lines()


    def _index(self):
        '''creates the list of lines to be formatted, each paired with the 
        index of the word to index by (indexing should be on the list gotten
        from lines.split()'''
        lines = []

        for word in self.index_words:
            for i in range(len(self.raw_lines)):

                raw_line = self.raw_lines[i]
                lower_line = self.lower_lines[i]
                
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
                    lines.append((s, index))
        return lines


    def output(self):
        '''returns fully indexed and formatted list of lines'''
        
        lines = self._index()

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