CC = g++

CFLAGS = -c -march=native -O3 -DASM_INLINE_CRC32 -masm=intel -DGET_LOAD_DATA

LDFLAGS =

SOURCES = main.cpp 								\
		  Text-parsing-functions/text_parse.cpp \
		  FastList/list.cpp 				    \
		  ListDump/list_dump.cpp 				\
		  debug/debug.cpp 					    \
		  debug/color_print.cpp					\
		  hash_table.cpp

OBJECTS = $(SOURCES:.cpp=.o) asm/crc32hash.o

EXECUTABLE = HashTable

all: $(SOURCES) $(EXECUTABLE)
$(EXECUTABLE): $(OBJECTS)
	@$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	@$(CC) $(CFLAGS) $< -o $@

clean:
	@rm -rf Text-parsing-functions/*.o HashTable
	@rm -rf ListDump/*.o HashTable
	@rm -rf FastList/*.o HashTable
	@rm -rf debug/*.o HashTable
	@rm -rf *.html HashTable
	@rm -rf *.o
	@rm -rf *.old
	@rm -rf *.data

	@rm -rf *.csv
	@rm -rf dump_src/*.png HashTable

run:
	@./HashTable word_set.txt seek_word_set.txt

test:
	@perf record ./HashTable word_set.txt seek_word_set.txt
	@perf report > perf_report.txt

stat:
	@perf stat -r 20 ./HashTable word_set.txt seek_word_set.txt
