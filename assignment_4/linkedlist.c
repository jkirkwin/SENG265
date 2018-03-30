/*
 * linkedlist.c
 *
 * Based on the implementation approach described in "The Practice 
 * of Programming" by Kernighan and Pike (Addison-Wesley, 1999).
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linkedlist.h"


void *emalloc(size_t n) {
    void *p;

    p = malloc(n);
    if (p == NULL) {
        fprintf(stderr, "malloc of %zu bytes failed", n);
        exit(1);
    }

    return p;
}


wordnode_t *new_word(char *w, int val) {
    assert( w != NULL);

    wordnode_t *temp = (wordnode_t *)emalloc(sizeof(wordnode_t));

    temp->val = val;
    temp->w = strdup(w);
    temp->next = NULL;

    return temp;
}


wordnode_t *add_front(wordnode_t *list, wordnode_t *new) {
    new->next = list;
    return new;
}


wordnode_t *add_end(wordnode_t *list, wordnode_t *new) {
    wordnode_t *curr;

    if (list == NULL) {
        new->next = NULL;
        return new;
    }

    for (curr = list; curr->next != NULL; curr = curr->next);
    curr->next = new;
    new->next = NULL;
    return list;
}


wordnode_t *peek_front(wordnode_t *list) {
    return list;
}


wordnode_t *remove_front(wordnode_t *list) {
    if (list == NULL) {
        return NULL;
    }

    return list->next;
}


void free_word(wordnode_t *word) {
    assert(word != NULL);

    if (word->w != NULL) {
        free(word->w);
    }
    free(word);
}


void apply(wordnode_t *list,
           void (*fn)(wordnode_t *list, void *),
           void *arg)
{
    for ( ; list != NULL; list = list->next) {
        (*fn)(list, arg);
    }
}


/* 
 * Added by Jamie Kirkwin
 *
 * Only to be called with a sorted list 
 *
 * Params are the head and the node to be added
 *
 * If the string field of the new node matches that of any of the 
 * current nodes, no changes are made
 *
 * Returns the head of the list
 */
wordnode_t *add_sorted(wordnode_t *list, wordnode_t *newnode)
{    
    assert(newnode != NULL && newnode->w != NULL);

    int cmp1, cmp2;
    wordnode_t *cur;

    if(list == NULL)
    {
        // case: empty list
        return add_front(list, newnode);   
    }

    cmp1 = strcmp(list->w, newnode->w);
    if(0 < cmp1)
    {
        return add_front(list, newnode);
    }else if(cmp1 == 0)
    {
        // duplicate word
        return list;
    }

    for(cur = list; cur->next != NULL; cur = cur->next)
    {
        cmp1 = strcmp(cur->w, newnode->w);
        cmp2 = strcmp(newnode->w, cur->next->w);
        if(cmp1 == 0 || cmp2 == 0)
        {
            // duplicate word
            return list;
        }else if(cmp1 < 0 && cmp2 < 0)
        {
            newnode->next = cur->next;
            cur->next = newnode;
            return list;
        }
    }

    // sanity check
    assert(cur->next == NULL);

    cmp1 = strcmp(cur->w, newnode->w);
    if(cmp1 == 0)
    {
        // duplicate word
        return list;
    }else
    {
        // sanity check
        assert(cmp1 < 0);
        
        cur->next = newnode;
        newnode->next = NULL;
    }

    return list;
}

/* 
 * Added by Jamie Kirkwin
 *
 * Returns 1 if the string passed is contained in any of the nodes 
 * in the list, returns 0 otherwise
 */
int in_list(wordnode_t *list, char *w)
{
    assert(w != NULL);
    for(; list != NULL; list = list->next)
    {
        if(strcmp(list->w, w) == 0) return 1;
    }

    return 0;
}