all: diff
	differentiator.exe file_output.txt
	dot -Tpng graphviz\graph.dot -o graphviz\tree_graph.png
	start graphviz\tree_graph.png

diff: src\main.cpp src\differentiator.cpp stack\src\stack.cpp src\input_output.cpp
	g++ -o differentiator src\main.cpp src\differentiator.cpp stack\src\stack.cpp src\input_output.cpp

