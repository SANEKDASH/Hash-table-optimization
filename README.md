# Hash-table-optimization
Здраствуйте!

На этот раз задача была следующая:
1) Реализовать структуру данных хеш-таблица на языке программирования Си.
2) Построить гистограммы заселенности для нескольких хеш-функций.
3) Для каждой хеш-функции высчитать дисперсию распределения элементов по ячейкам.
4) Оптимизировать алгоритмы хеш-таблицы всячески используя язык Ассемблер.

## Установка

## Что такое хеш-таблица?
<img src="./readme_src/hash_table.png" alt="hash_table.png" width="800"/>

Хеш-таблица - структура данных, которая базируется на соответствии ключа(какие-то данные) и его хеш-значения(какое-то число, зависящее от ключа).
Ключ с его хеш-значением обычно называют парой.

Хеш-таблица позволяет выполнять над собой три операции:
1) Поиск пары по ключу.
2) Добавление пары.
3) Удаление пары.


Большой __плюс__ хеш-таблицы - скорость по сравнению с другими.

`В лучшем случае, доступ к данным можно совершить за O(1).`

`В худшем (при возникновении коллизий) - за O(n).`


Большой __минус__ хеш-таблицы - использование большого кол-ва памяти по сравнению с другими структурами данных.

### Коллизии
Ситуацию, при которой разным ключам соответствует одно и то же хеш-значение называется __коллизией__.

Существует несколько методов разрешения проблемы коллизий. О них вы можете узнать [здесь](https://ru.wikipedia.org/wiki/%D0%A5%D0%B5%D1%88-%D1%82%D0%B0%D0%B1%D0%BB%D0%B8%D1%86%D0%B0)

В своей реализации я использовал метод списков - каждое множество представляет собой список, содержащий ключи
с одинаковыми хеш-значениями

### Load Factor

Load factor - характеристика хеш-таблицы, описывающая среднее кол-во элементов в одном подмножестве.

Идеальным этот показатель считается <= 1.

В динамических хеш-таблицах (те, которые могут расширять кол-во списков) load factor поддерживается около 1 с помощью добавления списков. Такие действия облегчают поиск и делают его алгоритмическую сложность O(1).

В этой работе мы пользуемся статической хеш-таблицей (т.е. без возможности добавлять списки),
а load factor взяли равным примерно 7, чтобы лучше видеть узкие места в различных алгоритмах данной структуры данных.

Для тестирования в хеш-таблицу загружался текст произведения Шекспира "Гамлет" поделенный на слова.
Но мы заранее не знаем, сколько уникальных слов в тексте.

Поэтому алгоритм действий был следующим:
1)  Загрузить текст в хеш-таблицу.
`Функция добавления проверяет,
 существует ли добавлямый элемент таблице, если есть - не добавяет`

2) Узнаем, сколько уникальных слов в тексте.

`это число можно узнать по кол-ву элементов в хеш-таблице после загрузки`

Запоминаем это число.

3) Создаем новую хеш-таблицу, базируясь на известном кол-ве уникальных
элементов и load factor'е.

## Анализ хеш-функций
Для тестирования хеш-функций сначала в хеш-таблицу загружался текст произведения Шекспира "Гамлет" поделенный на слова.

__Количество списков__, полученное из алгоритма, указанного в предыдущем пункте, равно __797__


Идеальной хеш-функцией считается та, которая может обеспечить равномерное распределение элементов
хеш-таблицы по ее подмножествам/спискам при минимальном кол-ве __коллизий__.

Соответственно, мы можем определить насколько хорош алгоритм хеширования используя
такую вещь, как __дисперсия__.

__Дисперсия__ характеризует, насколько кольчество элементов в подможествах отличается от среднего кол-ва
элементов в подмножестве.

Считается дисперсия по следующей формуле:

![alt text](./readme_src/dispersion.png)

Соответственно, чем __меньше__ дисперсия - тем __равномернее__ распределение --> тем __лучше__ хеш-алгоритм.

Так же удобно строить __гистограммы заселенности__ хеш-таблицы.
Они позволяют наглядно увидеть, само распределение элементов по подножествам.

Далее будут приведены дисперсии и гистограммы заселенности для нескольких
реализованных мной хеш-алгоритмов.

