#include "trie.h"
int find_substring(char* result,char* on_going)
{
    char* temp = malloc((strlen(result)+1)*sizeof(char));
    strcpy(temp,result);
    char* token = strtok(temp,"|");
    while(token!=NULL)
    {
        if(strcmp(token,on_going) == 0 )
        {
            return 1;
        }
        token = strtok(NULL,"|");
    }
    free(temp);
    return 0;
}

int binarySearch(Trie_Node* node, int size, char* word,int mode)
{
    printf("Eimai sto GIT\n");
    int found = 0, mid = 0, start = 0, end = size;

    while(end > start)
    {
        mid = start + (end - start)/2;

        if (strcmp(word, node->children[mid].word) == 0)
        {
            found = 1;
            break;
        }
        else if (strcmp(word, node->children[mid].word) > 0)
        {
            start = mid + 1;
        }
        else if (strcmp(word, node->children[mid].word) < 0)
        {
            end = mid - 1;
        }
    }
    if(mode == 1)
    {
        return mid;
    }
    else if(mode == 2)
    {
        return (found == 1) ? mid : -1;
    }
}

Trie* Init_Trie(void)
{
    Trie* trie = malloc(sizeof(Trie));

    trie->root = malloc(sizeof(Trie_Node));

    trie->depth = 0;

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

    int i , position,node_size, right_position;

    while (current_word != NULL)
    {
        node_size = current_node->size;

        position = -1;

        ///Checking if current word exists in current_nodes children
        if(current_node->size > 0)
        {
            position =  binarySearch(current_node, current_node->size, current_word, 2);
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
                current_node->children = realloc(current_node->children, (node_size*2)*sizeof(Trie_Node));
            }

            right_position = node_size;

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

char* Search_Substream(Trie_Node* root,char* ngram, char* result)
{
    Trie_Node* current_node = root;

    char *current_word = strtok(ngram, " \n");

    char *remaining_ngram = strtok(NULL, "\n");

    char *on_going_ngram = NULL;

    int position, node_size;
    while (current_word != NULL)
    {
        node_size = current_node->size;

        position = -1;

        if(node_size > 0)
        {
            position =  binarySearch(current_node, node_size, current_word, 2);
        }

        if(position == -1) /// If we haven't found the word in current_node's children
        {
            if(on_going_ngram != NULL) free(on_going_ngram);
            return result;
        }
        else
        {
            if(on_going_ngram == NULL)
            {
                on_going_ngram = malloc((strlen(current_word)+1)*sizeof(char));
                strcpy(on_going_ngram, current_word);
            }
            else
            {
                on_going_ngram = realloc(on_going_ngram, (strlen(on_going_ngram)+(strlen(current_word)+2))*sizeof(char));
                strcat(on_going_ngram, " ");
                strcat(on_going_ngram, current_word);
            }
            if(current_node->children[position].is_final)
            {
                if(result == NULL)
                {
                    result = malloc((strlen(on_going_ngram) + 1) * sizeof(char));
                    strcpy(result, on_going_ngram);
                }
                else if(find_substring(result,on_going_ngram) == 0) {
                    result = realloc(result, (strlen(result) + (strlen(on_going_ngram) + 2)) * sizeof(char));
                    strcat(result, "|");
                    strcat(result, on_going_ngram);
                }

            }

            current_node = &current_node->children[position];

            current_word = strtok(remaining_ngram," \n");
            remaining_ngram = strtok(NULL,"\n");

        }

    }
    return result;
}

void Search_Ngram(Trie* trie,char* ngram) {

    char *cut_word = NULL;  // Used later to cut the first word of current ngram

    char *remaining_ngram = strtok(ngram, "\n");  // Get the rest ngram

    char* result = NULL;

    int depth = trie->depth, i = 0;
    size_t size = 0;
    char* check_sub = NULL;

    while (remaining_ngram != NULL)
    {
        for(i = 0; i < strlen(remaining_ngram); i++)
        {
            size++;
            if(isspace(remaining_ngram[i])) depth--;
            if(depth == 0) break;
        }

        check_sub = strndup(remaining_ngram, size);

        result = Search_Substream(trie->root, check_sub, result);




        free(check_sub);

        depth = trie->depth;
        size = 0;
        cut_word = strtok(remaining_ngram," \n");
        remaining_ngram = strtok(NULL,"\n");
    }

    if(strlen(result) == 0) printf("-1\n");
    else puts(result);
    free(result);
}

void Print_Trie(Trie_Node node)
{
    for(int i = 0; i < node.size; i++)
    {
        Print_Trie(node.children[i]);
        puts(node.children[i].word);

    }
}