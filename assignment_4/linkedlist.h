#ifndef _LINKEDLIST_H_
#define _LINKEDLIST_H_

typedef struct word wordnode_t;
struct word {
	int val;
    char   *w;
    wordnode_t *next;
};

// Zastre's Prototypes
wordnode_t *new_word(char *w, int);
wordnode_t *add_front(wordnode_t *, wordnode_t *);
wordnode_t *add_end(wordnode_t *, wordnode_t *);
wordnode_t *peek_front(wordnode_t *);
wordnode_t *remove_front(wordnode_t *);
void        free_word(wordnode_t *);
void        apply(wordnode_t *, 
                void(*fn)(wordnode_t *, void *), void *arg);
void * emalloc(size_t n);

// Jamie's Prototypes
wordnode_t *add_sorted(wordnode_t *, wordnode_t *);
int in_list(wordnode_t *, char *);

#endif
