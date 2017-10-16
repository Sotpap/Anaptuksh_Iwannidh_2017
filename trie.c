#include "trie.h"

Trie_Ptr Init_Trie() {
    Trie_Ptr trie;


    trie = malloc(sizeof(Trie));

    trie->root = malloc(sizeof(Trie_Node)); /*Allocate space for root */

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

int Insert_Ngram(Trie_Node_Ptr root,char* ngram)
{
    char * current_word = strtok(ngram, " \n"); // Get first word of current ngram

    char * remaining_ngram = strtok(NULL, "\n"); // Get the rest ngram

    if(current_word == NULL ) return 1;

    Trie_Node_Ptr prefix_node;
    //check if current ngram exists in children array//
    int found = 0;
    for(int i = 0 ; i < root->size ; i++)
    {
        if(strcmp(root->children[i]->word,current_word) == 0)
        {
            prefix_node = root->children[i]; /*Store the location of node*/
            found = 1;
            break;
        }
    }
    if(found == 0) /*If we have not found word,we add it on trie*/
    {
        Trie_Node_Ptr new_node;
        if (remaining_ngram == NULL) /*If we handle the last word of ngram*/
        {
            new_node = New_Node(current_word, 'T');
           // printf("evala thn leksh %s\n",current_word);

        }
        else
        {
            new_node = New_Node(current_word, 'F');
            //printf("evala thn leksh %s\n",current_word);
        }
        if (root->size == 0) /*If we have no children,we allocate children array and store the location of node*/
        {
            root->children = malloc(1 * sizeof(Trie_Node_Ptr*));

            root->children[0] = new_node;

            (root->size)++;
        }
        else /*If we have children*/
        {
            int new_size = root->size;
            if (root->size % 2 == 0 || root->size == 1) /*If we have to re-allocate our array*/
            {
                new_size *= 2;
                root->children = realloc(root->children, new_size * sizeof(Trie_Node_Ptr *));
            }

            //Na diavazetai!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
///////////////////////////////////////
            int j=-1;
            for(int i = 0 ; i < root->size ; i++) {
                if ((strcmp(current_word, root->children[i]->word) < 0)) {
                    j = i;


                    break;
                }
            }
            //while (strcmp(current_word,root->children[j]->word)>0) j++;
            if(j!=-1) {
                for (int k = root->size - 1; k >= j; k--) {
                    root->children[k + 1] = root->children[k];
                }
                root->children[j] = new_node;

            }
            else
            {
                root->children[root->size] = new_node;

            }

////////////////////////////////////////////
            (root->size)++;
        }
      	return  Insert_Ngram(new_node,remaining_ngram); /*Recursive call for remaining ngram*/

    }
    else /* If prefix was found*/
    {
	    if(remaining_ngram == NULL) /*If we handle the last word of ngram*/
	    {
		    prefix_node->is_final = 'T';
		    return 1;
	    }
	    else
	    {
        	return Insert_Ngram(prefix_node,remaining_ngram); /*Recursive call for remaining ngram*/
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
