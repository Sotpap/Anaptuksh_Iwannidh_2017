#include "trie.h"
int count = 0;
Trie* Init_Trie()
{
    Trie* trie = malloc(sizeof(Trie));

    trie->root = malloc(SIZE*sizeof(Trie_Node));
    trie->root->size = 0;
    trie->root->children = malloc(sizeof(Trie_Node*));

    trie->root->children[0] = malloc(SIZE*sizeof(Trie_Node));
    memset(trie->root->children[0],'\0',SIZE*sizeof(Trie_Node));

    return trie;
}

Trie_Node* New_Node(char* word,char is_final)
{
    Trie_Node* new_node = malloc(sizeof(Trie_Node));

    new_node->is_final = is_final;

    new_node->word = malloc((strlen(word)+1)*sizeof(char));
    strcpy(new_node->word,word);

    new_node->size = 0;

    new_node->children = malloc(sizeof(Trie_Node*));

    new_node->children[0] = malloc(SIZE*sizeof(Trie_Node));
    memset(new_node->children[0],'\0',SIZE*sizeof(Trie_Node));

    return new_node;

}
int is_child(Trie_Node* node_array,char* word,int size)
{
    for(int i = 0 ; i < size ; i++)
    {
        if(strcmp(word,node_array[i].word)==0) return i;
    }
    return -1;
}
void Insert_Ngram(Trie* trie,char* ngram)
{
    char* current_word = strtok(ngram," \n");
    char* remaining_ngram = strtok(NULL,"\n");

    Trie_Node** current_node_array = &(trie->root);
    int position;
    int size;
    while(current_word!=NULL)
    {

        position = is_child(*current_node_array,current_word,(*current_node_array)->size);
        if(position == -1 )/*If we have not found word*/
        {
            size = (*current_node_array)->size;


            Trie_Node* new_node = NULL;
            if(remaining_ngram == NULL) new_node = New_Node(current_word,'T');
            else new_node = New_Node(current_word,'F');

            //printf("tha valw to %s sto %p sthn thesh %d\n",current_word,current_node_array[0] ,(*current_node_array)->size);

            current_node_array[0][size] = *new_node;
            ((*current_node_array)->size)++;
            size = (*current_node_array)->size;
            if(size % SIZE == 0)
            {
                *current_node_array = realloc(*current_node_array,(size*size)*sizeof(Trie_Node));
            }



            current_node_array = new_node->children;

        }
        else
        {
            if(remaining_ngram == NULL) current_node_array[0][position].is_final = 'T';


            current_node_array = (*current_node_array)[position].children;


        }
        current_word = strtok(remaining_ngram," \n");
        remaining_ngram = strtok(NULL,"\n");
    }


}
void Search_Ngram(Trie* trie, char* ngram)
{
    char* result = malloc((strlen(ngram)+1)*sizeof(char));
    memset(result,'\0',(strlen(ngram)+1)*sizeof(char));

    char* current_word = strtok(ngram," \n");
    char* remaining_ngram = strtok(NULL,"\n");
    char* on_going_ngram = NULL;



    Trie_Node** current_node_array = &(trie->root);
    int position;
    while(current_word!=NULL)
    {

        position = is_child(*current_node_array,current_word,(*current_node_array)->size);
        if(position == -1 )/*If we have not found word*/
        {
            if(on_going_ngram!= NULL)
            {
                free(on_going_ngram);
                on_going_ngram = NULL;
            }
            if(current_node_array != &trie->root)
            {
                current_node_array = &(trie->root);
            }
            else
            {
                current_word = strtok(remaining_ngram," \n");
                remaining_ngram = strtok(NULL,"\n");
            }

        }
        else
        {
            if(on_going_ngram == NULL)
            {
                on_going_ngram = malloc((strlen(current_word)+1)*sizeof(char));
                sprintf(on_going_ngram,"%s", current_word);
            }
            else
            {
                on_going_ngram = realloc(on_going_ngram, (strlen(on_going_ngram)+(strlen(current_word)+2))*sizeof(char));
                sprintf(on_going_ngram,"%s %s",on_going_ngram, current_word);
            }
            if(current_node_array[0][position].is_final == 'T')
            {

                if(strstr(result,on_going_ngram) == NULL)
                {


                    if(remaining_ngram == NULL) sprintf(result,"%s%s",result,on_going_ngram);
                    else sprintf(result,"%s|%s",result,on_going_ngram);

                }
            }

            current_node_array = (*current_node_array)[position].children;

            current_word = strtok(remaining_ngram," \n");
            remaining_ngram = strtok(NULL,"\n");
        }

    }
    puts(result);

}

void Print_Trie(Trie_Node* root)
{
    for(int i = 0 ; i < root->size ; i++)
    {
        Print_Trie(*(root[i].children));
        printf("%s me %c\n",root[i].word,root[i].is_final);
    }
}