1) хеш-функция, возвращающая ноль (далее ZeroHash):
```cpp
static uint64_t ZeroHash(char *data)
{
    return 0;
}

```

Гистограмма заселенности:
![alt text](./readme_src/zero_hash_diag.png)

Дисперсия: 40152.63

2) хеш-функция, возвращающая ASCII код первого символа в слове (далее StupidHash):
```cpp
static uint64_t StupidHash(char *data)
{
    return *data;
}

```


Гистограмма заселенности:
![alt text](./readme_src/stupid_hash_diag.png)

Дисперсия: 1741.259

3) хеш-функция, возвращающая длину слова (далее StrlenHash):
```cpp
static uint64_t StrlenHash(char *data)
{
    return strlen(data);
}

```


Гистограмма заселенности:
![alt text](./readme_src/strlen_hash_diag.png)

Дисперсия: 5558.15

4) хеш-функция, возвращающая сумму ASCII кодов всех символов в слове (далее SumHash):
```cpp
static uint64_t SumHash(char *data)
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
![alt text](./readme_src/sum_hash_diag.png)

Дисперсия: 42.29

5) хеш-функция, возвращающая частное от суммы ASCII кодов всех символов в слове и его длинны (далее SumStrlenHash):
```cpp
static uint64_t SumStrlenHash(char *data)
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
![alt text](./readme_src/sum_strlen_hash_diag.png)


Дисперсия: 2925.81

6) хеш-функция, использующая циклический сдвиг вправо(далее RorHash):
```cpp
static uint64_t RorHash(char *data)
{
    uint64_t hash_val = 0;

    for (size_t i = 0; data[i] != '\0'; i++)
    {
        hash_val = ((hash_val >> 1) | (hash_val << 63)) ^ data[i];
    }

    return hash_val;
}
```

Гистограмма заселенности:
![alt text](./readme_src/ror_hash_diag.png)


Дисперсия: 27.98

7) Хещ-функция использующая циклический сдвиг влево (Далле RolHash:
```cpp
static uint64_t RolHash(char *data)
{
    uint64_t hash_val = 0;

    for (size_t i = 0; data[i] != '\0'; i++)
    {

        hash_val = ((hash_val << 1) | (hash_val >> 63)) ^ data[i];

    }

    return hash_val;
}
```

Гистограмма заселенности:
![alt text](./readme_src/rol_hash_diag.png)

Дисперсия: 13.27

8) CRC32Hash:
```cpp
uint64_t CRC32Hash(char *data)
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
![alt text](./readme_src/crc32_hash_diag.png)

Дисперсия: 9.58

Можем видеть, что минимальная диспресия принадлежит функции CRC32Hash().
Поэтому в дальнейших тестах будем использовать её.


### Странные дела с RorHash() и RolHash()
Почему-то случилось так, что в ассемблере существуют инструкции
ror и rol (циклический сдвиг вправо и влево соответственно), но в языках C/C++ они не реализованы.

Используя сайт [godbolt](https://godbolt.org/) я посмотрел, во что компилятор преобразует
код этих двух интересных функций.

Моему удивлению не было предела, когда я увидел заветные инструкции ror и rol:
![alt text](./readme_src/ror_rol.png)

Видимо компилятор g++ достаточно умен, чтобы найти в коде ror и rol.

## Поиск узких мест
Для поиска узких мест в алгоритмах хеш-таблицы я использовал утилиту __perf__.

Для тестов был взят текст произведения Шекспира "Король Лир", поделенный на слова.

Тест представляет из себя поиск всех слов из "Короля Лир" 2560 раз.

Для начала проведем профилирование для базовой версии (без оптимизации):

```
------------------------------------------------------------------------------------------------------------+
Call persantage:                                                                                                 |
------------------------------------------------------------------------------------------------------------+

# ........  .........  ....................  ................................................................
#
    74.57%  HashTable  HashTable             [.] CRC32Hash(char*)
    10.93%  HashTable  HashTable             [.] ListFind(List*, char*, unsigned long*)
     7.16%  HashTable  libc.so.6             [.] __strcmp_evex
     5.85%  HashTable  HashTable             [.] HashTableFindElem(HashTable*, char*, HashTablePos*)
     0.83%  HashTable  HashTable             [.] TestHashTable(HashTable*, WordSet*)
     0.53%  HashTable  HashTable             [.] strcmp@plt
     0.01%  HashTable  libc.so.6             [.] _int_free
     0.01%  HashTable  libc.so.6             [.] _IO_fread

