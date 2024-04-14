CC=g++
CFLAGS=-c -Wall -march=native -DWORD_IS_64_BYTE_SIZE -Wshadow -Winit-self -Wredundant-decls -Wcast-align -Wundef -Wfloat-equal -Winline -Wunreachable-code -Wmissing-declarations -Wmissing-include-dirs -Wswitch-enum -Wswitch-default -Weffc++ -Wmain -Wextra -Wall -g -pipe -fexceptions -Wcast-qual -Wconversion -Wctor-dtor-privacy -Wempty-body -Wformat-security -Wformat=2 -Wignored-qualifiers -Wlogical-op -Wno-missing-field-initializers -Wnon-virtual-dtor -Woverloaded-virtual -Wpointer-arith -Wsign-promo -Wstack-usage=8192 -Wstrict-aliasing -Wstrict-null-sentinel -Wtype-limits -Wwrite-strings -Werror=vla -D_EJUDGE_CLIENT_SIDE
LDFLAGS=
SOURCES=main.cpp ../Text-parsing-functions/text_parse.cpp \
				 ../FastList/list.cpp 					  \
				 ../ListDump/list_dump.cpp 				  \
				 ../debug/debug.cpp 					  \
				 ../debug/color_print.cpp

OBJECTS=$(SOURCES:.cpp=.o)

EXECUTABLE=WordDevider

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	@$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	@$(CC) $(CFLAGS) $< -o $@

clean:
	@rm -rf ../Text-parsing-functions/*.o WordDevider
	@rm -rf ../ListDump/*.o WordDevider
	@rm -rf ../FastList/*.o WordDevider
	@rm -rf ../debug/*.o WordDevider
	@rm -rf ../*.html WordDevider
	@rm -rf *.o WordDevider
	@rm -rf ../dump_src/*.png WordDevider
	@rm -rf ../dump_src/*.dot WordDevider

run1:
	./WordDevider ./very_large_text.txt ../word_set.txt

run2:
	./WordDevider seek_file.txt ../seek_word_set.txt
