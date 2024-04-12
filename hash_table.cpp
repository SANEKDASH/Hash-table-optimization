#include <immintrin.h>

#include "hash_table.h"
#include "ListDump/list_dump.h"

extern "C" uint64_t asm_CRC32Hash(ListElemType_t data);

//================================================================================================

static FILE *dump_file = nullptr;

static const char *kDumpFileName = "hash_table_dump.html";

static uint64_t StupidHash   (ListElemType_t data);
static uint64_t ZeroHash     (ListElemType_t data);
static uint64_t StrlenHash   (ListElemType_t data);
static uint64_t SumHash      (ListElemType_t data);
static uint64_t SumStrlenHash(ListElemType_t data);
static uint64_t RorHash      (ListElemType_t data);
static uint64_t RolHash      (ListElemType_t data);

static void DumpHashTable(HashTable  *hash_table);

static HashTableState_t HashTableVerify(HashTable *hash_table);

static HashTableStateCode_t VerifyHashTableLists(HashTable *hash_table);

//================================================================================================

HashTableErrs_t HashTableAddData(HashTable      *hash_table,
                                 ListElemType_t  data)
{
    CHECK(hash_table);

    HASH_TABLE_VERIFY(hash_table);

    DUMP_HASH_TABLE(hash_table);

    size_t hash_pos = hash_table->HashFunc(data) % hash_table->list_count;

    size_t list_pos = 0;

    if (ListFind(hash_table->list_array + hash_pos, data, &list_pos) == kCantFind)
    {
        ListAddAfter(&hash_table->list_array[hash_pos],
                      hash_table->list_array[hash_pos].tail,
                      data);
    }

    return kHashSuccess;
}

//================================================================================================

HashTableErrs_t HashTableDeleteElem(HashTable      *hash_table,
                                    ListElemType_t  key_val)
{
    CHECK(hash_table);

    HASH_TABLE_VERIFY(hash_table);

    size_t hash_pos = hash_table->HashFunc(key_val) % hash_table->list_count;

    size_t list_pos = 0;

    if (ListFind(hash_table->list_array + hash_pos, key_val, &list_pos) != kListClear)
    {
        return kCantFindHashTableElem;
    }

    ListDelete(hash_table->list_array + hash_pos, list_pos);

    return kHashSuccess;
}

//================================================================================================

HashTableErrs_t HashTableInit(HashTable *hash_table,
                              size_t     hash_table_size)
{
    CHECK(hash_table);

    if (hash_table_size == 0)
    {
        return kInvalidHashTableSize;
    }

    hash_table->list_count = hash_table_size;

    #ifdef ASM_CRC32

        hash_table->HashFunc = asm_CRC32Hash;

    #else

        hash_table->HashFunc = CRC32Hash;

    #endif

    hash_table->list_array = (List *) calloc(hash_table->list_count, sizeof(List));

    for (size_t i = 0; i < hash_table->list_count; i++)
    {
        ListState_t constructor_result = ListConstructor(&hash_table->list_array[i]);

        if (constructor_result != kListClear)
        {
            SET_HASH_STATE(constructor_result);

            return kListConstructorErr;
        }
    }

    HASH_TABLE_VERIFY(hash_table);

    DUMP_HASH_TABLE(hash_table);

    return kHashSuccess;
}

//================================================================================================

HashTableErrs_t HashTableDtor(HashTable *hash_table)
{
    CHECK(hash_table);

    HASH_TABLE_VERIFY(hash_table);

    for (size_t i = 0; i < hash_table->list_count; i++)
    {
        ListDestructor(hash_table->list_array + i);
    }

    free(hash_table->list_array);

    hash_table->list_array = nullptr;

    hash_table->state = kHashTableDestroyed;

    return kHashSuccess;
}

//================================================================================================

HashTableErrs_t HashTableFindElem(HashTable      *hash_table,
                                  ListElemType_t  key_val,
                                  HashTablePos   *position_struct)
{
    CHECK(hash_table);

    HASH_TABLE_VERIFY(hash_table);

    size_t list_pos = 0;

    size_t hash_pos = hash_table->HashFunc(key_val) % hash_table->list_count;

    if (ListFind(hash_table->list_array + hash_pos, key_val, &list_pos) != kListClear)
    {
        return kCantFindHashTableElem;
    }

    position_struct->list_pos = list_pos;

    position_struct->hash_table_pos = hash_pos;

    return kHashSuccess;
}

//================================================================================================

static uint64_t StupidHash(ListElemType_t data)
{
    CHECK(data);

    return *data;
}
//================================================================================================

static uint64_t ZeroHash(ListElemType_t data)
{
    return 0;
}

//================================================================================================

static uint64_t StrlenHash(ListElemType_t data)
{
    CHECK(data);

    return strlen(data);
}

//================================================================================================

static uint64_t SumHash(ListElemType_t data)
{
    CHECK(data);

    uint64_t sum = 0;

    for (size_t i = 0; data[i] != '\0'; i++)
    {
        sum += data[i];
    }

    return sum;
}

//================================================================================================

static uint64_t SumStrlenHash(ListElemType_t data)
{
    CHECK(data);

    uint64_t sum = SumHash(data);

    uint64_t word_len = strlen(data);

    if (word_len == 0)
    {
        return 0;
    }

    return (uint64_t) (sum / word_len);
}

//================================================================================================

