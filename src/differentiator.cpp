#include "..\include\differentiator.h"

static FILE* error_file = fopen ("error_file_diff.txt", "w");

static const char file_graph_name[] = "graphviz\\graph_diff.dot";

static Class_operation long_op_determinator (char* operation);

static struct Node* copy_subtree (const struct Node* node);

static void simplifier_conv_of_const (struct Node* tree, int* changed);

static void free_subtree (struct Node* tree);


#define SUBTREE_DTOR(node)      \
    do                          \
    {                           \
        free(node);             \
        node = NULL;            \
    }                           \
    while (0)


int run_differentiator (struct Node* tree, FILE* file_output)
{
    if (tree == NULL)
        return ERROR;
   // tree_output (tree, file_output);

    //simplifier (tree);

    struct Node* diff_tree = diff (tree, 'x');

     char choice = '\0';
    while (choice != 'n')
    {
        printf ("do you want to print diff_tree?\n"
                "( y, n )\n");
        scanf ("%c", &choice);
        if (choice == 'y')
        {
            build_graphviz (diff_tree, file_graph_name);
            system ("dot -Tpng graphviz\\graph_diff.dot -o graphviz\\tree_graph_diff.png");
            system ("start graphviz\\tree_graph_diff.png");
            clean_buffer ();
            break;
        }
        clean_buffer ();
    }

    simplifier (diff_tree);

   // tree_output (diff_tree, file_output);

    //char choice = '\0';
    while (choice != 'n')
    {
        printf ("do you want to print diff_tree?\n"
                "( y, n )\n");
        scanf ("%c", &choice);
        if (choice == 'y')
        {
            build_graphviz (diff_tree, file_graph_name);
            system ("dot -Tpng graphviz\\graph_diff.dot -o graphviz\\tree_graph_diff.png");
            system ("start graphviz\\tree_graph_diff.png");
            break;
        }
        clean_buffer ();
    }

    tree_dtor (diff_tree);

    return SUCCESS;
}

double calculator (struct Node* tree, int* is_var_in_subtree)
{
    if (tree == NULL)
        return ERROR;

    if (tree->type == T_VAR)  // if there is a variable in subtree
        *is_var_in_subtree = 1;

    if (*is_var_in_subtree == 1)
        return 0;

    if (tree->type == T_OP)
    {
        switch (tree->data.operation)
        {
            case '^':
                return pow (calculator (tree->left, is_var_in_subtree), calculator (tree->right, is_var_in_subtree));
                break;

            case '+':
                return calculator (tree->left, is_var_in_subtree) + calculator (tree->right, is_var_in_subtree);
                break;

            case '-':
                return calculator (tree->left, is_var_in_subtree) - calculator (tree->right, is_var_in_subtree);
                break;

            case '*':
                return calculator (tree->left, is_var_in_subtree) * calculator (tree->right, is_var_in_subtree);
                break;

            case '/':
                return calculator (tree->left, is_var_in_subtree) / calculator (tree->right, is_var_in_subtree);
                break;

            default:
                return ERROR;
                break;
        }
    }
    else if (tree->type == T_OP_LONG)
    {
        switch (long_op_determinator (tree->data.operation_long))
        {
            case OP_LN:
                return log (calculator (tree->left, is_var_in_subtree));
                break;

            case OP_SIN:
                return sin (calculator (tree->left, is_var_in_subtree));
                break;

            case OP_EXP:
                return exp (calculator (tree->left, is_var_in_subtree));
                break;

            case OP_COS:
                return cos (calculator (tree->left, is_var_in_subtree));
                break;

            case OP_TG:
                return tan (calculator (tree->left, is_var_in_subtree));

            default:
                return ERROR;
                break;
        }
    }
    /*if (tree->type == T_VAR)
        *is_var_in_subtree = 1;

    if (is_var_in_subtree != NULL && *is_var_in_subtree == 1)
        return 10;
*/
    return tree->data.value;
}

