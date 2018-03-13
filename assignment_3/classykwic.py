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
import classykwic

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
        '''returns the fully indexed list of lines'''
        return []