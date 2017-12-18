//
// Created by tentas on 11/16/17.
//

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "bloom.h"

//int bloom_size = 690037;
int bloom_size = 70951;

short int* Init_Bloom()
{
    short int* bloom = malloc(bloom_size*sizeof(short int));
    int i = 0;
    for(i = 0;i < bloom_size;i++)
    {
        bloom[i] = 0;
    }
    return bloom;
}

static size_t djb_hash(const char* cp)
{
    size_t hash = 5381;
    while (*cp)
        hash = 33 * hash ^ (unsigned char) *cp++;
    return hash;
}

static size_t fnv1a_hash(const char* cp)
{
    size_t hash = 0x811c9dc5;
    while (*cp) {
        hash ^= (unsigned char) *cp++;
        hash *= 0x01000193;
    }
    return hash;
}

unsigned int hash_bloom (const char* word)
{
    unsigned int hash = 0;
    for (int i = 0 ; word[i] != '\0' ; i++)
    {
        hash = 31*hash + word[i];
    }
    return (hash>=0) ? hash : (-hash);
}


unsigned int hash_bloom_1(char* s)
{
    int i;
    for( i=0; *s; s++ ) i = 131*i + *s;
    return (i>=0) ? i : (-i);
}

static unsigned long sdbm_hash(char *str)
{
    unsigned long hash = 0;
    int c;

    while (c = *str++)
        hash = c + (hash << 6) + (hash << 16) - hash;

    return hash;
}

unsigned long long hash_fun_1(char *n_gram)
{
    int i = 0, len = strlen(n_gram);
    long long int hash;

    for(i = 0; i < len; i++)
    {
        hash = 15*hash + n_gram[i];
    }

    return (hash>=0) ? hash : (-hash);
}

unsigned long long hash_fun_2(char *n_gram)
{
    int i = 0, len = strlen(n_gram);
    long long int hash;

    for(i = 0; i < len; i++)
    {
        hash = 46*hash + n_gram[i];
    }

    return (hash>=0) ? hash : (-hash);
}

unsigned long long hash_fun_3(char *n_gram)
{
    int i = 0, len = strlen(n_gram);
    long long int hash;

    for(i = 0; i < len; i++)
    {
        hash = 69*hash + n_gram[i];
    }

    return (hash>=0) ? hash : (-hash);
}

unsigned long long hash_fun_4(char *n_gram)
{
    int i = 0, len = strlen(n_gram);
    long long int hash;

    for(i = 0; i < len; i++)
    {
        hash = 45*hash + n_gram[i];
    }

    return (hash>=0) ? hash : (-hash);
}

unsigned long long hash_fun_5(char *n_gram)
{
    int i = 0, len = strlen(n_gram);
    long long int hash;

    for(i = 0; i < len; i++)
    {
        hash = 36*hash + n_gram[i];
    }

    return (hash>=0) ? hash : (-hash);
}

/*
int check_cross_bloom(short int *cross_bloom, char *n_gram)
{
    long long int hash_1 = 0,hash_2 = 0,hash_3 = 0,hash_4 = 0,hash_5 = 0;

    hash_1 = (hash_fun_1(n_gram))%bloom_size;

    hash_2 = (hash_fun_2(n_gram))%bloom_size;

    hash_3 = (hash_fun_3(n_gram))%bloom_size;

    hash_4 = (hash_fun_4(n_gram))%bloom_size;

    hash_5 = (hash_fun_5(n_gram))%bloom_size;

    if(cross_bloom[hash_1] &&
       cross_bloom[hash_2] &&
       cross_bloom[hash_3] &&
       cross_bloom[hash_4] &&
       cross_bloom[hash_5]) return 0;
    else
    {
        cross_bloom[hash_1] = 1;
        cross_bloom[hash_2] = 1;
        cross_bloom[hash_3] = 1;
        cross_bloom[hash_4] = 1;
        cross_bloom[hash_5] = 1;
        return 1;
    }
}

*/
int check_bloom(short int *bloom, char* n_gram)
{
    size_t djb, fn, sdbm;
    unsigned int simple, simple1;

    djb = (djb_hash(n_gram))%bloom_size;

    fn = (fnv1a_hash(n_gram))%bloom_size;

    simple = (hash_bloom(n_gram))%bloom_size;

    simple1 = (hash_bloom_1(n_gram))%bloom_size;

    sdbm = (sdbm_hash(n_gram))%bloom_size;

    if(bloom[djb] &&
       bloom[fn] &&
       bloom[simple] &&
       bloom[simple1] &&
       bloom[sdbm]) return 0;
    else
    {
        bloom[djb] = 1;
        bloom[fn] = 1;
        bloom[simple] = 1;
        bloom[simple1] = 1;
        bloom[sdbm] = 1;
        return 1; //check_cross_bloom(cross_bloom,n_gram);
    }
}

void insert_bloom(short int *bloom, char* n_gram)
{
    size_t djb, fn, sdbm;
    unsigned int simple, simple1;
    //long long int hash_1 = 0,hash_2 = 0,hash_3 = 0,hash_4 = 0,hash_5 = 0;

    djb = (djb_hash(n_gram))%bloom_size;

    fn = (fnv1a_hash(n_gram))%bloom_size;

    simple = (hash_bloom(n_gram))%bloom_size;

    simple1 = (hash_bloom_1(n_gram))%bloom_size;

    sdbm = (sdbm_hash(n_gram))%bloom_size;

/*    hash_1 = (hash_fun_1(n_gram))%bloom_size;

    hash_2 = (hash_fun_2(n_gram))%bloom_size;

    hash_3 = (hash_fun_3(n_gram))%bloom_size;

    hash_4 = (hash_fun_4(n_gram))%bloom_size;

    hash_5 = (hash_fun_5(n_gram))%bloom_size;
*/
    bloom[djb] = 1;
    bloom[fn] = 1;
    bloom[simple] = 1;
    bloom[simple1] = 1;
    bloom[sdbm] = 1;
/*
    cross_bloom[hash_1] = 1;
    cross_bloom[hash_2] = 1;
    cross_bloom[hash_3] = 1;
    cross_bloom[hash_4] = 1;
    cross_bloom[hash_5] = 1;
*/
}


