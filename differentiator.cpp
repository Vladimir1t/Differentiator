#include <stdio.h>
#include <stdlib.h>
#include <math.h>

union element
{
    unsigned char operation;
    double        value;
};

struct Node
{
    int           type;
    union element elem;
    struct Node*  left;
    struct Node*  right;
}

int node_insert ()
{
    struct Node* node_1 = NULL;
    CALLOC (node_1, struct Node, 1)

    node_1->type = 2;
    node_2->elem.double = 100;

    return SUCCESS;
}
