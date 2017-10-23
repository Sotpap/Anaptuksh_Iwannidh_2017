//
// Created by tentas on 10/11/17.
//

#ifndef PROJECT_TRIE_HEADER_H
#define PROJECT_TRIE_HEADER_H

#endif //PROJECT_TRIE_HEADER_H
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define SIZE 10

typedef struct Trie_Node {
    char* word;
    char is_final;
    struct Trie_Node*  children; /*Array of pointers to children*/
    int size; //How many children this node has
} Trie_Node;

typedef Trie_Node* Trie_Node_Ptr;

typedef struct Trie
{
    Trie_Node_Ptr  root;
}Trie;
typedef Trie* Trie_Ptr;


Trie_Ptr Init_Trie(void);

Trie_Node New_Node(char* word,char is_final);
int Insert_Ngram(Trie* root,char* ngram) ;
void Search_Ngram(Trie_Node_Ptr root,Trie_Node_Ptr node,char* ngram,char* on_going_ngram,FILE* fp);

void Print_Trie(Trie_Node_Ptr root);