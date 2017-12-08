//
// Created by tentas on 10/11/17.
//

#ifndef PROJECT_TRIE_HEADER_H
#define PROJECT_TRIE_HEADER_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "top.h"
#include "Linear_Hashing.h"
#include "bloom.h"


#define SIZE 20
#define STRING_TABLE_SIZE 15

typedef struct Trie_Node
{
    int fere;					//labeling
    char* word;					//strings saved in this node
    int is_final;				//is node final? boolean
    struct Trie_Node*  children; 		/*Array of pointers to children*/
    signed short int* string_table;		// table used to store each word's size and status as final
    struct Hash_Table* hash_table;
    int string_table_size;			// size of string table
    int size; 					//How many children this node has
} Trie_Node;
/*
typedef struct Trie_Node
{
    int fere;
    char* word;
    int is_final;
    struct Trie_Node*  children;

    int size; //How many children this node has
} Trie_Node;
*/

typedef struct Trie
{
    Trie_Node*  root;
    int depth;
}Trie;


typedef struct Delete_Pair {
    Trie_Node *father;
    Trie_Node *child;
    int position;
}Delete_Pair;


Trie_Node* New_Node(char* word,int is_final);

Trie* Init_Trie(void);

//Trie_Node* New_Node(char* word,char is_final);
void Insert_Ngram(Trie* root,char* ngram);
void Search_Ngram(Trie* index, char* ngram,int, Index*);
void Search_Substream(Trie_Node* root,char* ngram, char** result,int , short int*, Index*,char**);
int binary_search(Trie_Node* current_node,int min,int max,char* word);
int Delete_Ngram(Trie_Node* , char* ngram,int hash_count);
void Print_Trie(Trie_Node root);
int first_depth_first(char* string,int depth);


void Insert_Static(Trie* trie, char* ngram);
void compress(Trie_Node* current);
void  pull( Trie_Node* top_node);
void Search_Static(Trie* trie,char* ngram,int count,Index* );
void Static_Substream(Trie_Node* root,char* ngram, char** result, short int*,Index* ,int count);
int Static_binary_search(Trie_Node* current_node,int min,int max,char* word);

#endif //PROJECT_TRIE_HEADER_H
