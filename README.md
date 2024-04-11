# Hash-table-optimization
Здраствуйте!

На этот раз задача была следующая:
1) Реализовать структуру данных хэш-таблица на языке программирования Си.
2) Построить гистограммы заселенности для нескольких хэш-функций.
3) Для каждой хэш-функции высчитать дисперсию распределения элементов по ячейкам.

## Установка

## Что такое хэш-таблица?

## Анализ хэш-функций
Для хэш-таблицы было реализованно несколько хэш-функций:
1) Хэш-функция, возвращающая ноль (далее ZeroHash):
```cpp
static uint64_t ZeroHash(ListElemType_t data)
{
    return 0;
}

```

Гистограмма заселенности:
    картинка

Дисперсия: 40152.63

2) Хэш-функция, возвращающая ASCII код первого символа в слове (далее StupidHash):
```cpp
static uint64_t StupidHash(ListElemType_t data)
{
    return *data;
}

```


Гистограмма заселенности:
    картинка

Дисперсия: 1741.259

3) Хэш-функция, возвращающая длину слова (далее StrlenHash):
```cpp
static uint64_t StrlenHash(ListElemType_t data)
{
    return strlen(data);
}

```


Гистограмма заселенности:
    картинка

Дисперсия: 5558.15

4) Хэш-функция, возвращающая сумму ASCII кодов всех символов в слове (далее SumHash):
```cpp
static uint64_t SumHash(ListElemType_t data)
{
    uint64_t sum = 0;

    for (size_t i = 0; data[i] != '\0'; i++)
    {
        sum += data[i];
    }

    return sum;
}
```


Гистограмма заселенности:
    картинка

Дисперсия: 42.29

5) Хэш-функция, возвращающая частное от суммы ASCII кодов всех символов в слове и его длинны (далее SumStrlenHash):
```cpp
static uint64_t SumStrlenHash(ListElemType_t data)
{
    uint64_t sum = SumHash(data);

    uint64_t word_len = strlen(data);

    if (word_len == 0)
    {
        return 0;
    }

    return (uint64_t) (sum / word_len);
}
```


Гистограмма заселенности:
    картинка

Дисперсия: 2925.81

6) Хэш-функция, реализиющая следующий алгоритм(КАВО!!!)(далее RorHash):
```cpp
static uint64_t RorHash(ListElemType_t data)
{
    uint64_t hash_val = 0;

    for (size_t i = 0; data[i] != '\0'; i++)
    {
        hash_val = (hash_val | (hash_val & (~((~0) << 1)))) ^ data[i];
    }

    return hash_val;
}
```

Гистограмма заселенности:
    картинка

Дисперсия: 27.98

7) LorHash:
```cpp
static uint64_t LorHash(ListElemType_t data)
{
    uint64_t hash_val = 0;

    for (size_t i = 0; data[i] != '\0'; i++)
    {

        hash_val = ((hash_val << 1) | ((hash_val & (~((~0) >> 1))) >> 63)) ^ data[i];

    }

    return hash_val;
}
```

Гистограмма заселенности:
    картинка

Дисперсия: 13.27

8) CRC32Hash:
```cpp
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
```

Гистограмма заселенности:
    картинка

Дисперсия: 9.58

## Поиск узких мест
Для поиска узких мест в алгоритмах хэш-таблицы я использовал утилиту perf.

Для тестов был взят текст произведения Гамлета "Король Лир".

Чтобы узнать исходные показатели (производительности)? будем компилировать программу
без всяких флагов оптимизации.

Результаты профилирования №1:
    фотка ы

Как можем заметить, узкими местами в алгоритмах хэш-таблицы являются следующие функции:
1) CRC32Hash()
2) ListFind() (вызывается следующей функцией)
3) HashTableFindElem()

>__strcmp_evex() я здесь не учел, так как эту функцию писали мужики, знающие свое дело.
>Врядли я смогу его ускорить, но есть одна хитрость. Позже я вам о ней поведаю.