Class_operation long_op_determinator (char* operation)
{
    if (!strcmp (operation, "ln"))
        return OP_LN;

    else if (!strcmp (operation, "sin"))
        return OP_SIN;

    else if (!strcmp (operation, "exp"))
        return OP_EXP;

    else if (!strcmp (operation, "cos"))
        return OP_COS;

    else if (!strcmp (operation, "sin"))
        return OP_SIN;

    else if (!strcmp (operation, "tg"))
        return OP_TG;
}

struct Node* diff (const struct Node* node, char var)
{
    switch (node->type)
    {
        case T_NUM:
            {
                double value = 0;
                return create_node (T_NUM, &value, NULL, NULL);
                break;
            }

        case T_VAR:
            {
                double value = 1;
                return create_node (T_NUM, &value, NULL, NULL);
                break;
            }

        case T_OP:
            switch (node->data.operation)
            {
                case '+':
                    //printf ("add\n");
                    ADD_DIFF (node);

                case '-':
                    SUB_DIFF (node);

                case '*':
                    MUL_DIFF (node);

                case '/':
                    DIV_DIFF (node);

                case '^':
                   // return pow_diff (node);
                    break;

                default:
                    return NULL;
                    break;
            }
            break;

        case T_OP_LONG:
            switch (long_op_determinator (node->data.operation_long))
            {
                case OP_LN:
                    {
                        printf ("diff ln\n");
                        unsigned char mul = '*', div = '/';
                        double value = 1;
                        struct Node* val = copy_subtree (node->left);
                        struct Node* dln = create_node (T_OP, &div,  create_node (T_NUM, &value, NULL, NULL), val);
                        return create_node (T_OP, &mul, dln, diff (node->left, var));
                        break;
                    }

                case OP_SIN:
                    {
                        printf ("diff sin\n");
                        unsigned char mul = '*';
                        char* cos = "cos";
                        struct Node* val = copy_subtree (node->left);
                        struct Node* dsin = create_node (T_OP_LONG, cos, val, NULL);
                        return create_node (T_OP, &mul, dsin, diff (node->left, var));
                        break;
                    }

                case OP_COS:
                    {
                        printf ("diff cos\n");
                        unsigned char mul = '*', sub = '-';
                        double value = 0;
                        char* sin = "sin";

                        struct Node* val_1 = copy_subtree (node->left);
                        struct Node* val_2 = create_node (T_OP_LONG, sin, val_1, NULL);
                        struct Node* dcos = create_node (T_OP, &mul, val_2, diff (node->left, var));
                        return create_node (T_OP, &sub, create_node (T_NUM, &value, NULL, NULL), dcos);
                        break;
                    }

                case OP_EXP:
                    {
                        unsigned char mul = '*';
                        Node* val = copy_subtree (node);
                        Node* res = create_node (T_OP, &mul, val, diff (node->left, var));
                        return res;
                        break;
                    }

            }
            break;

        default:
            return NULL;
            break;
    }
}

struct Node* pow_diff (const struct Node* node)
{

}

struct Node* copy_subtree (const struct Node* node)
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

void simplifier (struct Node* tree)
{
    int changed = 1;
    while (changed > 0)
    {
        changed = 0;
        simplifier_conv_of_const (tree, &changed);
    }
}

void simplifier_conv_of_const (struct Node* tree, int* changed)
{
    int var = 0;
    double value = calculator (tree, &var);

    if (var == 0 && tree->type != T_NUM && tree->type != DEFUALT)
    {
        tree->data.value = value;
        tree->type = T_NUM;

        SUBTREE_DTOR(tree->left);
        SUBTREE_DTOR(tree->right);
        *changed += 1;
    }
    if (tree->left != NULL)
    {
        simplifier_conv_of_const (tree->left, changed);
    }
    if (tree->right != NULL)
    {
        simplifier_conv_of_const (tree->right, changed);
    }
}

void free_subtree (struct  Node* tree)
{
    if (tree->left != NULL)
        free_subtree (tree->left);

    if (tree->right != NULL)
        free_subtree (tree->right);

    tree->left = NULL;
    tree->right = NULL;
    free (tree);
    tree = NULL;
}
