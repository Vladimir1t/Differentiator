#include "..\include\differentiator.h"

int main (int argc, char* argv[])
{
    FOPEN (file_output, "file_output.txt", "w");
    struct Node* root = NULL;

    if (argc < 2 && get_database (&root, argv[1]) != SUCCESS)
        return 0;

    const char file[] =  "graphviz\\graph.dot";

    build_graphviz (root, file);

    //tree_output (root, file_sourse);

    run_differentiator (root, file_output);

    return 0;
}
