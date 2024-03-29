all: diff
	diff.exe
diff: src\main.cpp src\differentiator.cpp stack\stack.cpp input_output.cpp
	g++ -o diff src\main.cpp src\differentiator.cpp stack\stack.cpp input_output.cpp

