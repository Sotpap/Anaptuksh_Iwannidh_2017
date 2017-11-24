//
// Created by tentas on 11/16/17.
//

#ifndef ANAPTUKSH_IWANNIDH_2017_TOP_H
#define ANAPTUKSH_IWANNIDH_2017_TOP_H

typedef struct Cell
{
    char* word;
    int counter;
} Cell;

typedef struct Index
{
    Cell *index;
    int size;
} Index;

Index* Init_Result_Array();
void Free_Result_Array(Index *result_array);
void Insert_Result_Array(Index *result_array,char* n_gram);
void Print_Top_K(Index* result_array, int k);

#endif //ANAPTUKSH_IWANNIDH_2017_TOP_H