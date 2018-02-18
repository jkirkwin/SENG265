/* Jamie Kirkwin
 * Seng 265 Assignment 1
 * V00875987
 * kwic1.c
 * Feb 8, 2018
 * Intro to C, indexing a text file by keyword
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> //for toupper()

#define MAX_EXCLUDED 20
#define MAX_INDEX_WORDS 100
#define MAX_LINES 100
#define MAX_INDEX_LINES 3500 // max words per line * max lines
#define MAX_LINE_LEN 70
#define SEPERATOR_LEN 2


// arrays to store and manage input
char excluded[MAX_EXCLUDED][MAX_LINE_LEN +1];
char index_words[MAX_INDEX_WORDS][MAX_LINE_LEN +1];
char index_lines[MAX_LINES][MAX_LINE_LEN +1];
char output_lines[MAX_INDEX_LINES][MAX_LINE_LEN +1];

// function prototypes
void remove_newline(char*, int);
void get_input();
void nullify_arrays();
void index();
int in_lists(char*);
int get_num_words();
int strcmp_wrapper();
int find_and_upper(char*, char*, int);

// debugging prototypes
void print_hex(int, int, int, int, char*);
void print_lists();
void print_str_in_hex(char*, int);

int main()
{
    nullify_arrays();
    
    // Fill index_words, index_lists, and excluded with input
    get_input();

    // Use qsort to sort index_words
    // Adapted from in-class example, wordlist.c, by Dr Zastre - file found on connex: wordlist_c.pdf
    qsort(index_words, get_num_words(), (MAX_LINE_LEN+1) * sizeof(char), strcmp_wrapper);
    
    // index lines and arrange output in output_lines
    index();    

    // Print results
    for(int i = 0; i < MAX_INDEX_LINES; i++)
    {
        if(output_lines[i][0] == '\0') break;

        printf("%s\n", output_lines[i]);
    } 

    return 0;
}


/* Indexes the list of lines based on the sorted list of index words */
void index()
{
    int output_index = 0;

    char line_buffer[MAX_LINE_LEN + 1];
    memset(line_buffer, '\0', MAX_LINE_LEN +1);

    char* word = NULL;

    for(int i = 0; i < MAX_INDEX_WORDS; i++)
    {
        //for each non-excluded word: 

        if(index_words[i][0] == '\0') break;
       
        word = index_words[i];
       
        for(int j = 0; j < MAX_LINES; j++)
        {
            // for each line of input:

            if(index_lines[j][0] == '\0') break; // no more occurances of word

            // copy line into line_buffer
            strncpy(line_buffer, index_lines[j], MAX_LINE_LEN);

            // if line contains word
            if( find_and_upper(line_buffer, word, strlen(word)) == 1 )
            {
                // word is capitalized within buffer   
                // copy from buffer into output array
                strncpy(output_lines[output_index++], line_buffer, MAX_LINE_LEN);


            } //end if

        } // end inner for

    } //end outer for 
}


/* Uses strtok() to search str for an instance of wrd
 * if found, that instance is capitalized and return 1
 * else return 0
 */
int find_and_upper(char *str, char *wrd, int wrd_len)
{

    int str_len = strlen(str);

    char *token = NULL;
    char *ptr = NULL;
    int index = 0;

    char tmp[str_len +1];    
    memset(tmp, '\0', str_len +1);
    strncpy(tmp, str, str_len);

    token = strtok(tmp, " "); // token points to the first word in the line
    do
    {
        if( strcmp(token, wrd) == 0 )
        {
            // Find the index at which the instance of word begins
            index = 0;
            ptr = tmp;

            while( strcmp(ptr + index, wrd) != 0 )
            {
                index++;
            }

            // capitalize the word within str
            for(int i = 0; i < wrd_len; i++)
            {
                *(str + index + i) = toupper( *(str + index + i) );
            }

            return 1; // Success - Found and Capitalized
        }

        token = strtok(NULL, " ");

    }while(token);   
    
    return 0; // No instance of wrd in str
}


/* Credit goes to Dr Zastre for this function.
 * Can be found on connex, titled wordlist_c.pdf
 * Used to enable qsort for strings
 */
int strcmp_wrapper(const void *a, const void *b)
{
    // Cast the pointers to be char pointers so we can use strcmp
    char *sa = (char *) a; 
    char *sb = (char *) b;

    return strcmp(sa,sb);
}

/* Counts the number of strings in word_list and returns that value */
int get_num_words()
{
    int count = 0;
    while( index_words[count][0] != '\0' ) 
    { 
        count++;
    }
    return count;
}


/*  Uses knowlege of input format to fill the excluded and index lists
 *  and stores each line of input (sans newline) in index_lines
 */
