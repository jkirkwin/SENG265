/* Jamie Kirkwin
 * kwic4.c
 * SENG265 Assignment 4
 * March 30, 2018
 */


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "linkedlist.h"
#include <assert.h>
#include <ctype.h>
#include <regex.h>

// Formatting pad values
#define start 10
#define centre 30
#define end 60

// Prototypes
void printword(wordnode_t*, void*);
void get_input(wordnode_t **excluded, wordnode_t **lines, 
	wordnode_t **index_words);
wordnode_t * get_output_lines(wordnode_t *lines, wordnode_t *index_words);
void print_output(wordnode_t * output_lines);
void chomp(char *);
void free_list(wordnode_t *);
void str_toupper(char *, int);
char * int_to_str(int);
char * trim(int mode, char * buffer, char * index_word);


int main(int argc, char *argv[]) {

    wordnode_t *excluded = NULL;
    wordnode_t *lines = NULL;
    wordnode_t *index_words = NULL;

    get_input(&excluded, &lines, &index_words);
    wordnode_t *output_lines = get_output_lines(lines, index_words);
    print_output(output_lines);

#ifdef DEBUG
	printf("EXCLUDED:\n");
    apply(excluded, &printword, "%s | %d\n");

    printf("\nLINES:\n");
    apply(lines, &printword, "%s | %d\n");

    printf("\nINDEX WORDS:\n");
    apply(index_words, &printword, "%s | %d\n");
    
    printf("\nOUTPUT LINES:\n");
    apply(output_lines, &printword, "%s | %d\n");

    printf("\n - - - DEBUG is defined - - - \n\n");
#endif

    free_list(excluded);
    free_list(index_words);
    free_list(lines);
    free_list(output_lines);

    return 0;
}


/*
 * Stores the excluded words and the lines of input into two lined lists,
 * uses the list of exlcluded words to also create a list of words with 
 * which to index the lines.
 */
void get_input(wordnode_t **excluded_p, wordnode_t **lines_p, 
	wordnode_t **index_words_p)
{
	int copied;
	unsigned long int read;

	char * token, * upper;
	char * tmp = NULL;
	char * seperator = NULL;

	// Read version number and initial seperator
	copied = getline(&tmp, &read, stdin);
	copied = getline(&seperator, &read, stdin);
	chomp(seperator);

	// mode=0 -> read excluded words
	// mode=1 -> read input lines
	int mode = 0;
	copied = getline(&tmp, &read, stdin);
	while(copied != -1)
	{
		chomp(tmp);
		if(strcmp(tmp, seperator) == 0)
		{
			assert(mode == 0);
			mode = 1;
		} else if(copied == 1) 
		{
			// blank line
			copied = getline(&tmp, &read, stdin);
			continue;
		} else if(mode == 0)
		{
			// reading excluded word
			upper = strdup(tmp);
			if(upper == 0)
			{
				printf("alloc error\n");
				exit(1);
			}
			str_toupper(upper, strlen(upper));

			*excluded_p = add_end(*excluded_p, new_word(upper, 0));
			
			free(upper);
		} else if(mode == 1)
		{
			// reading input line
			*lines_p = add_end(*lines_p, new_word(tmp, 0));
			token = strtok(tmp, " ");
			while(token != 0)
			{
				upper = strdup(token);
				if(upper == 0)
				{
					printf("alloc error\n");
					exit(1);
				}
				str_toupper(upper, strlen(upper));

				if(in_list(*excluded_p, upper) == 0 && 
					in_list(*index_words_p, upper) == 0)
				{
				
					*index_words_p = add_sorted(*index_words_p, 
							new_word(upper, 0));	
				}
				free(upper);
				token = strtok(NULL, " ");
			}
		} else
		{
			// should not be here
			assert(0);
		}
		
		copied = getline(&tmp, &read, stdin);
	}

	free(tmp); 
	free(seperator);
}


/*
 * Uses the index words to create a list of lines to be output, with the
 * index of the word to be centred stored in the val field of each struct.
 */
wordnode_t * get_output_lines(wordnode_t *lines, wordnode_t *index_words)
{
	wordnode_t * output_lines = NULL;

	wordnode_t * word_p, * line_p;
	for(word_p = index_words; word_p != NULL; word_p = word_p->next)
	{
		for(line_p = lines; line_p != NULL; line_p = line_p->next)
		{
			int len = 0;

			char * tmp = strdup(line_p->w);
			str_toupper(tmp, strlen(tmp));

			char * token = strtok(tmp, " ");
			while(token != 0)
			{
				if(strcmp(token, word_p->w) == 0)
				{
					// word occurs at line[len]
					output_lines = add_end(output_lines, 
						new_word(line_p->w, len));
				}
				len += strlen(token) + 1;
				token = strtok(NULL, " ");
			}
            free(tmp);
		}
	}
	return output_lines; 
}


/*
 * Returns the string representation of num
 * Notice: allocates memory for the string returned
 */
char * int_to_str(int num)
{
	int digits = 0;
	int copy = num;
	while(copy != 0)
	{
		copy = copy/10;
		digits++;
	}

	char * str = (char *) emalloc(sizeof(char) * (digits + 1) );
	memset(str, '\0', digits + 1);

	int i;
	for(i = digits-1; i >= 0; i--)
	{
		str[i] = (num%10) + '0';

		num = num / 10;
	}
	return str;
}


