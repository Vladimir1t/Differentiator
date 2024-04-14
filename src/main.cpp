#include "..\include\differentiator.h"

int main (int argc, char* argv[])
{
    FOPEN (file_output, "file_output.txt", "w");
    struct Node* root = NULL;
    printf ("\n-- DIFFERENTIATOR --\n\n");
    if (get_database (&root, argv[1]) != SUCCESS)
        return 0;

    const char file[] = "graphviz\\graph.dot";

    simplifier (root);
    build_graphviz (root, file);
    char choice = '\0';
    while (choice != 'n' && choice != 'y')
    {
        printf ("Do you want to print data_tree?\n"
                "( y, n )\n");
        scanf ("%c", &choice);
        if (choice == 'y')
        {
            system ("dot -Tpng graphviz\\graph.dot -o graphviz\\tree_graph.png");
            system ("start graphviz\\tree_graph.png");
        }
        clean_buffer ();
    }

    simplifier (root);

   // run_differentiator (root, file_output);

    return 0;
}



