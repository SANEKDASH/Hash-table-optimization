#ifndef HASH_TABLE_HEADER
#define HASH_TABLE_HEADER

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include "FastList/list.h"
#include "debug/debug.h"

#include "Text-parsing-functions/text_parse.h"

typedef unsigned int HashTableState_t;

typedef enum
{
    kHashTableClear     = 0,

    kHashTableDestroyed = 1 << 0,
    kNullHashFunc       = 1 << 1,
    kNullListArray      = 1 << 2,
    kInvalidListCount   = 1 << 3,

    kOneOfListsDamaged  = 1 << 4,
} HashTableStateCode_t;

static const char *kHashTableErrorMessages[] =
{
    "Table destoryed",
    "Null hash func",
    "Null list array",
    "Invalid list count",
    "One of lists is damaged",
};

static const size_t kErrorMessagesCount = sizeof(kHashTableErrorMessages) / sizeof(char *);

typedef enum
{
    kHashSuccess,
    kListConstructorErr,
    kCantFindHashTableElem,
    kHashDumpAlreadyStarted,
    kHashDumpWasntStarted,
    kInvalidHashTableSize,
} HashTableErrs_t;

struct HashTablePos
{
    size_t hash_table_pos;
    size_t list_pos;
};

struct HashTable
{
    List   *list_array;
    size_t  list_count;

    HashTableState_t state;

    uint64_t (*HashFunc)(ListElemType_t);
};

static const size_t kHashTableSize = 128;

HashTableErrs_t HashTableAddData(HashTable      *hash_table,
                                 ListElemType_t  data);

HashTableErrs_t HashTableDeleteElem(HashTable      *hash_table,
                                    ListElemType_t  key_val);

HashTableErrs_t HashTableInit(HashTable *hash_table,
                              size_t     hash_table_size);

HashTableErrs_t HashTableDtor(HashTable *hash_table);

HashTableErrs_t DumpHashTableBegin();

HashTableErrs_t DumpHashTableEnd();

void PrintHashTableLoadedData(HashTable  *hash_table,
                              const char *file_name);

HashTableErrs_t TestHashTable(HashTable *hash_table,
                              WordSet   *seek_word_set);

HashTableErrs_t LoadWordsInHashTable(HashTable *hash_table,
                                     WordSet   *word_set);

 uint64_t CRC32Hash    (ListElemType_t data);

#endif
