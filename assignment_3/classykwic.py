#!/usr/bin/env python3

'''
Jamie Kirkwin
Seng 265 Assignment 3
classykwic.py
March 17, 2018

Refactor kwic2 using regular expressions and a Kwic class

The script using this module is kwic3.py
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
        '''returns a list of the indexed and formatted lines'''
        output = []

        for word in self.index_words:

            # Matches any line containing the current index word
            pattobj = re.compile(r'^.*\b'+word+r'\b.*$', re.IGNORECASE)

            for line in self.raw_lines:
                matchobj = pattobj.search(line)

                if(matchobj):
                    # capitalize the index word occurances 
                    output_line = re.sub(r'\b' +word+ r'\b', word.upper(), \
                            line, flags = re.IGNORECASE)
                    
                    # add the formatted line to output
                    output.append(self._format(output_line, word))
                    
        return output


    def _format(self, output_line, index_word):
        ''' returns a formatted version of the line passed in'''
        
        pre_max = self.centre - self.start
        post_max = self.end +1 - self.centre - len(index_word)

        # extract only the words which will fit in our formatting space
        pattern = r'\b *(.{0,' +str(pre_max)+ r'})\b' +index_word+ \
                r'\b(.{0,' +str(post_max)+ r'})\b'
        matchobj = re.search(pattern, output_line, re.IGNORECASE)
        f_line = matchobj.group(0).strip()

        pad = self.centre - 1 - len(matchobj.group(1))
        return pad * " " + f_line