/* Each line of output is formatted and printed */
void print_output(wordnode_t * output_lines)
{
	wordnode_t *line_node;
	for(line_node = output_lines; line_node != NULL; 
		line_node = line_node->next)
	{
		char * w = line_node->w;
		int val = line_node->val;

		char * in_line = w + val;

		// get length of index word
		int ind_len;
		for(ind_len = 0; ind_len < strlen(in_line); ind_len++)
		{
			if(in_line[ind_len] == ' ')
			{
				break;
			}
		}

		// Extract index word
		char * index_word = (char *) emalloc( sizeof(char) * (ind_len + 1));
		memset(index_word, '\0', ind_len + 1);
		strncpy(index_word, in_line, ind_len);
		str_toupper(index_word, ind_len);
		

		// Extract Pre Buffer
		int pre_buffer_len = val;
		char *pre_buffer = (char *) emalloc(sizeof(char) * 
			(pre_buffer_len + 1));
		memset(pre_buffer, '\0', pre_buffer_len + 1);
		strncpy(pre_buffer, w, pre_buffer_len);

		// Extract Post Buffer 
		int post_buffer_len = strlen(w) - (val + strlen(index_word));
		char *post_buffer = (char *) emalloc(sizeof(char) * 
			(post_buffer_len + 1));
		memset(post_buffer, '\0', post_buffer_len + 1);
		strncpy(post_buffer, in_line + strlen(index_word), post_buffer_len);
		
		// Remove trailing whitespace
		while(post_buffer[post_buffer_len - 1] == ' ')
		{
			post_buffer[post_buffer_len - 1] = '\0';
			post_buffer_len--;
		}

		// Trim
		if(pre_buffer_len > centre - start)
		{
			char * tmp = trim(0, pre_buffer, index_word);
			free(pre_buffer);
			pre_buffer = tmp;	
		}
		if(post_buffer_len > end + 1 - centre - strlen(index_word))
		{
			char * tmp = trim(1, post_buffer, index_word);
			free(post_buffer);
			post_buffer = tmp; 
		}

		// Create Whitespace Pad Buffer 
		int pad_size = centre - strlen(pre_buffer) -1;
		char * pad = (char *) emalloc(sizeof(char) * (pad_size + 1));
		pad[pad_size] = '\0';
		memset(pad, ' ', pad_size);

		printf("%s%s%s%s\n", pad, pre_buffer, index_word, post_buffer);

		free(pad);
		free(pre_buffer);
		free(post_buffer);
		free(index_word);
	}
}


/*
 * Returns a trimmed version of the buffer passed in
 * Mode param dictates which end is trimmed
 * Notice: Allocates Memory
 */
char * trim(int mode, char * buffer, char * index_word)
{
	assert(mode == 0 || mode == 1);
	// Mode=0 -> pre_buffer trim
	// Mode=1 -> post_buffer trim

	char * trimmed, *pattern, *pre_pattern, *post_pattern, *max_len_str;
	int patt_len;
	
	int status;
	regex_t re;
	regmatch_t match[4];

	if(mode == 0)
	{
		// pattern:" .{0,MAX_LEN}$"
		pre_pattern = " (.{0,";
		post_pattern = "}$)";
		max_len_str = int_to_str(centre - start);
	}else
	{
		// pattern:"(^.{0,MAX_LEN-1}) "
		pre_pattern = "(^.{0,";
		post_pattern = "}) ";
		max_len_str = int_to_str(end + 1 - centre - strlen(index_word));
	}

	patt_len = strlen(pre_pattern) + strlen(post_pattern) + \
		strlen(max_len_str); 

	pattern = (char *) emalloc(sizeof(char) * (patt_len + 1));
	memset(pattern, '\0', patt_len+1);
	strcat(pattern, pre_pattern);
	strcat(pattern, max_len_str);
	strcat(pattern, post_pattern);

	// Compile and check for error
	if(regcomp(&re, pattern, REG_ICASE|REG_EXTENDED))
	{
		printf("Could not compile pattern\n");
		exit(1);
	}

	status = regexec(&re, buffer, 3, match, 0);
	if(status != 0)
	{
		printf("ERROR: No Match\n");
	}

	// the section specified by match[1] is the trimmed substring
	int so = match[1].rm_so;
	int eo = match[1].rm_eo;

	trimmed = (char *) emalloc(sizeof(char) * (eo - so + 1));
	memset(trimmed, '\0', eo - so + 1);
	strncpy(trimmed, buffer + so, eo - so);

	free(pattern);
	free(max_len_str);
	regfree(&re);

	return trimmed;
}


/* Capitalizes n characters of the given string */
void str_toupper(char * w, int n)
{
	int i;
	for(i = 0; i < n; i++)
	{
		*(w + i) = toupper(*(w + i));
	}	
}


/* frees each node and its contents */
void free_list(wordnode_t * list)
{
	wordnode_t * p;
	while(list != NULL)
	{
		p = list->next;
		free_word(list);
		list = p;
	}
}


/* remove trailing newline */
void chomp(char * w)
{
	int offset = strlen(w) - 1;
	if(w[offset] == '\n') w[offset] = '\0';
}


/* for debugging */
void printword(wordnode_t *n, void *arg) 
{
    char *format = (char *)arg;

    printf(format, n->w, n->val);
}