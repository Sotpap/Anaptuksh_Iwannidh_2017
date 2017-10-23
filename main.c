//
// Created by antonis on 10/11/17.
//
#include "file_handler.h"

int main(int argc,char** argv)
{
    Arguments file_list;

    if(Get_Arguments(argc,argv,&file_list) == 0) return FILE_ERROR;

    Trie_Ptr trie = Init_Trie();



    if(Extract_From_Init(file_list.init_file, trie) == 0) return FILE_ERROR;

    FILE *fp;

    char* ngram = malloc(strlen("this is a test")+1* sizeof(char));
    strcpy(ngram,"this is a test");
    Search_Ngram(trie, ngram, fp);


   Print_Trie(trie->root);
}

