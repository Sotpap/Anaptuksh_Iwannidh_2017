#include "trie.h"

Trie_Ptr Init_Trie() {
    Trie_Ptr trie;


    trie = malloc(sizeof(Trie));

    trie->root = malloc(sizeof(Trie_Node)); /*Allocate space for root */

    trie->root->children = malloc(10 * sizeof(Trie_Node));

    trie->root->size = 0; /*We have no children when we begin*/

    return trie;
}

Trie_Node_Ptr New_Node(char* word, char is_final)
{

    Trie_Node_Ptr new_node;

    new_node = malloc(sizeof(Trie_Node));

    new_node->size = 0;

    new_node->word = malloc((strlen(word) + 1) * sizeof(char));
    strcpy(new_node->word, word);

    new_node->is_final = is_final;


    return new_node;


}

int Insert_Ngram(Trie_Node_Ptr root, char* ngram)
{

    char * current_word = strtok(ngram, " \n"); // Get first word of current ngram

    char * remaining_ngram = strtok(NULL, "\n"); // Get the rest ngram

    Trie_Node_Ptr current_node = root;

    while(current_word!= NULL)
    {

        int i = 0, found = 0;
        for(i = 0 ; i < current_node->size ; i++)
        {
            if (strcmp(current_node->children[i]->word, current_word) == 0)
            {
                found = 1;
                break;
            }
            else if(strcmp(current_node->children[i]->word, current_word) < 0) break;
        }
        if(found)
        {
            if(remaining_ngram == NULL)
            {
                current_node->children[i]->is_final = 'T'; ///If it was the last word of the Ngram.
                return 1;
            }
            else
            {
                current_word = strtok(ngram, " \n"); /// Get first word of current ngram

                remaining_ngram = strtok(NULL, "\n"); /// Get the rest ngram

                current_node = current_node->children[i];
            }
        }
        else
        {
            if((current_node->size%10) == 0)
            {
                if(current_node->size == 0)
                {

                    current_node->children[i]->word = malloc((strlen(current_word) + 1) * sizeof(char));

                    strcpy(current_node->children[i]->word, current_word);

                    current_node->size++;
                    if(remaining_ngram == NULL)
                    {
                        current_node->children[i]->is_final = 'T';
                        return 1;
                    }
                    else
                    {
                        current_node->children[i]->is_final = 'F';
                        current_node->children[i]->children = malloc(10*sizeof(Trie_Node));
                        current_node->children[i]->size = 0;

                        current_word = strtok(ngram, " \n"); /// Get first word of current ngram

                        remaining_ngram = strtok(NULL, "\n"); /// Get the rest ngram

                        current_node = current_node->children[i];


                    }
                }
                else
                {
                    ///Slav realloc.
                }
            }
        }
    }
}

Trie_Node_Ptr is_child(Trie_Node_Ptr node,char* word)
{
    for(int i = 0 ; i < node->size ; i++)
    {
        if(strcmp(word,node->children[i]->word) == 0) /*If we found word*/
        {
            return node->children[i];
        }
        else if(strcmp(word,node->children[i]->word) < 0)
        {
            return NULL;
        }
    }
    return NULL;
}

void Search_Ngram(Trie_Node_Ptr root,Trie_Node_Ptr node,char* ngram,char* on_going_ngram)
{
    if(ngram == NULL) return;
    char * current_word = strtok(ngram, " \n"); // Get first word of current ngram

    char * remaining_ngram = strtok(NULL, "\n"); // Get the rest ngram

    Trie_Node_Ptr current_node = is_child(node,current_word);

    if(current_node == NULL)
    {
        if(on_going_ngram)
        {
           // free(on_going_ngram);
            printf("prin kalesw remaining = %s\n",ngram);
            char* new_ngram = malloc(((strlen(remaining_ngram)+strlen(current_word))+2)*sizeof(char));
            sprintf(new_ngram,"%s %s",current_word,remaining_ngram);
            Search_Ngram(root,root,new_ngram,NULL);
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
            sprintf(on_going_ngram,"%s %s",on_going_ngram,current_word);
        }
        if(current_node->is_final == 'T')
        {
            puts(on_going_ngram);
        }
        Search_Ngram(root,current_node,remaining_ngram,on_going_ngram);

    }


}

/*char* Search_Ngram(Trie_Node_Ptr root,Trie_Node_Ptr current,char* ngram,char* on_going_ngram)
{

    char * current_word = strtok(ngram, " \n"); // Get first word of current ngram

    char * remaining_ngram = strtok(NULL, "\n"); // Get the rest ngram

    if(current_word == NULL ) return NULL;
    for(int i = 0 ; i < current->size ; i++)
    {
        if(strcmp(current->children[i]->word,current_word) == 0) { // If we find word
            if (current->children[i]->is_final == 'F') // If word is not final
            {
                if(on_going_ngram == NULL)
                {
                    on_going_ngram = malloc((strlen(current_word)+1)*sizeof(char));
                    sprintf(on_going_ngram,"%s",current_word);

                }
                else
                {
                    sprintf(on_going_ngram,"%s %s",on_going_ngram,current_word);
                }
                Search_Ngram(root,current->children[i], remaining_ngram,on_going_ngram); // Continue searching
            }
            else
            {
                if(on_going_ngram == NULL)
                {
                    on_going_ngram = malloc((strlen(current_word)+1)*sizeof(char));
                    sprintf(on_going_ngram,"%s",current_word);

                    puts(on_going_ngram);

                    Search_Ngram(root,current->children[i], remaining_ngram, on_going_ngram);



                }
                else {
                    sprintf(on_going_ngram, "%s %s", on_going_ngram, current_word);

                    puts(on_going_ngram);

                    Search_Ngram(root,current->children[i], remaining_ngram, on_going_ngram);


                }

            }
        }
        else {
            Search_Ngram(root,root,remaining_ngram,on_going_ngram);
        }
    }
}*/


void Print_Trie(Trie_Node_Ptr root)
{
    int i;
    for(i = 0 ; i < root->size ; i++ )
    {
        Print_Trie(root->children[i]);
        printf("%s ",root->children[i]->word);
    }
    printf("\n");
}
