//
// Created by tentas on 11/16/17.
//

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

short int* Init_Bloom()
{
    short int* bloom = malloc(7919*sizeof(short int));
    int i = 0;
    for(i = 0;i < 7919;i++)
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

unsigned int hash (const char* word)
{
    unsigned int hash = 0;
    for (int i = 0 ; word[i] != '\0' ; i++)
    {
        hash = 31*hash + word[i];
    }
    return (hash>=0) ? hash : (-hash);
}


unsigned int hash_1(char* s)
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

int check_bloom(short int *bloom, char* n_gram)
{
    size_t djb, fn, sdbm;
    unsigned int simple, simple1;

    djb = djb_hash(n_gram);

    fn = fnv1a_hash(n_gram);

    simple = hash(n_gram);

    simple1 = hash_1(n_gram);

    sdbm = sdbm_hash(n_gram);

    if(bloom[djb%7919] && bloom[fn%7919] && bloom[simple%7919] && bloom[simple1%7919] && bloom[sdbm%7919]) return 0;
    else return 1;
}

void insert_bloom(short int *bloom, char* n_gram)
{
    size_t djb, fn, sdbm;
    unsigned int simple, simple1;

    djb = djb_hash(n_gram);

    fn = fnv1a_hash(n_gram);

    simple = hash(n_gram);

    simple1 = hash_1(n_gram);

    sdbm = sdbm_hash(n_gram);

    bloom[djb%7919] = 1;
    bloom[fn%7919] = 1;
    bloom[simple%7919] = 1;
    bloom[simple1%7919] = 1;
    bloom[sdbm%7919] = 1;

}