------------------------------------------------------------------------------------------------------------+
Stat:                                                                                                       |
------------------------------------------------------------------------------------------------------------+

          9 399,92 msec task-clock                       #    1,000 CPUs utilized               ( +-  0,16% )
                32      context-switches                 #    3,404 /sec                        ( +-  9,35% )
                 2      cpu-migrations                   #    0,213 /sec                        ( +- 21,24% )
               840      page-faults                      #   89,362 /sec                        ( +-  0,02% )
    42 048 202 548      cycles                           #    4,473 GHz                         ( +-  0,03% )
    60 452 494 102      instructions                     #    1,44  insn per cycle              ( +-  0,00% )
     6 325 683 532      branches                         #  672,951 M/sec                       ( +-  0,00% )
           209 171      branch-misses                    #    0,00% of all branches             ( +- 11,62% )
                        TopdownL1                 #     65,8 %  tma_backend_bound
                                                  #      3,5 %  tma_bad_speculation
                                                  #      0,4 %  tma_frontend_bound
                                                  #     30,2 %  tma_retiring             ( +-  0,07% )

            9,4005 +- 0,0148 seconds time elapsed  ( +-  0,16% )

------------------------------------------------------------------------------------------------------------+
```

Результаты профилирования для базовой версии + O3:

```
------------------------------------------------------------------------------------------------------------+
Call persantage:                                                                                                 |
------------------------------------------------------------------------------------------------------------+

# Overhead  Command    Shared Object     Symbol
# ........  .........  ................  ....................................................................
#
    51.85%  HashTable  HashTable         [.] CRC32Hash(char*)
    18.45%  HashTable  libc.so.6         [.] __strcmp_evex
    16.21%  HashTable  HashTable         [.] ListFind(List*, char*, unsigned long*)
    10.97%  HashTable  HashTable         [.] TestHashTable(HashTable*, WordSet*)
     1.96%  HashTable  HashTable         [.] strcmp@plt
     0.05%  HashTable  libc.so.6         [.] _int_memalign

------------------------------------------------------------------------------------------------------------+
Stat:                                                                                                       |
------------------------------------------------------------------------------------------------------------+

          2 639,31 msec task-clock                       #    1,000 CPUs utilized               ( +-  0,29% )
                 7      context-switches                 #    2,652 /sec                        ( +- 10,39% )
                 0      cpu-migrations                   #    0,000 /sec
             3 499      page-faults                      #    1,326 K/sec                       ( +-  0,00% )
    11 833 823 771      cycles                           #    4,484 GHz                         ( +-  0,28% )
    27 487 905 447      instructions                     #    2,32  insn per cycle              ( +-  0,00% )
     2 745 059 665      branches                         #    1,040 G/sec                       ( +-  0,00% )
         1 245 007      branch-misses                    #    0,05% of all branches             ( +- 89,03% )
                        TopdownL1                 #     50,6 %  tma_backend_bound
                                                  #      2,1 %  tma_bad_speculation
                                                  #      1,7 %  tma_frontend_bound
                                                  #     45,6 %  tma_retiring             ( +-  0,29% )

           2,63974 +- 0,00767 seconds time elapsed  ( +-  0,29% )

