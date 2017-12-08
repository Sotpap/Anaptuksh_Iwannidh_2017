#include "trie.h"
#include "bloom.h"
#include <time.h>
int binary_search(Trie_Node* current_node,int min,int max,char* word)
{
    int str_result;
    int mid = 0;
    while(max >= min)
    {
        mid = (max+min)/2;
        str_result = strcmp(word,current_node->children[mid].word);
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
    trie->root->string_table = NULL;
    trie->root->string_table_size = 0;
    trie->root->fere = 0;

    trie->root->size = 0;

    trie->root->is_final = 0;

    trie->root->children = malloc(SIZE*sizeof(Trie_Node));

    memset(trie->root->children,'\0',SIZE*sizeof(Trie_Node));

    trie->root->hash_table = Hash_Table_Init();

    return trie;
}

void Insert_Ngram(Trie* trie,char* ngram) {
    int local_depth = 0;
    Trie_Node *current_node = trie->root;

    char *current_word = strtok(ngram, " \n");  // Get first word of current ngram

    char *remaining_ngram = strtok(NULL, "\n");  // Get the rest ngram

    int i , position,node_size;

    if(remaining_ngram != NULL) current_node = Hash_Table_Insert(trie->root->hash_table,current_word,0);
    else current_node = Hash_Table_Insert(trie->root->hash_table,current_word,1);

    local_depth++;

    current_word = strtok(remaining_ngram, " \n"); ///Getting next word if exists.

    remaining_ngram = strtok(NULL,"\n"); /// Getting the rest of the ngram if exists.


    while (current_word != NULL) {

        position = -1;

        node_size = current_node->size;
        i = 0;
        local_depth++;

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
                    memmove(&(current_node->children[right_position+1]),
                            &(current_node->children[right_position]),
                            (node_size - right_position)*sizeof(Trie_Node));
                    break;
                }
            }
            current_node->children[right_position] = *new_node; /// Inserting new node to current's children.
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
    if(local_depth > trie->depth) trie->depth = local_depth;
}

void Search_Substream(Trie_Node* root,char* ngram, char** result,int count, short int *bloom, Index* result_array,char** on_going_ngram) {
    Trie_Node *current_node = root;

    char *save;
    char *current_word = strtok_r(ngram, " \n", &save);


    int i, position, node_size;
    while (current_word != NULL) {
        position = -1;

        node_size = current_node->size;
        i = 0;

        position = binary_search(current_node, 0, node_size - 1, current_word);
        ///Checking if current word exists in current_nodes children


        if (position == -1) /// If we haven't found the word in current_node's children
        {
            return;
        } else {
            strcat(*on_going_ngram, " ");
            strcat(*on_going_ngram, current_word);

            if (current_node->children[position].is_final) {
                if (strlen(*result) == 0) {
                    current_node->children[position].fere = count;
                    strcpy(*result, *on_going_ngram);
                    insert_bloom(bloom, *on_going_ngram);
                    Insert_Result_Array(result_array, *on_going_ngram);
                } else if (check_bloom(bloom, *on_going_ngram)) {
                    if (current_node->children[position].fere != count) {
                        current_node->children[position].fere = count;
                        strcat(*result, "|");
                        strcat(*result, *on_going_ngram);
                        Insert_Result_Array(result_array, *on_going_ngram);
                    }
                }

            }

            current_node = &current_node->children[position];

            current_word = strtok_r(NULL, " \n", &save);
        }

    }
    return;
}

int first_depth_first(char* word,int depth) {
    int i = 0;
    int len = strlen(word);
    int counter = 0;
    int char_count = 0;
    while (counter < depth) {
        if (i == len) return char_count;
        if (word[i] == ' ') {
            counter++;
        }
        char_count++;
        i++;
    }

    return char_count - 1;

}

