#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "linkedlist.h"
#include <assert.h>
#include <regex.h>
#include <ctype.h>
// #include <sys/types.h>

#define start 10
#define centre 30
#define end 60
#define wrd_bndry "^|$|[^[:alnum:]_]"
/*
 * This will be used for testing and exploration during the
 * development of assignment 4
 */


// prototypes
void test_add_sorted();
void test_in_list();
void print_list(wordnode_t *list, void * arg);
void using_getline();
void using_regex();
void str_toupper(char * w, int n);
void word_boundary_approach();
char * format_string(char *, char *);
char * int_to_str(int);

int main()
{

	char * index_word = "WITHIN";
	char * line = "this line has a word or more contained within it";
	char * formatted = format_string(index_word, line);
	printf("formatted: \'%s\'\n", formatted);
	// using_regex();
	// word_boundary_approach();

	free(formatted);
	return 0;
}



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


char * format_string(char * index_word, char * line)
{
	// if(word is in line)
	//		extract pre and post buffers
	//		if(strlen(pre_buffer) > max_allowed)
	//			use a new regex to trim
	// 		if(strlen(post_buffer) > max allowed)
	// 			use a new regex to trim
	//		concatenate pre_buffer, index_word, post_buffer

	char * formatted;
	formatted = (char *) emalloc(sizeof(char) * end - start + 1);
	memset(formatted, '\0', end - start + 1);

	char *pre_buffer, *post_buffer;
	int max_pre_len = centre - start;
	int max_post_len = end - centre - strlen(index_word);
	int pre_len, post_len;

	int status;
	regex_t re;
	regmatch_t match[5];

	char *pattern, *pre_patt, *post_patt;
	pre_patt = "^(.*)";
	post_patt = "(.*)$";
	int size = strlen(pre_patt) + strlen(post_patt) + strlen(index_word) + 1;
	pattern = emalloc(sizeof(char) * size);
	memset(pattern, '\0', size);

	strcat(pattern, pre_patt);
	strcat(pattern, index_word);
	strcat(pattern, post_patt);

	// compile and check for error
	if(regcomp(&re, pattern, REG_ICASE|REG_EXTENDED))
	{
		// error in compilation
		printf("Could not compile pattern\n");
		exit(1);
	}

	status = regexec(&re, line, 4, match, 0);
	if(status == 0)
	{
		// match successful 

		// extract pre and post buffers
		pre_len = match[1].rm_eo - match[1].rm_so;
		post_len = match[2].rm_eo - match[2].rm_so;
		
		pre_buffer = (char *) emalloc(sizeof(char) * (pre_len + 1));
		post_buffer = (char *) emalloc(sizeof(char) * (post_len + 1));		
		
		memset(pre_buffer, '\0', pre_len + 1);
		memset(post_buffer, '\0', post_len + 1);

		strncpy(pre_buffer, line, pre_len);
		strncpy(post_buffer, line + match[2].rm_so, post_len);


		if(pre_len > max_pre_len)
		{

			// need to trim the pre buffer
			int buff_status;
			regex_t buff_re;
			regmatch_t buff_match[2];

			// build the pattern to use
			char * buff_pre_patt = " (.{0,";
			char * buff_post_patt = "})$";
			char * buff_num = int_to_str(max_pre_len);
			int buff_len = strlen(buff_num) + strlen(buff_pre_patt) + 
				strlen(buff_post_patt);

			char * buff_patt = (char *) emalloc(sizeof(char) * (buff_len + 1));
			memset(buff_patt, '\0', buff_len + 1);
			
			strcat(buff_patt, buff_pre_patt);
			strcat(buff_patt, buff_num);
			strcat(buff_patt, buff_post_patt);
			
			// compile and check for error
			if(regcomp(&buff_re, buff_patt, REG_ICASE|REG_EXTENDED))
			{
				// error in compilation
				printf("Could not compile pattern\n");
				exit(1);
			}

			buff_status = regexec(&buff_re, pre_buffer, 2, buff_match, 0);
			if(buff_status == 0)
			{
				// extract trimmed portion
				printf("start of trimmed portion: %d\n", buff_match[1].rm_so);
				printf("end of trimmed portion:	  %d\n", buff_match[1].rm_eo);
				printf("to concatenate: \'%s\'\n", pre_buffer + buff_match[1].rm_so);
				strcat(formatted, pre_buffer + buff_match[1].rm_so);

			}else
			{
				printf("Error: no match");
				exit(1);
			}

			free(buff_num);
			free(buff_patt);
		}else
		{
			strcat(formatted, pre_buffer);
		}

		if(post_len > max_post_len)
		{
			// need to trim the post buffer
		}

		free(pre_buffer);
		free(post_buffer);
	}
	else
	{
		printf("Error, no match found\n");
		exit(1);
	}

	free(pattern);
	return formatted;
}



/*
 * Constructs a regex pattern based on the word and buffer sizes passed
 * 
 * Using Python regex syntax, the string returned would be of the form
 *		\b(.{0,pre_max})\bindex_word\b(.{0,post_max})\b
 */
