#!/usr/bin/env python3

'''
Jamie Kirkwin
Seng 265 Assignment 3
classykwic.py
March 15, 2018

Refactor kwic2 using regular expressions and a Kwic class

The program using this class is kwic3.py
'''

import sys
import re

class Kwic:

    # formatting pad values (class variables)
    start = 10
    centre = 30
    end = 60


    def _get_index_words(self):
        '''Creates and returns a list of all non-excluded words'''

        index_words = []
        for line in self.raw_lines:
            
            for word in line.split():
                word = word.lower()
                if word not in (self.excluded + index_words):
                    index_words.append(word) 

        index_words.sort()
        return index_words


    def __init__(self, excluded, lines):
        self.excluded = excluded
        self.raw_lines = lines
        self.index_words = self._get_index_words()


    def output(self):
        output = []

        for word in self.index_words:

            # Matches any line containing the current index word
            pattobj = re.compile(r'^.*\b'+word+r'\b.*$', re.IGNORECASE)

            for line in self.raw_lines:
                matchobj = pattobj.search(line)

                if(matchobj):
                    # capitalize the index word occurances
                    output_line = re.sub(word, word.upper(), line)

                    output.append(self._format(output_line, word))

        return output


    def _format(self, output_line, index_word):
        ''' returns a formatted version of the line passed in'''
        pre_max = self.centre - self.start
        post_max = self.end - self.centre - len(index_word)

        pattern = r'(?<=\b) *(.{0,' +str(pre_max)+ '})' +index_word+ r'(.{0,' +str(post_max)+ r'})(?=\b)'

        matchobj = re.search(pattern, output_line, re.IGNORECASE)

        f_line = matchobj.group(0).strip()

        pad = self.centre - 1 - len(matchobj.group(1))
        print("pad = {}".format(pad))

        return pad * " " + f_line