------------------------------------------------------------------------------------------------------------+
```


Как можем заметить, узкими местами в алгоритмах хеш-таблицы являются следующие функции:
1) CRC32Hash()
2) ListFind() (вызывается следующей функцией)
3) HashTableFindElem()

>__strcmp_evex() я здесь не учел, так как эту функцию писали мужики, знающие свое дело.
>Вряд ли я смогу его ускорить, но есть одна хитрость, которая мне в этом поможет.
>Позже я вам о ней поведаю.

Что ж, давайте оптимизировать!

## Оптимизация

Перед тем, как приступить к оптимизации, хотелось бы подчеркнуть тот факт, что ускорение, полученное мной, может розниться
от системы к системе. Ускорение может зависеть от различного множества всяческих факторов: от версии компилятора до
комплектующих вашего ПК.
Ниже преведены параметры моей системы:

|                               |                        |
|:-----------------------------:|:----------------------:|
|Процессор                      | Intel core i5 11400H.  |
|Кол-во оперативной памяти      | 16гб.                  |
|ОС                             | Ubuntu Linux 22.04.4.  |
|Версия ядра Ubuntu             | 6.5.0-26-generic.      |
|Используемый компилятор        | g++                    |
|Версия компилятора             | 11.4.0                 |



### Оптимизация №1: хеш-функция
Давайте попытаемся оптимизировать самую тяжелую функцию - функцию хеширования.

Так совпало (зуб даю, реально совпало), что существует:
1) инструкция процессора __crc32__
2) intrinsic функция ___mm_crc32_u8()__

 А методов реализовывать оптимизацию существует аж целых 3:
1) функция языка Си под названием asm()
2) функция написанная на языке Ассемблер, вызываемая в Си
3) intrinsic функции

Для примера я написал функцию на Ассемблере, которая считает хеш CRC32:
```nasm
asm_CRC32Hash:
; Entering function
    push rbp
    mov rbp, rsp

    xor rax, rax
; Hash start value
    mov eax, 0xffffffff

    jmp .HashTest
.HashCycle:
    crc32 eax, byte [rdi]

    inc rdi
.HashTest:
    cmp byte [rdi], 0
    ja .HashCycle

; Leaving function
    mov rsp, rbp
    pop rbp

    ret
```

Результаты профилирования для версии с asm_CRC32Hash():
```
------------------------------------------------------------------------------------------------------------+
Call persantage:                                                                                                 |
------------------------------------------------------------------------------------------------------------+

# Overhead  Command    Shared Object         Symbol
# ........  .........  ....................  ................................................................
#
    43.91%  HashTable  HashTable             [.] ListFind(List*, char*, unsigned long*)
    25.63%  HashTable  libc.so.6             [.] __strcmp_evex
    13.99%  HashTable  HashTable             [.] HashTableFindElem(HashTable*, char*, HashTablePos*)
     5.87%  HashTable  HashTable             [.] asm_CRC32Hash.HashTest
     4.67%  HashTable  HashTable             [.] TestHashTable(HashTable*, WordSet*)
     3.35%  HashTable  HashTable             [.] strcmp@plt
     1.86%  HashTable  HashTable             [.] asm_CRC32Hash
     0.12%  HashTable  libc.so.6             [.] _int_malloc

------------------------------------------------------------------------------------------------------------+
Stat:                                                                                                       |
------------------------------------------------------------------------------------------------------------+

          1 757,61 msec task-clock                       #    1,000 CPUs utilized               ( +-  0,65% )
                 7      context-switches                 #    3,983 /sec                        ( +-  9,17% )
                 0      cpu-migrations                   #    0,000 /sec
               841      page-faults                      #  478,490 /sec                        ( +-  0,02% )
     7 627 247 609      cycles                           #    4,340 GHz                         ( +-  0,28% )
    22 847 753 012      instructions                     #    3,00  insn per cycle              ( +-  0,00% )
     3 441 280 626      branches                         #    1,958 G/sec                       ( +-  0,00% )
           594 347      branch-misses                    #    0,02% of all branches             ( +- 68,39% )
                        TopdownL1                 #     33,1 %  tma_backend_bound
                                                  #      3,6 %  tma_bad_speculation
                                                  #      3,8 %  tma_frontend_bound
                                                  #     59,5 %  tma_retiring             ( +-  0,29% )

            1,7580 +- 0,0115 seconds time elapsed  ( +-  0,65% )