void get_input()
{

    // First get rid of version number and record seperator
    char seperator[MAX_LINE_LEN+1];
    fgets(seperator, MAX_LINE_LEN +1, stdin);
    fgets(seperator, MAX_LINE_LEN +1, stdin);
    remove_newline(seperator, MAX_LINE_LEN);
    
    // Input excluded words 
    int index = 0;
    char buffer[MAX_LINE_LEN +1];
    fgets(buffer, MAX_LINE_LEN+1,stdin);
    remove_newline(buffer, MAX_LINE_LEN);

    while(strcmp(buffer, seperator) != 0)
    {
        strncpy(excluded[index++], buffer, MAX_LINE_LEN);
        
        fgets(buffer, MAX_LINE_LEN, stdin);

        remove_newline(buffer, MAX_LINE_LEN);
    }
    
    // Now we input the index lines and extract the index words
    index = 0;
    int line_num = 0;
    char *token;
    char line[MAX_LINE_LEN +1];
   
    while( fgets(buffer, MAX_LINE_LEN+1, stdin))  // Stops at EOF 
    {

        if(buffer[0] == '\n') continue; // To skip blank lines

        //remove newline from buffer
        remove_newline(buffer, MAX_LINE_LEN);

        // add the line to index_lines
        strncpy(index_lines[line_num], buffer, MAX_LINE_LEN);
        line_num++;

        // Adapted from SENG 265 Lab 4 Slides by Amanda Dash
        // Use strtok() to tokenize the line by whitespace
        // then add each non-excluded word to index_words
        strncpy(line, buffer, MAX_LINE_LEN);
        token = strtok(line, " ");
        while(token != 0)
        {
            // search the excluded word list and index word list for a match
            // add to index list if no match is found

            if( in_lists(token)==0 )
            {
                strncpy(index_words[index++], token, MAX_LINE_LEN);
            }

            token = strtok(NULL, " ");
        } //end if
    } // end while
}


/* Returns 1 if a matching string is contained in excluded or index_words
 * returns 0 otherwise
 */
int in_lists(char* token)
{
    for(int i = 0; i < MAX_EXCLUDED; i++)
    {
        // check if list is fully searched
        if(excluded[i][0] == '\0') break;

        if( strcmp(token, excluded[i]) == 0) return 1;
    }

    for(int i = 0; i < MAX_INDEX_WORDS; i++)
    {   
        // check if list is fully searched
        if(index_words[i][0] == '\0') break;

        if( strcmp(token, index_words[i]) == 0) return 1;  
    }

    return 0;
}


/* Sets all characters in each of the four 2d arrays to null */
void nullify_arrays()
{
    for(int i = 0; i < MAX_EXCLUDED; i++)
    {
        memset(excluded[i], '\0', MAX_LINE_LEN);
    }

    for(int i = 0; i < MAX_INDEX_WORDS; i++)
    {
        memset(index_words[i], '\0', MAX_LINE_LEN);
    }

    for(int i = 0; i < MAX_LINES; i++)
    {
        memset(index_lines[i], '\0', MAX_LINE_LEN);                
    }

    for(int i = 0; i < MAX_INDEX_LINES; i++)
    {
        memset(output_lines[i], '\0', MAX_LINE_LEN);                
    }
}

/* Replaces any newline characters (of the first n) with null */
void remove_newline(char* p, int n)
{
    for(int i = 0; i < n; i++)
    {
        if( *(p+i) == '\n' ) *(p+i) = '\0';
    }
}


/* A debugging method that prints the contents of the input arrays in hex
 * the 3 flags determine which to print
 * max_lines sets the cutoff number of index_lines to print
 * msg is printed at the beginning of the call
 */
void print_hex(int ex_flag, int wrd_flag, int line_flag, int max_lines, char* msg)
{
    int cap = max_lines;
    if(max_lines >= MAX_LINES) cap = MAX_LINES;

    printf("PRINT_HEX: %s\n", msg);
    
    if(ex_flag)
    {
        printf("\n\tExcluded:\n");
        for(int i = 0; i < MAX_EXCLUDED; i++)
        {
            printf("excluded[%d] = ", i);
            for(int j  = 0; j < MAX_LINE_LEN; j++)
            {
                printf("%x-", excluded[i][j]);
            }
            printf("\n");
        }
    } // end if

    if(wrd_flag)
    {
        printf("\n\tIndex Words:\n");
        for(int i = 0; i < MAX_INDEX_WORDS; i++)
        {
            printf("index_words[%d] = ", i);
            for(int j  = 0; j < MAX_LINE_LEN; j++)
            {
                printf("%x-", index_words[i][j]);
            }
            printf("\n");
        }

    } // end if

    if(line_flag)
    {
        printf("\n\tIndex Lines:\n");
        for(int i = 0; i < cap; i++)
        {
            printf("index_lines[%d] = ", i);
            for(int j  = 0; j < MAX_LINE_LEN; j++)
            {
                printf("%x-", index_lines[i][j]);
            }
            printf("\n");
        }
    } // end if
}

/* A debugging function 
 * Prints each of the 3 lists which store input until an empty string is found
 */
 void print_lists()
{
    printf("\n\tExcluded:\n");
    for(int i = 0; i < MAX_EXCLUDED; i++)
    {
        if(excluded[i][0] == '\0') break; // list fully traversed
        printf("excluded[%d] = %s\n", i, excluded[i]);
    }
    
    printf("\n\tIndex Words:\n");
    printf("get_num_words = %d\n", get_num_words() );
    for(int i = 0; i < MAX_INDEX_WORDS; i++)
    {
        if(index_words[i][0] == '\0') break; // list fully traversed
        printf("index_words[%d] = %s\n", i, index_words[i]);
    }
    
    printf("\n\tIndex Lines:\n");
    for(int i = 0; i < MAX_LINES; i++)
    {
        if(index_lines[i][0] == '\0') break; // list fully traversed
        printf("index_lines[%d] = %s\n", i, index_lines[i]);
    }
}


// Prints the hex value of each character of the string passed in 
void print_string_in_hex(char* str, int length)
{
    printf("printing string : \'%s\'\n", str);

    for(int i = 0; i < length; i++) printf("%x-", *(str + i));
    
    printf("\n");
}
