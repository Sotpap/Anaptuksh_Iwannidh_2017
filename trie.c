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
                printf("sto realloc me size = %d\n",size);
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
    printf("%d\n",++count);

}

void Print_Trie(Trie_Node* root)
{
    for(int i = 0 ; i < root->size ; i++)
    {
        Print_Trie(*(root[i].children));
        printf("%s me %c\n",root[i].word,root[i].is_final);
    }
}