------------------------------------------------------------------------------------------------------------+
```

Как можем видеть, функция хеширования сдвинулась с первого на последнее место в чарте call graph.
Это не может не радовать.
Но у нас все еще осталась работа в виде оптимизации функции поиска.

### Оптимизация №2: поиск

Как можем видеть из call graph, в функции HashTableFindElem() расходуется на ListFInd() и __strcmp_evex(), который вызывается с-под ListFind().

Оптимизация __strcmp_evex() является довольно сложной ~~быть может невозможной~~ задачей, так как он уже оптимизирован.
Хитрость в этом случае заключается в том, что в данной задаче (поиск заранее известных слов) мы можем переити от общего к частному.

> Длины слов в обоих текстах не превышают 37 --> не превышают 64 символов.

С помощью технологии AVX-512, поддерживаемой на моем процессоре, я попытался ускорить алгоритм поиска, используя intrinsic функции.

Список используемых intrinsic ф-ций:
1) _mm512_load_epi64() - загружает в регистр 64 байт памяти, расположенной по адресу, выравненному по 64.

2) _mm512_cmp_epi16_mask() - сравнивает две пачки памяти по 64 байт каждая.

Результаты профилирования для версии asm_CRC32Hash() + O3 (до оптимизации поиска):
```
------------------------------------------------------------------------------------------------------------+
Call persantage:                                                                                            |
------------------------------------------------------------------------------------------------------------+

# Overhead  Command    Shared Object         Symbol
# ........  .........  ....................  ................................................................
#
    33.79%  HashTable  HashTable             [.] ListFind(List*, char*, unsigned long*)
    31.93%  HashTable  libc.so.6             [.] __strcmp_evex
    13.90%  HashTable  HashTable             [.] TestHashTable(HashTable*, WordSet*)
    10.18%  HashTable  HashTable             [.] asm_CRC32Hash.HashTest
     5.76%  HashTable  HashTable             [.] strcmp@plt
     3.26%  HashTable  HashTable             [.] asm_CRC32Hash
------------------------------------------------------------------------------------------------------------+
Stat:                                                                                                       |
------------------------------------------------------------------------------------------------------------+

            895,20 msec task-clock                       #    0,999 CPUs utilized               ( +-  0,86% )
                 3      context-switches                 #    3,351 /sec                        ( +- 14,18% )
                 0      cpu-migrations                   #    0,000 /sec
             3 497      page-faults                      #    3,906 K/sec                       ( +-  0,00% )
     3 891 269 483      cycles                           #    4,347 GHz                         ( +-  0,25% )
    13 170 303 836      instructions                     #    3,38  insn per cycle              ( +-  0,00% )
     2 970 488 330      branches                         #    3,318 G/sec                       ( +-  0,00% )
           191 652      branch-misses                    #    0,01% of all branches             ( +- 35,17% )
                        TopdownL1                 #     20,5 %  tma_backend_bound
                                                  #      1,6 %  tma_bad_speculation
                                                  #     10,1 %  tma_frontend_bound
                                                  #     67,9 %  tma_retiring             ( +-  0,10% )

           0,89572 +- 0,00772 seconds time elapsed  ( +-  0,86% )

------------------------------------------------------------------------------------------------------------+
```

Результаты профилирования для версии asm_CRC32Hash() + avx_ListFind + O3 (после оптимизации поиска):
```
------------------------------------------------------------------------------------------------------------+
Call persantage:                                                                                            |
------------------------------------------------------------------------------------------------------------+

# Overhead  Command    Shared Object         Symbol
# ........  .........  ....................  ................................................................
#
    53.48%  HashTable  HashTable             [.] ListFind(List*, char*, unsigned long*)
    23.14%  HashTable  HashTable             [.] TestHashTable(HashTable*, WordSet*)
    16.76%  HashTable  HashTable             [.] asm_CRC32Hash.HashTest
     4.66%  HashTable  HashTable             [.] asm_CRC32Hash
------------------------------------------------------------------------------------------------------------+
Stat:                                                                                                       |
------------------------------------------------------------------------------------------------------------+

            585,69 msec task-clock                       #    0,999 CPUs utilized               ( +-  0,94% )
                 1      context-switches                 #    1,707 /sec                        ( +- 31,01% )
                 0      cpu-migrations                   #    0,000 /sec
             3 498      page-faults                      #    5,972 K/sec                       ( +-  0,01% )
     2 479 831 839      cycles                           #    4,234 GHz                         ( +-  0,13% )
     6 898 250 104      instructions                     #    2,78  insn per cycle              ( +-  0,00% )
     1 716 747 962      branches                         #    2,931 G/sec                       ( +-  0,00% )
           165 197      branch-misses                    #    0,01% of all branches             ( +- 40,34% )
                        TopdownL1                 #     18,1 %  tma_backend_bound
                                                  #      1,4 %  tma_bad_speculation
                                                  #     21,9 %  tma_frontend_bound
                                                  #     58,7 %  tma_retiring             ( +-  0,12% )

           0,58613 +- 0,00549 seconds time elapsed  ( +-  0,94% )

