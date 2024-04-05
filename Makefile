all: diff
	differentiator.exe file_input.txt
	
diff: src\main.cpp src\differentiator.cpp stack\src\stack.cpp src\input_output.cpp
	g++ -o differentiator src\main.cpp src\differentiator.cpp stack\src\stack.cpp src\input_output.cpp

