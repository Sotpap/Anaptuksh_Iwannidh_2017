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




    //Search_Ngram(trie->root,trie->root,ngram,NULL,fp);


   Print_Trie(trie->root);
}

