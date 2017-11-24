//
// Created by tentas on 11/16/17.
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "top.h"


Index* Init_Result_Array()
{
    Index* result_array = malloc(sizeof(Index));
    result_array->size = 0;

    result_array->index = malloc(100*sizeof(Cell));
    int i = 0;

    for(i = 0; i < 100; i ++){
        result_array->index[i].word = NULL;
        result_array->index[i].counter = 0;
    }

    return result_array;
}

void Free_Result_Array(Index* result_array)
{
    int i = 0;
    for(i = 0; i < result_array->size; i++)
    {
        free(result_array->index[i].word);
    }

    free(result_array->index);
    free(result_array);
}

void Insert_Result_Array(Index *result_array,char* n_gram)
{
    int i = 0, found = 0, move = 0;

    while(i < result_array->size)
    {
        if(!strcmp(result_array->index[i].word,n_gram))
        {
            result_array->index[i].counter++;
            found = 1;
            break;
        }
        if (strcmp(result_array->index[i].word,n_gram) > 0)
        {
            move = 1;
            break;
        }
        i++;
    }

    if(found == 0)
    {
        if(((result_array->size)%100 == 0 ) && (result_array->size!=0))
        {
            result_array->index = realloc(result_array->index, (2*(result_array->size))* sizeof(Cell));
        }
        if(move)
        {
            memmove((&result_array->index[i+1]),(&result_array->index[i]),(result_array->size-i)* sizeof(Cell));
        }
        result_array->index[i].word = strdup(n_gram);
        result_array->index[i].counter = 1;
        (result_array->size)++;
    }
}

void Print_Top_K(Index* result_array, int k)
{
    int top[k], i = 0, j = 0;
    Cell *temp = NULL;
    temp = malloc(sizeof(Cell));
    for (i = 0; ((i < k) && (i < result_array->size)); i++)
    {
        for (j = 0; ((j < k - i) && (i < result_array->size)); j++)
        {
            if (result_array->index[j].counter < result_array->index[j + 1].counter)
            {

                memcpy(&temp, (&result_array->index[j]), sizeof(Cell));
                memcpy((&result_array->index[j]), (&result_array->index[j+1]), sizeof(Cell));
                memcpy((&result_array->index[j + 1]), &temp, sizeof(Cell));
            }
        }
    }

    i = 0;
    printf("Top: ");
    while((i < k) && (i < result_array->size))
    {
        printf("%s",result_array->index[i].word);
        if(i != (result_array->size - 1)) printf("|");
        i++;
    }
    printf("\n");
}