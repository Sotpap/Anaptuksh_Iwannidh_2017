//
// Created by antonis on 10/11/17.
//

#include "file_handler.h"

int Get_Arguments(int argc,char** argv,Arguments* args)
{

    args->init_file = NULL;
    args->query_file = NULL;

    int i;
    if(argc != 5)
    {
        printf("Usage: ./ngrams -i <init_file> -q <query_file>\n");
        return 0;
    }
    for(i = 0 ; i < argc ; i++)
    {
        if(strcmp(argv[i],"-i") == 0) /*If we have found -i argument we take next parameter*/
        {
            args->init_file = fopen(argv[i+1],"r");
            if(args->init_file == NULL) //If init file does not exist we terminate
            {
                perror("Error opening init file");
                return 0;
            }
        }
        else if(strcmp(argv[i],"-q") == 0) /*If we have found -q argument we take next parameter*/
        {
            args->query_file = fopen(argv[i+1],"r");
            if(args->query_file == NULL ) //If query file does not exist
            {
                perror("Error opening query file");
                return 0;
            }

        }
    }

    return 1;
}

int word_count(char* word)
{
    int counter = 1;
    while (*word != '\0')
    {
        if (*word == ' ')
        {
            counter++;
        }
        word++;
    }
    return counter;

}

int Close_Files(Arguments args)
{
    fclose(args.query_file);
    fclose(args.init_file);
    return 1;
}

int Extract_From_Init(FILE* init_file, Trie* root) /*Getting words from init file and adds them to trie*/
{
    char* line = NULL;
    size_t len = 0;


    while(getline(&line,&len,init_file)!=-1)
    {
        line[strcspn(line, "\n")] = 0;

        Insert_Ngram(root,line);

    }
    return 1;
}

int Extract_From_Query(FILE* query_file, Trie* trie) /*Getting jobs from greeks and giving them to immigrants */
{
    char* line = NULL;
    size_t len = 0;
    char* job = NULL;
    char* work = NULL;

    while(getline(&line,&len,query_file)!=-1)
    {
        line[strcspn(line, "\n")] = 0;

        job = strtok(line," ");

        work = strtok(NULL,"\n");

        if(strcmp(job,"A") == 0)
        {
            Insert_Ngram(trie,work);
        }
        else if(strcmp(job,"Q") == 0)
        {
            Search_Ngram(trie, work);
        }
        else if(strcmp(job,"D") == 0)
        {

        }
        else if(strcmp(job,"F") == 0)
        {
            printf("F\n");
        }
    }
    return 1;
}