char * get_pattern(char * index_word, int pre_max, int post_max)
{
	// we will use ^|$|[^[:alnum:]_] in place of \b
	// https://stackoverflow.com/questions/38847671/regex-is-not-working-in-c

	return NULL;
}


void word_boundary_approach()
{
	// Regex
	int status;
	regex_t re;
	regmatch_t match[5];


	char * index_word = "HERE";
	int max_pre_len = centre - start;
	int max_post_len = end - centre - strlen(index_word);
	char * pre_buffer, * post_buffer;

	char * line = "this right here is an example string";
	char * extract;
	// char * pattern = get_pattern(index_word, max_pre_len, max_post_len);
	char * pattern = "(.*)^|$|[^[:alnum:]_]HERE^|$|[^[:alnum:]_]( Is)";
	printf("pattern: %s\n", pattern);

	// compile and check for error
	if(regcomp(&re, pattern, REG_ICASE|REG_EXTENDED))
	{
		// error in compilation
		printf("Could not compile pattern\n");
		exit(1);
	}

	status = regexec(&re, line, 4, match, 0);
	if(status == 0)
	{
		// we have a match
		printf("MATCH\n");

		int match_len = match[0].rm_eo - match[0].rm_so;
		int pre_len = match[1].rm_eo - match[1].rm_so;
		int post_len = match[2].rm_eo - match[2].rm_so;

		extract = (char *) calloc(match_len + 1, sizeof(char));
		pre_buffer = (char *) calloc(pre_len + 1, sizeof(char));
		post_buffer = (char *) calloc(post_len + 1, sizeof(char));
		if(extract == NULL || pre_buffer == NULL || post_buffer == NULL)
		{
			printf("Calloc Error\n");
			exit(0);
		}

		strncpy(extract, line + match[0].rm_so, match_len);
		strncpy(pre_buffer, line + match[1].rm_so, pre_len);
		strncpy(post_buffer, line + match[2].rm_so, post_len);

		printf("whole match 	 : |%s|\n", extract);
		printf("pre_buffer match : |%s|\n", pre_buffer);
		printf("post_buffer match: |%s|\n", post_buffer);

		free(extract);
		free(pre_buffer);
		free(post_buffer);
	}else
	{
		printf("NO MATCH\n");
	}

	regfree(&re);
}


void using_regex()
{

	/* get_output_lines() idea :
	 *
	 * Do the formatting inside the function
	 *
	 * Obviously need to find the word within the line -> that is easy enough
	 *
	 * want regex to match a boundary followed by some number of characters
	 * followed by the word, then followed by some number of characters
	 * then a word boundary.
	 *
	 * we then just need to copy that selection into a different piece of memory and capitalize the index word.
	 *
	 * this could be done at least 2 basic ways
	 *		1. 	Copy pre-buffer
 	 *			strcat to add the capitalized index word
 	 *			copy post_buffer into a tmp string
 	 *			strcat to add the post_buffer to index word
	 *				
	 *		2.	Copy the whole selection (match[0])
	 *			Capitalize the index word inside
	 *
	 * 2 will be easier if we refactor our str_to_upper function to also take a max length param
	 */


	// pattern = 	"\b *(.{0,"
	// 				+str(pre_max)+ 
	// 				"})\b" 
	// 				+index_word+ 
	//				"\b(.{0," 
	// 				+str(post_max)+ 
	//				"})\b"

	// If there is no \b equivalent:
	// We can do 3 groups
	// group 1 = pre-buffer
	// group 2 = index_word
	// group 3 = post-buffer
	// 
	// We extract these into independent strings
	// if(strlen(pre_buffer > 20)) { use regexes to cut it down to size }
	// if(strlen(post_buffer > 60 - (30 + strlen(index_word)) )) { use regexes to cut it down to size }
	// capitalize index_word
	// concatenate the 3 strings w strcat

	// regcomp returns 0 on successful compilation
	// regexec returns 0 if a match is found
	// must use regfree once finished

	// to extract groups, use strncpy & the indecies specified 
	// in match[group_number]

	// REGEXEC
	// param 1 is a pointer to a regex_t variable
	// param 2 is the line to be searched
	// param 3 is number of groupings
	// param 4 is array of groupings 
	// 		(should have at least param 3 entries)
	// param 5 is for flags

	
	// Formatting
	int max_pre_len = centre - start;
	int max_post_len;

	char *pre_buffer, *post_buffer;

	// Regex
	int status;
	regex_t re;
	regmatch_t match[5];

	wordnode_t * words = NULL;
	words = add_end(words, new_word("THIS", 0));  // edge case (first word)
	words = add_end(words, new_word("LONG", 0));  // should all fit on the line
	words = add_end(words, new_word("LINE", 0));  // case: trim post_buffer only  
	words = add_end(words, new_word("WORDS", 0));  // case: trim pre_buffer only  
	words = add_end(words, new_word("CASES", 0)); // edge case (last word)
	char * line = "this line is quite long and words will give cases";
	

	// To ensure we don't free memory we haven't allocated
	char * pattern = (char *) malloc(sizeof(char));
	if(pattern == NULL)
	{
		printf("error allocating memory\n");
	}

	char * pre_patt= "(.*)(";
	char * post_patt = ")(.*)";

	wordnode_t * p;
	int len;
	for(p = words; p != NULL; p = p->next)
	{
		max_post_len = end - (centre + strlen(p->w));

		// Construct pattern -> concatenate pre_patt, index word, post_pat
		// Will need to re-allocate
		len = strlen(pre_patt) + strlen(post_patt) + strlen(p->w) + 1;
		pattern = realloc(pattern, sizeof(char) * len);
		if(pattern == NULL)
		{
			printf("error reallocating memory\n");
		}
		memset(pattern, '\0', len);
		strcat(pattern, pre_patt);
		strcat(pattern, p->w);
		strcat(pattern, post_patt);

		// compile and check for error
		if(regcomp(&re, pattern, REG_ICASE|REG_EXTENDED))
		{
			// error in compilation
			printf("Could not compile pattern\n");
			exit(1);
		}
	
		status = regexec(&re, line, 4, match, 0);
		if(status == 0)
		{
			// we now have access to each of the 3 groups.
			// need to trim the pre and post buffers according to their sizes.
			int pre_len = match[1].rm_eo - match[1].rm_so; 
			if(pre_len > max_pre_len)
			{
				// trim pre_buffer 
				pre_buffer = (char *) emalloc(sizeof(char) * pre_len + 1);
				memset(pre_buffer, '\0', pre_len + 1);
				strncpy(pre_buffer, line + match[1].rm_so, pre_len);


				// now we want to use regexes to remove words at the beginning of the 


				free(pre_buffer);
			}

			if(match[3].rm_eo - match[3].rm_so > max_post_len)
			{
				// trim post_buffer 
			}


		}else
		{
			printf("ERROR: NO MATCH\n");
			exit(1);
		}

		// Free memory that was allocated in regcomp
		regfree(&re);
	}

	free(pattern);
}