void Search_Ngram(Trie* trie,char* ngram,int count, Index* result_array ) {
    char *cut_word = NULL;  // Used later to cut the first word of current ngram

    char *result = malloc(((strlen(ngram)) + 1) * sizeof(char));
    memset(result, '\0', (strlen(ngram) + 1) * sizeof(char));
    ///Init bloom filter.
    short int *bloom = Init_Bloom();

    int len = 0, max_depth = trie->depth + 1;
    char *token;
    Trie_Node *root_node;
    while (ngram != NULL) {

        char *cs_copy;

        char *on_going_ngram = malloc(128);
        memset(on_going_ngram, '\0', 128);

        len = first_depth_first(ngram, max_depth);
        char *check_sub = strndup(ngram, len);

        cs_copy = check_sub;

        token = strtok(check_sub, " \n");
        root_node = Hash_Table_Search(trie->root->hash_table, token);

        if (root_node != NULL) {

            if (root_node->is_final == 1) {

                if (strlen(result) == 0) {
                    root_node->fere = count;
                    strcpy(result, token);
                    insert_bloom(bloom, token);
                    Insert_Result_Array(result_array, token);
                } else if (check_bloom(bloom, token)) {
                    if (root_node->fere != count) {
                        root_node->fere = count;
                        strcat(result, "|");
                        strcat(result, token);
                        Insert_Result_Array(result_array, token);
                    }
                }
            }

            strcpy(on_going_ngram, token);
            check_sub = strtok(NULL, "\n");
            if (check_sub != NULL)
                Search_Substream(root_node, check_sub, &result, count, bloom, result_array, &on_going_ngram);

        }

        free(cs_copy);

        free(on_going_ngram);
        on_going_ngram = NULL;

        ngram = strchr(ngram, ' ');
        if (ngram) {
            ngram++;
            while (*ngram == ' ') ngram++;
        }

    }

    if (strlen(result) == 0) {
        printf("-1\n");
        free(result);
    } else {
        puts(result);
        free(result);
    }
    free(bloom);
}

int Delete_Ngram(Trie_Node* current_node,char* ngram,int hash_count) {


    char *current_word = strtok(ngram, " \n");


    char *remaining_ngram = strtok(NULL, "\n");

    if (hash_count == 0) {
        Trie_Node *pos_next_node = Hash_Table_Search(current_node->hash_table, current_word);

        if (pos_next_node != NULL) {

            if (Delete_Ngram(pos_next_node, remaining_ngram, ++hash_count)) {
                Hash_Table_Delete(current_node->hash_table, current_word);

            }

            return 1;

        } else return 0;
    }


    int position = -1;

    if (current_word == NULL) {

        if (current_node->is_final == 1) {

            current_node->is_final = 0;
            if (current_node->size == 0) {
                free(current_node->children);
                //free(current_node->word);

                return 1;
            }
            return 0;
        }
    } else {


        position = binary_search(current_node, 0, current_node->size - 1, current_word);
        if (position == -1) {
            return 0;
        } else {
            if (Delete_Ngram(&(current_node->children[position]), remaining_ngram, ++hash_count)) {

                free(current_node->children[position].word);
                memset(&(current_node->children[position]), '\0', sizeof(Trie_Node));


                if (position != current_node->size)
                    memmove(&(current_node->children[position]),
                            &(current_node->children[position + 1]),
                            (current_node->size - position) * sizeof(Trie_Node));

                current_node->size--;

                if ((current_node->size == 0) && !(current_node->is_final)) {
                    free(current_node->children);
                    //free(current_node->word);
                    return 1;
                }
                return 0;
            }
        }
    }

    return 0;
}


int Static_binary_search(Trie_Node* current_node,int min,int max,char* word) {
    int mid, str_result, len, i = 0;

    while (max >= min) {
        mid = (max + min) / 2;

        len = current_node->children[mid].string_table[0];

        if (len < 0) len = -len;

        str_result = strncmp(word, current_node->children[mid].word, len);
        if (str_result == 0) return mid;
        else if (str_result > 0) min = mid + 1;
        else max = mid - 1;
    }
    return -1;
}

