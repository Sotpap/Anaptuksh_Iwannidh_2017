#include "trie.h"

Trie_Ptr Init_Trie() {
    // Dhmiourgw to root to opoio tha einai deikths se enan pinaka apo nodes

    Trie_Ptr trie = malloc(sizeof(Trie));

    trie->root = malloc((SIZE+1)*sizeof(Trie_Node));
    trie->root->word = NULL;

    trie->root->children = malloc(SIZE*sizeof(Trie_Node));
    memset(trie->root->children,'\0',SIZE*sizeof(Trie_Node));
    trie->root->size = 0;

    return trie;
}

Trie_Node New_Node(char* word,char is_final)
{
    Trie_Node new_node;

    new_node.word = malloc((strlen(word)+1)*sizeof(char));
    strcpy(new_node.word,word);

    new_node.is_final = is_final;

    new_node.size = 0;
    new_node.children = malloc(SIZE*sizeof(Trie_Node));

    memset(new_node.children,'\0',SIZE*sizeof(Trie_Node));



    return new_node;
}

Trie_Node_Ptr is_child(Trie_Node* node_array,char* word)
{
    for(int i = 0 ; i < node_array->size; i++)
    {
        if(strcmp(word,node_array[i].word) == 0) /*If we found word*/
        {
            return &(node_array->children[i]);
        }

    }
    return NULL;
}

int Insert_Ngram(Trie* index,char* ngram) {

    Trie_Node *current_node = index->root;

    char *current_word = strtok(ngram, " \n"); // Get first word of current ngram

    char *remaining_ngram = strtok(NULL, "\n"); // Get the rest ngram

    Trie_Node_Ptr node;
    int position;
    while (current_word != NULL)
    {
        //psaxnw ton pinaka tou current node kai gurnaw deikth sto children an to vrw
        node = is_child(current_node, current_word);
        if (node == NULL) /* If we have not found word*/
        {
            Trie_Node new_node;
            if (remaining_ngram == NULL) new_node = New_Node(current_word, 'T');
            else new_node = New_Node(current_word, 'F');
            position = current_node->size;


            int new_position = -1;
            for (int i = 0; i < current_node->size; i++) {
                if (strcmp(current_word, current_node[i].word) < 0) {
                    new_position = i;
                }
            }
            if (new_position != -1) {
                for (int i = current_node->size - 1; i >= new_position; i--) {
                    current_node[i + 1] = current_node[i];
                }
                current_node[new_position] = new_node;
            } else current_node[position] = new_node;


            (current_node->size)++;

            current_node = current_node[position].children;
        } else {
            current_node = node;
        }

        current_word = strtok(remaining_ngram, " \n");
        remaining_ngram = strtok(NULL, "\n");
    }

}


void Search_Ngram(Trie* index, char* ngram, FILE* fp)
{

    Trie_Node *current_node = index->root;

    char *current_word = strtok(ngram, " \n"); // Get first word of current ngram

    char *remaining_ngram = strtok(NULL, "\n"); // Get the rest ngram

    char* on_going_ngram;

    Trie_Node_Ptr  node;

    while(current_word!= NULL)
    {

        //psaxnw ton pinaka tou current node kai gurnaw deikth sto children an to vrw
        node = is_child(current_node,current_word);

        if(node == NULL) //An den yparxei mesa sta children.
        {

            if(remaining_ngram!=NULL)
            {
                current_node = index->root;
                free(on_going_ngram);
                on_going_ngram = NULL;
            }

        }
        else
        {

            if(on_going_ngram == NULL)
            {
                on_going_ngram = malloc((strlen(current_word)+1)*sizeof(char));
                sprintf(on_going_ngram,"%s",current_word);
            }
            else
            {
                on_going_ngram = realloc(on_going_ngram, (strlen(current_word)+strlen(on_going_ngram)+2)*sizeof(char));
                sprintf(on_going_ngram,"%s %s",on_going_ngram,current_word);
            }
            if(current_node->is_final == 'T')
            {
                puts(on_going_ngram);
            }
            current_node = node;
        }
        current_word = strtok(remaining_ngram," \n");
        remaining_ngram = strtok(NULL,"\n");
    }

}




void Print_Trie(Trie_Node_Ptr root)
{

   for(int i = 0 ; i < root->size ; i++)
   {
       Print_Trie(root[i].children);
       puts(root[i].word);
   }
}