#include "..\include\differentiator.h"

static FILE* error_file = fopen ("error_file_diff.txt", "w");

static const char file_graph_name[] = "graphviz\\graph_diff.dot";

static Class_operation long_op_determinator (char* operation);

static struct Node* copy_subtree (const struct Node* node);

static struct Node* pow_diff (const struct Node* node, char var);

static void simplifier_conv_of_const (struct Node* tree, int* changed);

static void remove_neutral_elements (struct Node* tree, int* changed);

static void free_subtree (struct Node* tree);

int run_differentiator (struct Node* tree, FILE* file_output)
{
    if (tree == NULL)
        return ERROR;

    simplifier (tree);

    struct Node* diff_tree = diff (tree, 'x');

    simplifier (diff_tree);

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
                    ADD_DIFF (node);
                    break;

                case '-':
                    SUB_DIFF (node);
                    break;

                case '*':
                    MUL_DIFF (node);
                    break;

                case '/':
                    DIV_DIFF (node);

                case '^':
                   return pow_diff (node, var);
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
                    LN_DIFF (node);
                    break;

                case OP_SIN:
                    SIN_DIFF (node);
                    break;

                case OP_COS:
                    COS_DIFF (node);
                    break;

                case OP_EXP:
                    EXP_DIFF (node);
                    break;
            }
            break;

        default:
            return NULL;
            break;
    }
}

struct Node* pow_diff (const struct Node* node, char var)
{
    if (node->right->type == T_NUM)            // f(x) ^ n:  n * f(x) ^ (n - 1) * df(x)
    {
        unsigned char mul = '*', pow = '^';
        double value_1 = node->right->data.value;
        double value_2 = node->right->data.value - 1;

        struct Node* val_1 = create_node (T_NUM, &value_1, NULL, NULL);
        struct Node* val_2 = create_node (T_NUM, &value_2, NULL, NULL);
        struct Node* x = copy_subtree (node->left);
        struct Node* dx = diff (node->left, var);

        return create_node (T_OP, &mul, create_node (T_OP, &mul, val_1,  create_node (T_OP, &pow, x, val_2)), dx);
    }
    else if (node->left->type == T_NUM)       // a ^ f(x):  (ln(a) * a ^ f(x)) * df(x)
    {
        unsigned char mul = '*';
        double value_1 = log (node->left->data.value);

        struct Node* val_1 = create_node (T_NUM, &value_1, NULL, NULL);
        struct Node* x = copy_subtree (node);
        struct Node* dx = diff (node->right, var);

        return create_node (T_OP, &mul, create_node (T_OP, &mul, val_1, x), dx);
    }
    else       // f(x) ^ g(x):  exp(ln(f(x)) * g(x)) * (1 / f(x) * g(x) * df(x) + f(x) * dg(x))
    {
        unsigned char mul = '*';
        char* exp = "exp", *ln = "ln";

        struct Node* value = create_node (T_OP_LONG, exp, create_node (T_OP, &mul, create_node (T_OP_LONG, ln, copy_subtree (node->left), NULL), copy_subtree (node->right)), NULL);

        return diff (value, var);
    }
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
        remove_neutral_elements (tree, &changed);
    }
}

void simplifier_conv_of_const (struct Node* tree, int* changed)
{
    int is_var_in_subtree = 0;
    double value = calculator (tree, &is_var_in_subtree);

    if (is_var_in_subtree == 0 && tree->type != T_NUM && tree->type != DEFUALT)
    {
        tree->data.value = value;
        tree->type = T_NUM;

        SUBTREE_DTOR(tree->left);
        SUBTREE_DTOR(tree->right);
        *changed = 1;
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

void remove_neutral_elements (struct Node* tree, int* changed)
{
    if (tree->data.operation == '*')
    {
        if ((tree->left->type == T_NUM && tree->left->data.value == 0) || (tree->right->type == T_NUM && tree->right->data.value == 0))
        {
            tree->type = T_NUM;
            tree->data.value = 0;

            SUBTREE_DTOR (tree->left);
            SUBTREE_DTOR (tree->right);

            *changed = 1;
        }
        else if (tree->left->type == T_NUM && tree->left->data.value == 1)
        {
            SUBTREE_DTOR (tree->left);
            memcpy (tree, tree->right, sizeof (struct Node));
            *changed = 1;
        }

        else if (tree->right->type == T_NUM && tree->right->data.value == 1)
        {
            SUBTREE_DTOR (tree->right);
            memcpy (tree, tree->left, sizeof (struct Node));
            *changed = 1;
        }
    }
    else if (tree->data.operation == '+')
    {
        if (tree->left->type == T_NUM && tree->left->data.value == 0)
        {
            SUBTREE_DTOR (tree->left);
            memcpy (tree, tree->right, sizeof (struct Node));
            *changed += 1;
        }

        else if (tree->right->type == T_NUM && tree->right->data.value == 0)
        {
            SUBTREE_DTOR (tree->right);
            memcpy (tree, tree->left, sizeof (struct Node));
            *changed = 1;
        }
    }

    if (tree->left != NULL)
        remove_neutral_elements (tree->left, changed);

    if (tree->right != NULL)
        remove_neutral_elements (tree->right, changed);
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