------------------------------------------------------------------------------------------------------------+
```
Как видим, с помощью intrinsic ф-ций получилось еще больше ускорить программу.

А strcmp() и вовсе пропал из графа вызовов (неудевительно, т.к. он перестал использоваться).

Так же стоит отметить, что флаг -O3 (или -O1, -O2) здесь существенны, т.к. без этого флага эффективность intrinsic ф-ций заметно падает.

### Оптимизация №3: вернемся к хешу

Давайте посмотрим на еще один вариант оптимизации хеш-алгоритма - inline Ассемблер.

Язык C/C++ позволяет делать ассемблерные вставки прямо посреди основного кода программы.

Как мне кажется, к этому методу стоит только при каком-то определенном требовании свыше,
или настоять на другом методе, т.к. он ухудшает достаточно сильно читаемость кода,
особенно если используется больше одного раза.

Новая функция хеширования выглядит так:
```cpp

static inline uint64_t asm_inline_CRC32Hash(char *data)
{
    uint64_t hash_val = 0;

        asm(
            "    xor rax, rax                       \n"
            "    mov eax, 0xffffffff                \n"
            "    jmp HashTest                       \n"
            "HashCycle:                             \n"
            "    crc32 eax, byte ptr [%[str]]       \n"
            "    inc %[str]                         \n"
            "HashTest:                              \n"
            "    cmp byte ptr [%[str]], 0           \n"
            "    ja HashCycle                       \n"
            "    mov %[hash], rax                   \n"
                : [hash] "=m" (hash_val)
                : [str]   "d" (data)
           :);

    return hash_val;
}

```

Результаты профилирования для версии asm_inline_CRC32Hash + avx_ListFind + O3:
```
------------------------------------------------------------------------------------------------------------+
Call persantage:                                                                                            |
------------------------------------------------------------------------------------------------------------+

# Overhead  Command    Shared Object     Symbol
# ........  .........  ................  ....................................................................
#
    49.18%  HashTable  HashTable         [.] ListFind(List*, char*, unsigned long*)
    21.11%  HashTable  HashTable         [.] TestHashTable(HashTable*, WordSet*)
    19.77%  HashTable  HashTable         [.] HashTest
     8.04%  HashTable  HashTable         [.] asm_inline_CRC32Hash(char*)
     0.38%  HashTable  libc.so.6         [.] _int_malloc

------------------------------------------------------------------------------------------------------------+
Stat:                                                                                                       |
------------------------------------------------------------------------------------------------------------+

            551,84 msec task-clock                       #    0,999 CPUs utilized               ( +-  0,63% )
                 2      context-switches                 #    3,624 /sec                        ( +- 26,56% )
                 0      cpu-migrations                   #    0,000 /sec
             3 497      page-faults                      #    6,337 K/sec                       ( +-  0,00% )
     2 371 819 805      cycles                           #    4,298 GHz                         ( +-  0,29% )
     7 477 981 509      instructions                     #    3,15  insn per cycle              ( +-  0,00% )
     1 789 211 268      branches                         #    3,242 G/sec                       ( +-  0,00% )
            92 801      branch-misses                    #    0,01% of all branches             ( +-  0,37% )
                        TopdownL1                 #     25,0 %  tma_backend_bound
                                                  #      1,4 %  tma_bad_speculation
                                                  #      7,7 %  tma_frontend_bound
                                                  #     65,8 %  tma_retiring             ( +-  0,14% )

           0,55228 +- 0,00349 seconds time elapsed  ( +-  0,63% )

------------------------------------------------------------------------------------------------------------+
```

Результаты для той же версии, но без SIMD ListFind():
```
------------------------------------------------------------------------------------------------------------+
Call persantage:                                                                                            |
------------------------------------------------------------------------------------------------------------+

