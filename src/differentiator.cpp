#include "..\include\differentiator.h"

static FILE* error_file = fopen ("error_file_diff.txt", "w");

static const char file_graph_name[] = "graphviz\\graph_diff.dot";

static struct Node* create_node (class_type type, void* data, struct Node* left, struct Node* right);

static struct Node* copy_subtree (struct Node* node);

int run_differentiator (struct Node* root, FILE* file_output)
{
    if (root == NULL)
        return ERROR;

    struct Node* diff_tree = diff (root);

    build_graphviz (diff_tree, file_graph_name);

    //simplifier (diff_tree);

    tree_dtor (diff_tree);

    return SUCCESS;
}

struct Node* diff (const struct Node* node)
{
    switch (node->type)
    {
        case T_NUM:
        {
            double temp = 0;
            return create_node (T_NUM, &temp, NULL, NULL);
            break;
        }

        case T_VAR:
        {
            double temp = 1;
            return create_node (T_NUM, &temp, NULL, NULL);
        }

        case T_OP:
            switch (node->data.operand)
            {
                case '+':
                {
                    unsigned char add = '+';
                    return create_node (T_OP, &add, diff (node->left), diff (node->right));
                    break;
                }

                case '-':
                {
                    unsigned char sub = '-';
                    return create_node (T_OP, &sub, diff (node->left), diff (node->right));
                    break;
                }

                case '*':
                {
                    unsigned char mul = '*', add = '+';

                    struct Node* du = diff (node->left);
                    struct Node* u  = copy_subtree (node->left);

                    struct Node* dv = diff (node->right);
                    struct Node* v  = copy_subtree (node->right);

                    return create_node (T_OP, &add, create_node (T_OP, &mul, du, v), create_node (T_OP, &mul, u, dv));
                    break;
                }
                case '/':
                {
                    unsigned char mul = '*', sub = '-', div = '/':

                    struct Node* du = diff (node->left);
                    struct Node* u  = copy_subtree (node->left);

                    struct Node* dv = diff (node->right);
                    struct Node* v  = copy_subtree (node->right);

                    struct Node* nominator = create_node (T_OP, &sub, create_node (T_OP, &mul, du, v), create_node (T_OP, &mul, u, dv));
                    struct Node* denominator = create_node (T_OP, &mul, copy_subtree (u), copy_subtree (u));

                    return create_node (T_OP, &div, nominator, denominator);
                    break;
                }
                case '^':
                {
                    unsigned char mul = '*', sub = '-', deg = '^';
                    if (node->left->type == T_NUM)
                    {
                        unsigned char* ln = (char*) "ln";

                        struct Node* x  = copy_subtree (node);
                        struct Node* dx = diff (node->right);
                        struct Node* lna = create_node (T_OP_LONG, ln, NULL, )
                    }


                    struct Node*
                }
            }
    }
    return NULL;
}

struct Node* create_node (class_type type, void* data, struct Node* left, struct Node* right)
{
    struct Node* new_node = (struct Node*) calloc (1, sizeof (struct Node));

    new_node->type = type;
    switch (type)
    {
        case T_NUM:
            new_node->data.value = *(double*) data;
            break;

        case T_OP:
            new_node->data.operand = *(unsigned char*) data;
            break;

        case T_VAR:
            new_node->data.var = *(unsigned char*) data;
            break;
    }

    new_node->left  = left;
    new_node->right = right;

    return new_node;
}

struct Node* copy_subtree (struct Node* node)
{
    if (node == NULL)
        fprintf (error_file, "<< NULL_PTR >>\n");

    struct Node* new_node = (struct Node*) calloc (1, sizeof (struct Node));

    memcpy (new_node, node, sizeof (struct Node));

    if (node->left != NULL)
        new_node->left = copy_subtree (node->left);
    if (node->right != NULL)
        new_node->right = copy_subtree (node->right);

    return new_node;
}