void Insert_Static(Trie* trie, char* ngram) {
    Trie_Node *current_node = trie->root;
    //Get first word of current ngram
    char *current_word = strtok(ngram, " \n");
    //Get the rest ngram
    char *remaining_ngram = strtok(NULL, "\n");

    int i = 0, position, node_size, right_position, local = 0;

    while (current_word != NULL) {
        //Init of variables
        position = -1;

        node_size = current_node->size;
        local++;
        i = 0;

        position = binary_search(current_node, 0, node_size - 1, current_word);

        if (position == -1) /// If we haven't found the word in current_node's children
        {
            //Creating new node
            Trie_Node *new_node = malloc(sizeof(Trie_Node));

            new_node->word = strdup(current_word);

            new_node->string_table = calloc(STRING_TABLE_SIZE, sizeof(signed short int));


            //Assigning the right values to the final identifiers
            if (remaining_ngram == NULL) {
                new_node->is_final = 1;
                new_node->string_table[0] = strlen(current_word);

            } else {
                new_node->is_final = 0;
                new_node->string_table[0] = -(strlen(current_word));
            }

            new_node->string_table_size = 1;

            //Init of labeling and children table variables
            new_node->fere = 0;
            new_node->size = 0;

            //Allocating children array and setting value to zero.
            new_node->children = malloc(SIZE * sizeof(Trie_Node));

            memset(new_node->children, '\0', SIZE * sizeof(Trie_Node));

            ///Checking if there is aneed to allocate more space for the current node's children array
            if ((node_size % SIZE == 0) && (node_size != 0)) {
                current_node->children = realloc(current_node->children, (node_size * 2) * sizeof(Trie_Node));
            }

            ///Finding the right position to insert the new node in the current's children array
            right_position = node_size;

            for (i = 0; i < node_size; i++) {
                if (strcmp(current_node->children[i].word, current_word) > 0) {
                    right_position = i;

                    memmove(&(current_node->children[right_position + 1]),
                            &(current_node->children[right_position]),
                            (node_size - right_position) * sizeof(Trie_Node));
                    break;
                }
            }

            //Inserting new node to current's children.
            current_node->children[right_position] = *new_node;

            current_node->size += 1;

            //Passing new node as current to keep inserting the rest of the ngram
            current_node = &(current_node->children[right_position]);
        } else  ///If we found the word in current_node's children
        {
            //Checking if current word is the final of the ngram and updating if necessary
            if (remaining_ngram == NULL && current_node->children[position].is_final == 0) {
                current_node->children[position].is_final = 1;
                current_node->children[position].string_table[0] = -(current_node->children[position].string_table[0]);
            }
            //Passing new node as current to keep inserting the rest of the ngram
            current_node = &current_node->children[position];
        }
        //Getting next word if exists.
        current_word = strtok(remaining_ngram, " \n");
        //Getting the rest of the ngram if exists.
        remaining_ngram = strtok(NULL, "\n");
    }

    if(local > trie->depth) trie->depth = local;
}

void  pull( Trie_Node* top_node) {

    Trie_Node *temp = NULL;

    while (top_node->size == 1) {

        temp = top_node->children;

        top_node->fere = top_node->children[0].fere;   // coping fere
        top_node->is_final = top_node->children[0].is_final; // coping is final
        if (top_node->word == NULL) {                               // Initialising or concatenatin strings

            top_node->word = malloc((strlen(top_node->children[0].word) + 1) * sizeof(char));
            strcpy(top_node->word, top_node->children[0].word);

        } else {
            top_node->word = realloc(top_node->word,
                                     (strlen(top_node->word) + strlen(top_node->children[0].word) + 1) * sizeof(char));
            strcat(top_node->word, top_node->children[0].word);
        }

        top_node->string_table[top_node->string_table_size] = top_node->children[0].string_table[0]; // copying string table value and size
        top_node->string_table_size++;


        if (top_node->children == NULL) {                            // Won't somebody think of the children???
            top_node->children = malloc((top_node->children[0].size) *
                                        sizeof(Trie_Node));                    //sizeof(current_node->children)+sizeof(Trie_Node));

        } else {
            top_node->size = top_node->children[0].size;
            top_node->children = (top_node->children[0].children);
            free(temp[0].word);
            free(temp);
        }

    }
}