void using_getline()
{
	// it will realloc automatically - only free line ONCE
	// need to remove newline character?
	// returns -1 at EOF
	// can check if copied == 1 for blank line
	
	char * line = NULL;
	long unsigned int num_read;

	int copied = getline(&line, &num_read, stdin);
	while(copied != -1)
	{
		printf("read: %lu\tcopied: %d\n", num_read, copied);
		printf("line: %s\n", line);

		copied = getline(&line, &num_read, stdin);
	}

	free(line);
}	


// Testing add_sorted in linkedlist.[ch]
void test_add_sorted()
{
	wordnode_t * list1 = NULL;
	wordnode_t * list2 = NULL;
	
	list1 = add_sorted(list1, new_word("a", 0));
	list1 = add_sorted(list1, new_word("c", 2));
	list1 = add_sorted(list1, new_word("b", 1));
	list1 = add_sorted(list1, new_word("d", 3));
	list1 = add_sorted(list1, new_word("a", 4));
	list1 = add_sorted(list1, new_word("c", 5));
	list1 = add_sorted(list1, new_word("b", 6));
	list1 = add_sorted(list1, new_word("d", 7));

	list2 = add_sorted(list2, new_word("a", 0));
	list2 = add_sorted(list2, new_word("a", 9));
	list2 = add_sorted(list2, new_word("a", 7));
	list2 = add_sorted(list2, new_word("a", 6));
	list2 = add_sorted(list2, new_word("a", 8));
	list2 = add_sorted(list2, new_word("d", 3));
	list2 = add_sorted(list2, new_word("c", 2));
	list2 = add_sorted(list2, new_word("b", 1));

	printf("list 1:\n");
	apply(list1, &print_list, NULL);

	printf("\nlist 2:\n");
	apply(list2, &print_list, NULL);
}


// Testing in_list in linkedlist.[ch]
void test_in_list()
{
	wordnode_t *list = NULL; 

	assert(in_list(list, "b") == 0);

	list = add_front(list, new_word("a", 0));
	assert(in_list(list, "b") == 0);
	list = add_front(list, new_word("c", 2));
	assert(in_list(list, "b") == 0);
	list = add_front(list, new_word("b", 1));
	assert(in_list(list, "b") == 1);
	list = add_front(list, new_word("d", 3));
	
	assert(in_list(list, "a") == 1);
	assert(in_list(list, "b") == 1);
	assert(in_list(list, "c") == 1);
	assert(in_list(list, "d") == 1);
	assert(in_list(list, "x") == 0);
	assert(in_list(list, "y") == 0);
	assert(in_list(list, "z") == 0);
}

void print_list(wordnode_t *list, void * arg)
{
	printf("(%s,%d)\n",list->w, list->val);
}


/*
 * Capitalizes n characters of the given string
 */
void str_toupper(char * w, int n)
{
	int i;
	for(i = 0; i < n; i++)
	{
		*(w + i) = toupper(*(w + i));
	}	
}