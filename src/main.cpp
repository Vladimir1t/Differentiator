#include "..\include\differentiator.h"

int main (int argc, char* argv[])
{
    FOPEN (file_sourse, "file_output.txt", "a");

    struct Node* root = NULL;

    if (argc >= 2 && get_database (&root, argv[1]) != SUCCESS)
        return 0;

    //run_differentiator (root);

    //tree_output (root, file_sourse);

    build_graphviz (root);

    return 0;
}
