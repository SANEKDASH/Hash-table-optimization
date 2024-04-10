CC=g++
CFLAGS=-c -O3 -Wall -march=native -Wshadow -Winit-self -Wredundant-decls -Wcast-align -Wundef -Wfloat-equal -Winline -Wunreachable-code -Wmissing-declarations -Wmissing-include-dirs -Wswitch-enum -Wswitch-default -Weffc++ -Wmain -Wextra -Wall -g -pipe -fexceptions -Wcast-qual -Wconversion -Wctor-dtor-privacy -Wempty-body -Wformat-security -Wformat=2 -Wignored-qualifiers -Wlogical-op -Wno-missing-field-initializers -Wnon-virtual-dtor -Woverloaded-virtual -Wpointer-arith -Wsign-promo -Wstack-usage=8192 -Wstrict-aliasing -Wstrict-null-sentinel -Wtype-limits -Wwrite-strings -Werror=vla -D_EJUDGE_CLIENT_SIDE
LDFLAGS=
SOURCES=main.cpp Text-parsing-functions/text_parse.cpp  \
				 FastList/list.cpp 					    \
				 ListDump/list_dump.cpp 				\
				 debug/debug.cpp 					    \
				 debug/color_print.cpp					\
				 hash_table.cpp

OBJECTS=$(SOURCES:.cpp=.o)

EXECUTABLE=HashTable

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