# Overhead  Command    Shared Object     Symbol
# ........  .........  ................  ....................................................................
#
    32.02%  HashTable  HashTable         [.] ListFind(List*, char*, unsigned long*)
    31.55%  HashTable  libc.so.6         [.] __strcmp_evex
    12.99%  HashTable  HashTable         [.] TestHashTable(HashTable*, WordSet*)
    12.11%  HashTable  HashTable         [.] HashTest
     5.24%  HashTable  HashTable         [.] strcmp@plt
     4.81%  HashTable  HashTable         [.] asm_inline_CRC32Hash(char*)
     0.27%  HashTable  libc.so.6         [.] _int_malloc

------------------------------------------------------------------------------------------------------------+
Stat:                                                                                                       |
------------------------------------------------------------------------------------------------------------+

            884,27 msec task-clock                       #    1,000 CPUs utilized               ( +-  0,31% )
                 3      context-switches                 #    3,393 /sec                        ( +- 15,00% )
                 0      cpu-migrations                   #    0,000 /sec
             3 498      page-faults                      #    3,956 K/sec                       ( +-  0,00% )
     3 957 137 509      cycles                           #    4,475 GHz                         ( +-  0,25% )
    13 750 058 726      instructions                     #    3,47  insn per cycle              ( +-  0,00% )
     3 042 952 655      branches                         #    3,441 G/sec                       ( +-  0,00% )
           122 117      branch-misses                    #    0,00% of all branches             ( +-  5,13% )
                        TopdownL1                 #     19,0 %  tma_backend_bound
                                                  #      1,8 %  tma_bad_speculation
                                                  #     10,6 %  tma_frontend_bound
                                                  #     68,6 %  tma_retiring             ( +-  0,22% )

           0,88468 +- 0,00271 seconds time elapsed  ( +-  0,31% )

------------------------------------------------------------------------------------------------------------+
```

Как мы можем видеть, полученные результаты практически не отличаются от тех, что получены для полностью ассемблерной
версии хеш-функции.

Отличия в O3 версиях можно описать тем, что в функции asm_CRC32Hash() присутствуют наклодные расходы
на call и ret (__конвеер__ на них преостанавливается).

### Обсудим результаты

После неравного боя с ассемблерными оптимизациями мы все-таки добились своего -
оптимизировали хэш-таблицу.

Все ранее полученные результаты можно поместить в таблицу:

| Версия                        |Среднее время выполнения, сек  |
|:-----------------------------:|:----------------------:|
|(0) Базовая версия                                             | 9.40 +- 0.01           |
|(1) Базовая версия + O3                                        | 2,64 +- 0,02           |
|(2) Базовая версия + asm_CRC32Hash()                           | 1.79 +- 0.01           |
|(3) Базовая версия + asm_CRC32Hash() + O3                      | 0,90 +- 0,01           |
|(4) Базовая версия + asm_CRC32Hash() + SIMD поиск + O3         | 0,59 +- 0,01           |
|(5) Базовая версия + asm_inline_CRC32Hash + SIMD поиск + O3    | 0,55 +- 0.01           |
|(6) Базовая версия + asm_inline_CRC32Hash + O3                 | 0,89 +- 0,01           |

Самой быстрой версией по сравнению с базовой оказалась итерация №5 (ускорение в 17.1 раз), в которой использовались:
1) Возможности языка C/C++ вставлять куски ассемблерного кода в основной.

2) SIMD инструкции в совокупности с O3 (без O3, как говорилось ранее, в SIMD инструкциях не было бы смысла).

В заключение хочется сказать, что к ассемблерным оптимизациям стоит прибегать в случае, когда
требуется выжать максимум из апаратных возможностей конкретной машины, т.к.
после внедрения внедрения таких оптимизаций код программы становится менее читаемым, а
сама программа менее переносимой на другие машины.

Например: в этой работе мне пришлось применить технологии AVX-512, доступные на новых поколениях процессоров.
Это значит, что на более старых процессорах моя программа банально не запустилась бы.

### Без этих людей, я бы не смог выполнить эту работу:
1) [Денис Дедков](https://github.com/d3phys)
2) [Илья Дединский](https://github.com/ded32)