static uint64_t RorHash(ListElemType_t data)
{
    CHECK(data);

    uint64_t hash_val = 0;

    for (size_t i = 0; data[i] != '\0'; i++)
    {
        hash_val = ((hash_val >> 1) | ((hash_val & (~((~0) << 1))) << 63)) ^ data[i];
    }

    return hash_val;
}

//================================================================================================

static uint64_t RolHash(ListElemType_t data)
{
    CHECK(data);

    uint64_t hash_val = 0;

    for (size_t i = 0; data[i] != '\0'; i++)
    {
        hash_val = ((hash_val << 1) | ((hash_val & (~((~0) >> 1))) >> 63)) ^ data[i];
    }

    return hash_val;
}
//================================================================================================

uint64_t CRC32Hash(ListElemType_t data)
{
    uint32_t mask = 0;

    uint32_t hash_val = 0xFFFFFFFF;

    for (size_t i = 0; data[i] != 0; i++)
    {
        hash_val = hash_val ^ ((uint32_t) data[i]);

        for (size_t j = 0; j < 8; j++)
        {
            mask = -(hash_val & 1);

            hash_val = (hash_val >> 1) ^ (0xEDB88320 & mask);
        }
    }

    return (uint64_t) ~hash_val;
}
//================================================================================================

static HashTableState_t HashTableVerify(HashTable *hash_table)
{
    CHECK(hash_table);

    if (hash_table->HashFunc == nullptr)
    {
        hash_table->state |= kNullHashFunc;
    }

    if (hash_table->list_array == nullptr)
    {
        hash_table->state |= kNullListArray;
    }

    if (hash_table->list_count <= 0)
    {
        hash_table->state |= kInvalidListCount;
    }

    if (hash_table->state == kHashTableClear)
    {
        HashTableStateCode_t status = VerifyHashTableLists(hash_table);

        SET_HASH_STATE(status);
    }

    return hash_table->state;
}

//================================================================================================

static HashTableStateCode_t VerifyHashTableLists(HashTable *hash_table)
{
    for (size_t i = 0; i < hash_table->list_count; i++)
    {
        if (ListVerify(hash_table->list_array + i) != kListClear)
        {
            return kOneOfListsDamaged;
        }
    }

    return kHashTableClear;
}

//================================================================================================
#define LOG_PRINT(...) fprintf(dump_file, __VA_ARGS__)

HashTableErrs_t DumpHashTableBegin()
{
    if (dump_file != nullptr)
    {
        ColorPrintf(kRed, ">> %s(): dump is alredy started\n", __func__);

        return kHashDumpAlreadyStarted;
    }

    dump_file = fopen(kDumpFileName, "w");

    BeginListGraphDump();

    return kHashSuccess;
}

//================================================================================================

HashTableErrs_t DumpHashTableEnd()
{
    if (dump_file == nullptr)
    {
        ColorPrintf(kRed, ">> %s(): Can't end dump, because it wasn't opened\n"
                          "         Use func DumpHashTableBegin()", __func__);

        return kHashDumpWasntStarted;
    }

    fclose(dump_file);

    dump_file == nullptr;

    EndListGraphDump();

    return kHashSuccess;
}

//================================================================================================

#define GET_STATE_BIT(pos) hash_table->state & ((~(~0 << 1)) << pos)

static void DumpHashTable(HashTable  *hash_table)
{
    LOG_PRINT("-----------------------------------------------------------------------\n"
              "\tlist array pointer - [0x%p]\n\n"
              "\tlist count         - %ld\n\n"
              "\tstate              - %d\n", hash_table->list_array,
                                             hash_table->list_count,
                                             hash_table->state);

    for (size_t i = 0; i < kErrorMessagesCount; i++)
    {
        if (GET_STATE_BIT(i))
        {
            LOG_PRINT("\t\t%s\n",kHashTableErrorMessages[i]);
        }
    }

    LOG_PRINT("\n\tLists state:\n");

    for (size_t i = 0; i < hash_table->list_count; i++)
    {
        LOG_PRINT("\t\tlist [%ld] state:      %d\n"
                  "\t\t           elem_count: %d\n\n",i, hash_table->list_array[i].status,
                                                         hash_table->list_array[i].elem_count);
    }
}

#undef LOG_PRINT

//================================================================================================

void PrintHashTableLoadedData(HashTable  *hash_table,
                              const char *file_name)
{
    FILE *data_file = fopen(file_name, "w");

    fprintf(data_file, "ячейка\tкол-во объектов\n");

    for (size_t i = 0; i < hash_table->list_count; i++)
    {
        fprintf(data_file, "%ld\t%ld\n",i, hash_table->list_array[i].elem_count);
    }

    fclose(data_file);
}
//================================================================================================

HashTableErrs_t LoadWordsInHashTable(HashTable *hash_table,
                                     WordSet   *word_set)
{
    CHECK(hash_table);
    CHECK(word_set);


    for (size_t i = 0; i < word_set->word_count; i++)
    {
        HashTableAddData(hash_table, word_set->word_array[i].str);
    }

    return kHashSuccess;
}

static const size_t kTestCount = 2560;

//================================================================================================

HashTableErrs_t TestHashTable(HashTable *hash_table,
                              WordSet   *seek_word_set)
{
    CHECK(hash_table);
    CHECK(seek_word_set);

    HashTablePos pos = {0};

    for (size_t i = 0; i < kTestCount; i++)
    {
        for (size_t j = 0; j < seek_word_set->word_count; j++)
        {
            HashTableFindElem(hash_table, seek_word_set->word_array[i].str, &pos);
        }
    }

    return kHashSuccess;
}
//================================================================================================
