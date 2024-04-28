CC = g++
FLAGS = -Wshadow -Winit-self -Wredundant-decls -Wcast-align -Wundef -Wfloat-equal -Winline -Wunreachable-code\
	-Wmissing-declarations -Wmissing-include-dirs -Wswitch-enum -Wswitch-default\
	-Weffc++ -Wmain -Wextra -Wall -g -pipe -fexceptions -Wcast-qual\
	-Wconversion -Wctor-dtor-privacy -Wempty-body -Wformat-security -Wformat=2\
	-Wignored-qualifiers -Wlogical-op -Wmissing-field-initializers\
	-Wnon-virtual-dtor -Woverloaded-virtual -Wpointer-arith -Wsign-promo\
	-Wstack-usage=8192 -Wstrict-aliasing -Wstrict-null-sentinel -Wtype-limits\
	-Wwrite-strings -D_DEBUG -D_EJUDGE_CLIENT_SIDE

all: diff
	differentiator.exe file_input.txt

diff: src\main.o src\differentiator.o stack\src\stack.o src\input_output.o
	$(CC) -o differentiator main.o differentiator.o stack.o input_output.o

src\main.o: src\main.cpp
	$(CC) -c src\main.cpp $(FLAGS)

src\differentiator.o: src\differentiator.cpp
	$(CC) -c src\differentiator.cpp $(FLAGS)

stack\src\stack.o: stack\src\stack.cpp
	$(CC) -c stack\src\stack.cpp $(FLAGS)

src\input_output.o:  src\input_output.cpp
	$(CC) -c src\input_output.cpp $(FLAGS)


