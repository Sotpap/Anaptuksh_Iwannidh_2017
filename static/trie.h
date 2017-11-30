//
// Created by tentas on 10/11/17.
//

#ifndef PROJECT_TRIE_HEADER_H
#define PROJECT_TRIE_HEADER_H

#endif //PROJECT_TRIE_HEADER_H
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define SIZE 20

typedef struct Trie_Node
{
    int fere;					//labeling
    char* word;					//strings saved in this node
    int is_final;				//is node final? boolean
    struct Trie_Node*  children; 		/*Array of pointers to children*/
    signed short int string_table[10];		// table used to store each word's size and status as final
    int string_table_size;			// size of string table
    int size; 					//How many children this node has
} Trie_Node;

typedef struct Trie
{
    Trie_Node*  root;
    int depth;
}Trie;



Trie* Init_Trie(void);

//Trie_Node* New_Node(char* word,char is_final);
void compress(Trie* trie);							// compressing function as asked
//void pull(Trie_Node* top_node, Trie_Node* hyper_node);
void pull(Trie_Node* top_node);
void add_to_hyper_node(Trie_Node* current_node,Trie_Node* hyper_node);		//puttin a plain node in a hyper node
void Insert_Ngram(Trie* root,char* ngram);
void Search_Ngram(Trie* index, char* ngram,int);
void Search_Substream(Trie_Node* root,char* ngram, char** result,int);
int binary_search(Trie_Node* current_node,int min,int max,char* word);
int Delete_Ngram(Trie_Node* current_node, char* ngram);
void Print_Trie(Trie_Node root);
