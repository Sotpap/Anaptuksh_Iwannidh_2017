//
// Created by antonis on 10/11/17.
//
#include "file_handler.h"

int main(int argc,char** argv)
{
    Arguments file_list;

    if(Get_Arguments(argc,argv,&file_list) == 0) return FILE_ERROR;

    Trie* trie = Init_Trie();


    if(Extract_From_Init(file_list.init_file, trie) == 0) return FILE_ERROR;

    //if(Extract_From_Query(file_list.query_file, trie) == 0) return FILE_ERROR;

    //char* ngram = malloc((strlen("objectives readmission has a major role in the reduction of the quality of life and the increase in the years of lost life the main objectives of this study were to answer to the following research questions a what was the readmission rate b what were the social demographic and clinical characteristics of patients admitted to the psychiatric emergency service at nour university hospital affiliated to isfahan university of medical sciences isfahan iran c what were the effective factors on readmission method this cross sectional study was conducted on a total number of 3935 patients who were admitted to isfahan university hospital psychiatric ward in isfahan iran from 2004 to 2010 gender age marital status education self report history of previous admission type of psychiatric disorder substance misuse suicide and the length of the current psychiatric disorder were collected from the registered medical files of patients the data were analysed using the negative binomial regression model results we found that factors such as psychiatric anxiety disorder bipolar i bipolar ii psychotic disorder depression and self report history of previous admission were statistically significant in the number of readmissions using the negative binomial model conclusion readmission to the psychiatric ward is mainly predictable by the type of diagnosis and psychosocial supports")+1)*sizeof(char));
    //strcpy(ngram, "objectives readmission has a major role in the reduction of the quality of life and the increase in the years of lost life the main objectives of this study were to answer to the following research questions a what was the readmission rate b what were the social demographic and clinical characteristics of patients admitted to the psychiatric emergency service at nour university hospital affiliated to isfahan university of medical sciences isfahan iran c what were the effective factors on readmission method this cross sectional study was conducted on a total number of 3935 patients who were admitted to isfahan university hospital psychiatric ward in isfahan iran from 2004 to 2010 gender age marital status education self report history of previous admission type of psychiatric disorder substance misuse suicide and the length of the current psychiatric disorder were collected from the registered medical files of patients the data were analysed using the negative binomial regression model results we found that factors such as psychiatric anxiety disorder bipolar i bipolar ii psychotic disorder depression and self report history of previous admission were statistically significant in the number of readmissions using the negative binomial model conclusion readmission to the psychiatric ward is mainly predictable by the type of diagnosis and psychosocial supports");
    //Search_Ngram(trie, ngram);

    //Print_Trie(*(trie->root));
}