void compress(Trie_Node* current) {
    if(current) {
        int i = 0;
        for (i = 0; i < current->size; i++) {
            if (current->children[i].size == 1) {
                pull(&(current->children[i]));
                //puts(current->children[i].word);
                //for(int k = 0; k < current->children[i].string_table_size; k++) printf("%d ", current->children[i].string_table[k]);
                //printf("\n");
                if (current->children[i].size > 1) {
                    compress(&(current->children[i]));
                }
            } else if (current->children[i].size > 1) {
                compress(&(current->children[i]));
            }
        }
    }
}

void Static_Substream(Trie_Node* root,char* ngram, char** result, short int *bloom, Index* result_array,int count) {
    Trie_Node *current_node = root;                //search starting from root

    char *save;
    char *current_word = strtok_r(ngram, " \n", &save);

    char *on_going_ngram = malloc(5000);        // allocating and starting for string used in  strong current ngrams

    memset(on_going_ngram, '\0', 5000);

    char *check_word;

    char *temp;

    temp = NULL;
    int i, position, node_size, len;

    while (current_word != NULL) {
        position = -1;            // storing the position of the  word among children nodes

        node_size = current_node->size;
        i = 0;

        position = binary_search(current_node, 0, node_size - 1, current_word);
        ///Checking if current word exists in current_nodes children


        if (position == -1) /// If we haven't found the word in current_node's children
        {
            free(on_going_ngram);
            if (temp) free(temp);
            return;
        } else {
            if (strlen(on_going_ngram) == 0) {
                strcpy(on_going_ngram, current_word);
            } else {
                strcat(on_going_ngram, " ");
                strcat(on_going_ngram, current_word);
            }
            if(current_node->children[position].string_table[0] > 0) {
                printf("%p \n",result);
                if (strlen(*result) == 0) {
                    strcpy(*result, on_going_ngram);
                    insert_bloom(bloom, on_going_ngram);
                    Insert_Result_Array(result_array, on_going_ngram);
                } else if (check_bloom(bloom, on_going_ngram)) {
                    strcat(*result, "|");
                    strcat(*result, on_going_ngram);
                    Insert_Result_Array(result_array, on_going_ngram);
                }
            }

            check_word = strdup(current_node->children[position].word);

            temp = check_word;

            len = current_node->string_table[0];

            if (len < 0) len = -len;

            for (i = 1; i < current_node->string_table_size; i++) {
                current_word = strtok_r(NULL, " \n", &save);

                check_word += len;

                len = current_node->string_table[i];

                if (len < 0) len = -len;

                check_word += len;

                if (!strncmp(check_word, current_word, len)) {
                    strcat(on_going_ngram, " ");
                    strcat(on_going_ngram, current_word);
                    if (current_node->string_table[i] > 0) {
                        if (check_bloom(bloom, on_going_ngram)) {
                            strcat(*result, "|");
                            strcat(*result, on_going_ngram);
                            Insert_Result_Array(result_array, on_going_ngram);
                        }
                    }
                } else {
                    if (check_word) free(check_word);
                    free(on_going_ngram);
                    return;
                }
            }

        }
        current_word = strtok_r(NULL, " \n", &save);
    }
    free(on_going_ngram);
    if (temp) free(temp);
    return;
}

void Search_Static(Trie* trie,char* ngram,int count, Index* result_array) {

    short int *bloom = Init_Bloom();

    char *result = malloc(((strlen(ngram)) + 1) * sizeof(char)); //allocating memory for result string
    memset(result, '\0', (strlen(ngram) + 1) * sizeof(char));      //arxikopoihsh tou result
    char *check_sub = NULL;                    // string used for storing each substream before searching
    int len = 0, max_depth = trie->depth + 1;
    char *cs_copy;

    while (ngram != NULL)        //until we have search  each possible substream of ngram
    {
        len = first_depth_first(ngram, max_depth);
        char *check_sub = strndup(ngram, len);

        cs_copy = check_sub;

        Static_Substream(trie->root, check_sub, &result, bloom, result_array, count);        //search check sub in trie

        free(cs_copy);                        // free check sub string

        ngram = strchr(ngram, ' ');                    // subtracting a word from ngram
        if (ngram) ngram++;
    }

    if (strlen(result) == 0)            // if not found
    {
        printf("-1\n");
        free(result);
    } else {
        puts(result);                //else print the result
        free(result);
    }
    free(bloom);
}