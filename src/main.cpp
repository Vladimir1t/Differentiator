#include "..\inlude\differentiator.h"

int main (int argc, char argv[])
{
    if (argc >= 2)
        FOPEN (file_sourse, "mathTree.txt", "rb");

    //struct token node = {0};
    //bulid_tree (&node, file_sourse);
    struct token n1 = {T_NM, {3}, NULL, NULL};
    struct token n2 = {T_NM, {5}, NULL, NULL};
    struct token n3 = {T_OP, {0}, &n1, &n2};

    tree_output (node);

    build_graphviz (node);

    return 0
}
