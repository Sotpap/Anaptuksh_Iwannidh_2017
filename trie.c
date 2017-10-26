#include "trie.h"

Trie* Init_Trie(void)
{
    Trie* trie = malloc(sizeof(Trie));

    trie->root = malloc(sizeof(Trie_Node));

    trie->root->word = NULL;

    trie->root->size = 0;

    trie->root->is_final = 0;

    trie->root->children = malloc(SIZE*sizeof(Trie_Node));

    memset(trie->root->children,'\0',SIZE*sizeof(Trie_Node));

    return trie;
}

void Insert_Ngram(Trie* trie,char* ngram) {
    Trie_Node *current_node = trie->root;

    char *current_word = strtok(ngram, " \n");  // Get first word of current ngram

    char *remaining_ngram = strtok(NULL, "\n");  // Get the rest ngram

    int i , position,node_size;

    while (current_word != NULL) {

        position = -1;

        node_size = current_node->size;

        ///Checking if current word exists in current_nodes children
        for (i = 0; i < node_size; i++)
        {
            if (strcmp(current_word, current_node->children[i].word) == 0)
            {
                position = i;
                break;
            }
        }

        if (position == -1) /// If we haven't found the word in current_node's children
        {
            Trie_Node* new_node = malloc(sizeof(Trie_Node));  /// Creating new node

            new_node->word = malloc((strlen(current_word)+1)*sizeof(char));
            strcpy(new_node->word, current_word);

            if(remaining_ngram == NULL) new_node->is_final = 1;
            else new_node->is_final = 0;

            new_node->size = 0;

            new_node->children = malloc(SIZE*sizeof(Trie_Node));

            memset(new_node->children,'\0',SIZE*sizeof(Trie_Node));

            if((node_size % SIZE == 0) && (node_size != 0))
            {
                //Trie_Node* temp = *(current_node->children);
                //temp = realloc(temp, (node_size*2)*sizeof(Trie_Node));
                //*(current_node->children) = temp;

                current_node->children = realloc(current_node->children, (node_size*2)*sizeof(Trie_Node));
                //memset(new_node->children+(node_size*sizeof(Trie_Node)),'\0',(node_size)*sizeof(Trie_Node));
            }

            current_node->children[node_size] = *new_node; /// Inserting new node to current's children.

            current_node->size += 1;
            current_node = &current_node->children[node_size]; /// Passing new node as current to keep inserting the rest of the ngram
        }
        else  ///If we found the word in current_node's children
        {
            if (remaining_ngram == NULL) current_node->children[position].is_final = 1; /// Checking if current word is the final of the ngram

            current_node = &current_node->children[position];   /// Passing new node as current to keep inserting the rest of the ngram

        }

        current_word = strtok(remaining_ngram, " \n"); ///Getting next word if exists.

        remaining_ngram = strtok(NULL, " "); /// Getting the rest of the ngram if exists.

    }
}

void Search_Ngram(Trie* trie,char* ngram) {
    Trie_Node *current_node = trie->root;

    char *current_word = strtok(ngram, " \n");  // Get first word of current ngram

    char *remaining_ngram = strtok(NULL, "\n");  // Get the rest ngram

    char* on_going_ngram = NULL;

    char* result = malloc((strlen(ngram)+1)*sizeof(char));
    memset(result,'\0',(strlen(ngram)+1)*sizeof(char));

    int i , position,node_size;

    while (current_word != NULL)
    {

        position = -1;

        node_size = current_node->size;

        ///Checking if current word exists in current_nodes children
        for (i = 0; i < node_size; i++)
        {
            if (strcmp(current_word, current_node->children[i].word) == 0)
            {
                position = i;
                break;
            }
        }

        if (position == -1) /// If we haven't found the word in current_node's children
        {
            if(on_going_ngram!= NULL)
            {
                free(on_going_ngram);
                on_going_ngram = NULL;
            }
            if(current_node != trie->root)
            {
                current_node = (trie->root);
            }
            else
            {
                current_word = strtok(remaining_ngram," \n");
                remaining_ngram = strtok(NULL,"\n");
            }
        }
        else  ///If we found the word in current_node's children
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
            if(current_node->children[position].is_final)
            {
                if(strstr(result,on_going_ngram) == NULL)
                {
                    if(remaining_ngram == NULL) sprintf(result,"%s%s",result,on_going_ngram);
                    else sprintf(result,"%s|%s",result,on_going_ngram);
                }
            }

            current_node = &current_node->children[position];

            current_word = strtok(remaining_ngram," \n");
            remaining_ngram = strtok(NULL,"\n");
        }
    }

    if(strlen(result) == 0) printf("-1\n");
    else puts(result);
    free(result);
    free(on_going_ngram);
}

void Print_Trie(Trie_Node node)
{
    for(int i = 0; i < node.size; i++)
    {
        Print_Trie(node.children[i]);

    }
    if(node.word != NULL)
    {
        printf(" %s |", node.word);

    }
}