#include "trie.h"
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
   	trie->root->string_table[0] = 0;
   	trie->root->string_table_size = 0;
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
            
	    if(new_node->is_final) new_node->string_table[0] = strlen(new_node->word);
	    else new_node->string_table[0] = - strlen(new_node->word);
            new_node->fere = 0;
            new_node->size = 0;
            new_node->string_table_size = 1;

            new_node->children = malloc(SIZE*sizeof(Trie_Node));

            memset(new_node->children,'\0',SIZE*sizeof(Trie_Node));

            if((node_size % SIZE == 0) && (node_size != 0))
            {
                current_node->children = realloc(current_node->children, (node_size*2)*sizeof(Trie_Node));
            }

            int right_position = node_size;
		int i;
            for( i = 0 ; i < node_size ; i++)
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

void Search_Substream(Trie_Node* root,char* ngram, char** result,int count)
{
    Trie_Node* current_node = root;				//search starting from root

    char* save;
    char *current_word = strtok_r(ngram, " \n",&save);

    char *on_going_ngram = malloc(5000);		// allocating and starting for string used in  strong current ngrams

    memset(on_going_ngram,'\0',5000);

    int i , position, node_size;
    while (current_word != NULL)
    {
        position = -1;			// storing the position of the  word among children nodes

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
                    current_node->children[position].fere = count;

                }
                else if( current_node->children[position].fere != count )
                {
                    current_node->children[position].fere = count;
                    strcat(*result, "|");
                    strcat(*result, on_going_ngram);
                }

            }

            current_node = &current_node->children[position];

            current_word = strtok_r(NULL," \n",&save);
        }

    }
    free(on_going_ngram);
    return ;
}


