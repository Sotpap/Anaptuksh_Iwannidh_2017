#include "trie.h"
#include "bloom.h"
#include <time.h>
int binary_search(Trie_Node* current_node,int min,int max,char* word)
{
    int mid;
    while(max >= min)
    {
        mid = (max+min)/2;
        int str_result = strcmp(word,current_node->children[mid].word);
        if(str_result == 0) return mid;
        else if(str_result > 0)
        {
            min = mid + 1;
        }
        else
        {
            max = mid - 1;
        }
    }
    return -1;


}

Trie* Init_Trie(void)
{
    Trie* trie = malloc(sizeof(Trie));

    trie->root = malloc(sizeof(Trie_Node));

    trie->depth = 0;

    trie->root->word = NULL;

    trie->root->fere = 0;

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
        i = 0;


        position = binary_search(current_node,0,node_size-1,current_word);

        if (position == -1) /// If we haven't found the word in current_node's children
        {

            Trie_Node* new_node = malloc(sizeof(Trie_Node));  /// Creating new node

            new_node->word = malloc((strlen(current_word)+1)*sizeof(char));
            strcpy(new_node->word, current_word);

            if(remaining_ngram == NULL) new_node->is_final = 1;
            else new_node->is_final = 0;

            new_node->fere = 0;
            new_node->size = 0;

            new_node->children = malloc(SIZE*sizeof(Trie_Node));

            memset(new_node->children,'\0',SIZE*sizeof(Trie_Node));

            if((node_size % SIZE == 0) && (node_size != 0))
            {
                current_node->children = realloc(current_node->children, (node_size*2)*sizeof(Trie_Node));
            }

            int right_position = node_size;

            for(int i = 0 ; i < node_size ; i++)
            {
                if(strcmp(current_node->children[i].word,current_word) > 0)
                {

                    right_position = i;
                    memmove(&(current_node->children[right_position+1]),&(current_node->children[right_position]),(node_size - right_position)*sizeof(Trie_Node));
                    break;
                }
            }

            current_node->children[right_position]= *new_node; /// Inserting new node to current's children.

            current_node->size += 1;
            current_node = &current_node->children[right_position]; /// Passing new node as current to keep inserting the rest of the ngram
        }
        else  ///If we found the word in current_node's children
        {
            if (remaining_ngram == NULL) current_node->children[position].is_final = 1; /// Checking if current word is the final of the ngram

            current_node = &current_node->children[position];   /// Passing new node as current to keep inserting the rest of the ngram

        }

        current_word = strtok(remaining_ngram, " \n"); ///Getting next word if exists.

        remaining_ngram = strtok(NULL,"\n"); /// Getting the rest of the ngram if exists.

    }
}

void Search_Substream(Trie_Node* root,char* ngram, char** result,int count, short int *bloom, Index* result_array)
{
    Trie_Node* current_node = root;

    char* save;
    char *current_word = strtok_r(ngram, " \n",&save);

    char *on_going_ngram = malloc(5000);
    memset(on_going_ngram,'\0',5000);

    int i , position, node_size;
    while (current_word != NULL)
    {
        position = -1;

        node_size = current_node->size;
        i = 0;

        position = binary_search(current_node,0,node_size-1,current_word);
        ///Checking if current word exists in current_nodes children


        if(position == -1) /// If we haven't found the word in current_node's children
        {
            free(on_going_ngram);
            return;
        }
        else
        {
            if(strlen(on_going_ngram) == 0)
            {
                strcpy(on_going_ngram, current_word);
            }
            else
            {
                strcat(on_going_ngram, " ");
                strcat(on_going_ngram, current_word);
            }
            if(current_node->children[position].is_final)
            {
                if(strlen(*result) == 0)
                {
                    strcpy(*result, on_going_ngram);
                    insert_bloom(bloom, on_going_ngram);
                    Insert_Result_Array(result_array,on_going_ngram);
                }
                else if (check_bloom(bloom, on_going_ngram))
                {
                    strcat(*result, "|");
                    strcat(*result, on_going_ngram);
                    insert_bloom(bloom, on_going_ngram);
                    Insert_Result_Array(result_array,on_going_ngram);
                }

            }

            current_node = &current_node->children[position];

            current_word = strtok_r(NULL," \n",&save);
        }

    }
    free(on_going_ngram);
    return ;
}


void Search_Ngram(Trie* trie,char* ngram,int count, Index* result_array )
{
    char *cut_word = NULL;  // Used later to cut the first word of current ngram

    char* result = malloc(((strlen(ngram))+1)*sizeof(char));
    memset(result,'\0',(strlen(ngram)+1)*sizeof(char));
    char* check_sub = NULL;
    ///Init bloom filter.
    short int *bloom = Init_Bloom();

    while (ngram != NULL)
    {

        check_sub = malloc((strlen(ngram)+1)*sizeof(char));
        strcpy(check_sub, ngram);
        clock_t begin = clock();
        Search_Substream(trie->root, check_sub, &result,count, bloom, result_array);


        free(check_sub);

        ngram = strchr(ngram,' ');
        if(ngram) ngram++;
    }

    if(strlen(result) == 0 )
    {
        printf("-1\n");
        free(result);
    }
    else
    {
       puts(result);
        free(result);
    }
    free(bloom);
}
void Print_Trie(Trie_Node node)
{
    for(int i = 0; i < node.size; i++)
    {
        Print_Trie(node.children[i]);
        puts(node.children[i].word);

    }
}

int Delete_Ngram(Trie_Node* current_node, char* ngram)
{
    char* current_word = strtok(ngram, " \n");

    char* remaining_ngram = strtok(NULL, "\n");

    int position = 0;

    if(current_word == NULL)
    {

        if(current_node->is_final == 1)
        {

            current_node->is_final = 0;
            if(current_node->size == 0)
            {
                free(current_node->children);
                free(current_node->word);

                return 1;
            }
            return 0;
        }
    }
    else
    {
        position = binary_search(current_node,0, current_node->size -1 ,current_word);

        if(position == -1)
        {
            return 0;
        }
        else
        {
            if(Delete_Ngram(&(current_node->children[position]),remaining_ngram))
            {
                memset(&(current_node->children[position]),'\0', sizeof(Trie_Node));


                if(position != current_node->size)
                    memmove(&(current_node->children[position]),&(current_node->children[position+1]),(current_node->size - position)* sizeof(Trie_Node));

                current_node->size--;

                if( (current_node->size == 0) && !(current_node->is_final))
                {
                    free(current_node->children);
                    free(current_node->word);
                    return 1;
                }
                return 0;
            }
        }
    }

    return 0;
}

