#include <stdio.h>
#include <stdlib.h>
#include <immintrin.h>

#include "list.h"
#include "../ListDump/list_dump.h"



//================================================================================================

static const size_t kStartListSize = 8;

static const int kFreePoison = -1;

static const int kMultiplier = 2;

static const size_t kMaxValidListSize = 65536;

//================================================================================================

static ListState_t ResizeList(List *list, size_t new_size);

static ListState_t ListLinearization(List *list);

static void MemSetList(List *list, ListElemType_t value);

//================================================================================================

ListState_t ListVerify(List *list)
{
    CHECK(list);

    if (list->head > kMaxValidListSize)
    {
        list->status |= kHeadLessZero;
    }

    if (list->data == nullptr)
    {
        list->status |= kNullData;
    }

    if (list->capacity < 0)
    {
        list->status |= kCapacityLessZero;
    }

    if (list->tail < list->head)
    {
        list->status |= kTailLessHead;
    }

    if (list->tail < 0)
    {
        list->status |= kTailLessZero;
    }

    if (list->elem_count > kMaxValidListSize)
    {
        list->status |= kElemCountLessZero;
    }

    if (list->prev[0] != list->tail)
    {
        list->status |= kWrongPrev;
    }

    if (list->next[list->tail] != 0)
    {
        list->status |= kWrongTailNext;
    }

    return list->status;
}

//================================================================================================

ListState_t ListConstructor(List *list)
{
    CHECK(list);

    list->capacity = kStartListSize;

    list->data = (ListElemType_t *) calloc(list->capacity, sizeof(ListElemType_t));
    list->prev = (int *)            calloc(list->capacity, sizeof(int));
    list->next = (int *)            calloc(list->capacity, sizeof(int));

    if (list->data == nullptr || list->prev == nullptr || list->next == nullptr)
    {
        list->status = kFailedAllocation;

        free(list->data);
        free(list->prev);
        free(list->next);

        return list->status;
    }

    list->status = kListClear;

    list->tail       = 0;
    list->head       = 0;
    list->free       = 1;
    list->elem_count = 0;

    MemSetList(list, kPoisonVal);

    list->data[0] = kPoisonVal;
    list->next[0] = list->head;
    list->prev[0] = list->head;

    return kListClear;
}

//================================================================================================

ListState_t ListDestructor(List *list)
{
    CHECK(list);

    free(list->data);
    free(list->prev);
    free(list->next);

    list->data = nullptr;

    list->prev = list->next = nullptr;

    return kListClear;
}

//================================================================================================

ListState_t ListAddAfter(List *list,
                         size_t pos,
                         ListElemType_t value)
{
    CHECK      (list);
    GRAPH_DUMP (list);
    LIST_VERIFY(list);

    if (list->status != kListClear)
    {
        return list->status;
    }

    if (pos < 0 || pos >= kMaxValidListSize)
    {
        return kWrongUsingOfList;
    }

    if (list->prev[pos] == kFreePoison)
    {
        return kWrongUsingOfList;
    }

    if (list->next[list->free] == 0)
    {
        ResizeList(list, list->capacity * kMultiplier);
    }

    ++list->elem_count;

    size_t data_pos = list->free;

    list->free = list->next[data_pos];

    if (list->next[pos] == 0)
    {
        list->tail = data_pos;
    }

    list->data[data_pos] = (ListElemType_t) aligned_alloc(64, 64 * sizeof(char));

    memcpy(list->data[data_pos], value, 64);

    list->prev[data_pos] = pos;
    list->next[data_pos] = list->next[pos];

    list->prev[list->next[pos]] = data_pos;
    list->next[pos]             = data_pos;

    GRAPH_DUMP(list);

    return kListClear;
}

//================================================================================================

ListState_t ListAddBefore(List *list,
                           size_t pos,
                           ListElemType_t value)
{
    CHECK(list);

    return ListAddAfter(list, pos - 1, value);
}

//================================================================================================