void Search_Ngram(Trie* trie,char* ngram,int count)
{
    char *cut_word = NULL;  // Used later to cut the first word of current ngram

    char* result = malloc(((strlen(ngram))+1)*sizeof(char)); //allocating memory for result string
    memset(result,'\0',(strlen(ngram)+1)*sizeof(char));      //arxikopoihsh tou result
    char* check_sub = NULL;					// string used for storing each substream before searching

    while (ngram != NULL)		//until we have search  each possible substream of ngram
    {

        check_sub = malloc((strlen(ngram)+1)*sizeof(char));		// allocating memory for check sub string
        strcpy(check_sub, ngram);					// pushing ngram to check sub
        clock_t begin = clock();					//starting the clock
        Search_Substream(trie->root, check_sub, &result,count);		//search check sub in trie


        free(check_sub);						// free check sub string
	
        ngram = strchr(ngram,' ');					// subtracting a word from ngram 
        if(ngram) ngram++;						
    }

    if(strlen(result) == 0 )			// if not found
    {
        printf("-1\n");
        free(result);
    }
    else
    {
       puts(result);				//else print the result 
        free(result);
    }
}
void Print_Trie(Trie_Node node)
{



	int i;
    for( i = 0; i < node.size; i++)
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

///////////////////STATIC PART/////////////////////////////////

/*
void add_to_hyper_node(Trie_Node* current_node ,Trie_Node* hyper_node){
	
	
	hyper_node->fere = current_node->fere;   // coping fere
	hyper_node->is_final = current_node->is_final; // coping is final
	printf("1\n");
	if(hyper_node->word == NULL){						       // Initialising or concatenatin strings

		hyper_node->word = malloc((strlen(current_node->word)+1)*sizeof(char));
		strcpy(hyper_node->word,current_node->word);
	
	}
	else{
		hyper_node->word = realloc(hyper_node->word,(strlen(hyper_node->word)+ strlen(current_node->word)+1)*sizeof(char));
		strcat(hyper_node->word,current_node->word);
	}
	
	hyper_node->string_table[hyper_node->string_table_size] = current_node->string_table[0]; // copying string table value and size
	hyper_node->string_table_size++;


	if(hyper_node->children == NULL){							// Won't somebody think of the children???
		hyper_node->children = malloc((current_node->size)*sizeof(Trie_Node));					//sizeof(current_node->children)+sizeof(Trie_Node));
		
	}
	else{
		hyper_node->children = realloc(hyper_node->children,(current_node->size)*sizeof(Trie_Node)); //sizeof(current_node->children)+sizeof(Trie_Node));
		
	}
	memmove(hyper_node->children, current_node->children,current_node->size*sizeof(Trie_Node));
	hyper_node->size = current_node->size;		
	

	free(current_node->word);				///Free current node
	//free(current_node);

}
 

void pull( Trie_Node* top_node, Trie_Node* hyper_node){

	
	
	if(top_node->size == 1) add_to_hyper_node(top_node, hyper_node);

	while(top_node != NULL && top_node->size == 1){

		printf("eimai sto pull\n");
		add_to_hyper_node( &(top_node->children[0]), hyper_node);
		top_node  = &(top_node->children[0]);


	}


	
} 
*/

void  pull( Trie_Node* top_node){

			Trie_Node* temp = NULL;
			
			while(top_node->size == 1){

						temp = top_node->children;

						top_node->fere = top_node->children[0].fere;   // coping fere
						top_node->is_final = top_node->children[0].is_final; // coping is final
						if(top_node->word == NULL){						       // Initialising or concatenatin strings

							top_node->word = malloc((strlen(top_node->children[0].word)+1)*sizeof(char));
							strcpy(top_node->word,top_node->children[0].word);
	
						}
						else{
							top_node->word = realloc(top_node->word,(strlen(top_node->word)+ strlen(top_node->children[0].word)+1)*sizeof(char));
							strcat(top_node->word,top_node->children[0].word);
						}

						top_node->string_table[top_node->string_table_size] = top_node->children[0].string_table[0]; // copying string table value and size
						top_node->string_table_size++;						


						if(top_node->children == NULL){							// Won't somebody think of the children???
							top_node->children = malloc((top_node->children[0].size)*sizeof(Trie_Node));					//sizeof(current_node->children)+sizeof(Trie_Node));
		
						}
						else{
							top_node->size = top_node->children[0].size;
							top_node->children = (top_node->children[0].children);
							free(temp[0].word);
							free(temp);	
						}
							
					//	free(current_node->word);				///Free current node
							//free(current_node);
			}


}
void push_stack(Trie_Node* stack[20], int children_poutsia[20], int trav_table_size,Trie_Node* node){ //pushing a node up primary and secondary stack 

		stack[trav_table_size] = node;
		
		children_poutsia[trav_table_size] = 1;
		trav_table_size = trav_table_size + 1;
		printf("trouth is %d\n",(node == stack[trav_table_size]));
		printf("table size is %d \n", trav_table_size);
		return;


}


void pop_stack(Trie_Node* stack[20], int children_poutsia[20], int trav_table_size){      //popping the last node of the stack

		stack[trav_table_size - 1] = NULL;
		children_poutsia[trav_table_size-1] = 0;
		trav_table_size--;

}



void compress(Trie* trie){

	Trie_Node* prev;
	Trie_Node* current;
	Trie_Node* traversal_table[20] ;
	current = trie->root;
	int root_size  = 1, children_poutsia[20];
	int trav_table_size = 0;
	long int i = 0;

	while(i < 20){                      // intitialising queue arrays 
		traversal_table[i] = NULL;
		children_poutsia[i] = 0; 
		i++;
	}
	push_stack(traversal_table ,children_poutsia ,trav_table_size,current);
	printf("trouth is %d\n",(trie->root == traversal_table[trav_table_size]));
	printf("table size is %d \n", trav_table_size);
	trav_table_size = 1;
	printf("table size is %d \n", trav_table_size);
	current = &(current->children[0]);    

	while(root_size < trie->root->size){
		//printf("Rooty boi is %d\n" , root_size);
			if(current->size == 1){			//if there is only one child we have to pull
				pull(current);
				

			}
			else if(current->size > 1){		// if there are two or more children 
						
					if( traversal_table[trav_table_size-1] != current){ // traversing node for the first time
							
						
						push_stack(traversal_table ,children_poutsia ,trav_table_size,current);  //pushing stacks
						current = &(current->children[0]);                                           // traversing first child
							
					}
					else{							//traversing node for a second time or more
							
							
							if(children_poutsia[trav_table_size-1] <traversal_table[trav_table_size-1]->size){ // if there are more children to check    
							
	                    				
	                					current = &(current->children[children_poutsia[trav_table_size - 1]]); // proceed to next child
								children_poutsia[trav_table_size-1]++;
			                      			
	              					} 
		      					else {                               // if we have checked each and every child
							
								pop_stack(traversal_table ,children_poutsia , trav_table_size);  //popping the stack
								current = traversal_table[trav_table_size-1];

							}

					}
			}
			else{  //leaf case 
					
					
					current = traversal_table[trav_table_size-1]; //back to the parent 
					//printf("trouth is %d\n",(current == &(trie->root->children[0])));
					//printf(	"trouth is %d\n",(current == trie->root));
			}

			if(current == trie->root){root_size++;} 

	}



}	