static ListState_t ResizeList(List *list, size_t new_size)
{
    CHECK(list);

    ListLinearization(list);

    list->capacity = new_size;

    list->data = (ListElemType_t *) realloc(list->data, list->capacity * sizeof(ListElemType_t));
    list->prev = (int *)            realloc(list->prev, list->capacity * sizeof(int));
    list->next = (int *)            realloc(list->next, list->capacity * sizeof(int));

/*
    Cppreference : https://en.cppreference.com/w/c/memory/free

    The function accepts (and does nothing with) the null pointer
    to reduce the amount of special-casing. Whether allocation succeeds or not,
    the pointer returned by an allocation function can be passed to free().
*/

    if (list->data == nullptr || list->prev == nullptr || list->next == nullptr)
    {
        list->status |= kFailedReallocation;

        free(list->data);
        free(list->prev);
        free(list->next);

        return kFailedReallocation;
    }

    MemSetList(list, kPoisonVal);

    return kListClear;
}

//================================================================================================

#ifdef LIST_FIND_OPTIMIZE

    #define OPTIMIZE_ONLY(...) __VA_ARGS__

#else

    #define OPTIMIZE_ONLY(...) ;

#endif

//================================================================================================

ListState_t ListFind(List           *list,
                     ListElemType_t  val,
                     size_t         *list_pos)
{
    CHECK(list);
    CHECK(list_pos);

    OPTIMIZE_ONLY(__m512i m_val = _mm512_load_epi64(val);)

    size_t cur_pos = 0;

    while(list->next[cur_pos] != 0)
    {
        if (list->data[cur_pos] != nullptr)
        {
            #ifdef LIST_FIND_OPTIMIZE

                __m512i cur_data = _mm512_load_epi64(list->data[cur_pos]);

                if(_mm512_cmp_epi16_mask(m_val, cur_data, _MM_CMPINT_NE) == 0)
                {
                    *list_pos = cur_pos;

                    return kListClear;
                }

            #else

                if (strcmp(val, list->data[cur_pos]) == 0)
                {
                    *list_pos = cur_pos;

                    return kListClear;
                }

            #endif
        }

        cur_pos = list->next[cur_pos];
    }

    return kCantFind;
}

//================================================================================================

#undef OPTIMIZE_ONLY

//================================================================================================

ListState_t ListDelete(List   *list,
                       size_t  pos)
{
    CHECK(list);

    if (ListVerify(list) != kListClear)
    {
        return list->status;
    }

    if (list->prev[pos] == kFreePoison || pos <= 0)
    {
        return kWrongUsingOfList;
    }

    if ((list->elem_count < list->capacity / (kMultiplier * kMultiplier)) &&
        (list->capacity > kStartListSize))
    {
        ResizeList(list, list->capacity / kMultiplier);
    }

    if (pos == list->tail)
    {
        list->tail = list->prev[pos];
    }

    list->prev[list->next[pos]] = list->prev[pos];
    list->next[list->prev[pos]] = list->next[pos];

    list->data[pos] = kPoisonVal;
    list->next[pos] = list->free;
    list->prev[pos] = -1;

    list->free = pos;

    --list->elem_count;

    GRAPH_DUMP(list);

    return kListClear;
}

//================================================================================================

static void MemSetList(List           *list,
                       ListElemType_t  value)
{
    CHECK(list);

    size_t i = list->tail + 1;

    for ( ; i < list->capacity - 1 ; ++i)
    {
        list->data[i] = value;

        list->next[i] = i + 1;

        list->prev[i] = -1;
    }

    list->data[i] = value;
    list->next[i] = 0;
    list->prev[i] = -1;

    GRAPH_DUMP(list);
}

//================================================================================================

static ListState_t ListLinearization(List *list)
{
    ListElemType_t *line_data = (ListElemType_t *) calloc(list->capacity, sizeof(ListElemType_t));

    if (line_data == nullptr)
    {
        return kFailedAllocation;
    }

    size_t pos = list->next[0];

    size_t i = 1;

    while (i < list->elem_count)
    {
        line_data[i] = list->data[pos];

        pos = list->next[pos];

        list->next[i - 1] = i;

        list->prev[i] = i - 1;

        ++i;
    }

    line_data[i] = list->data[pos];

    free(list->data);

    list->next[i - 1] = i;

    list->prev[i] = i - 1;
    list->next[i] = 0;

    list->prev[0] = i;

    list->data = line_data;
    list->tail = i;
    list->free = i + 1;

    return kListClear;
}

//================================================================================================
