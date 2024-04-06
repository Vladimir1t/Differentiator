#include "..\include\differentiator.h"

static FILE* error_file = fopen ("error_file_diff.txt", "w");

static const char file_graph_name[] = "graphviz\\graph_diff.dot";

static CLASS_OPERATION long_op_det (char* operation);

static struct Node* create_node (class_type type, void* data, struct Node* left, struct Node* right);

static struct Node* copy_subtree (struct Node* node);

static void simplifier_1 (struct Node* tree, int* changed);

static void free_subtree (struct Node* tree);


#define SUBTREE_DTOR(node)      \
    do                          \
    {                           \
        free(node);             \
        node = NULL;            \
    }                           \
    while (0)


int run_differentiator (struct Node* root, FILE* file_output)
{
    if (root == NULL)
        return ERROR;

    simplifier (root);

    struct Node* diff_tree = diff (root, 'x');

    simplifier (diff_tree);

    build_graphviz (diff_tree, file_graph_name);

    char choice = '\0';
    while (choice != 'n')
    {
        clean_buffer ();
        printf ("do you want to print diff_tree?\n"
                "( y, n )\n");
        scanf ("%c", &choice);
        if (choice == 'y')
        {
            system ("dot -Tpng graphviz\\graph_diff.dot -o graphviz\\tree_graph_diff.png");
            system ("start graphviz\\tree_graph_diff.png");
            break;
        }
    }

    tree_dtor (diff_tree);

    return SUCCESS;
}

double calculator (struct Node* tree, int* var)
{
    if (tree == NULL)
        return ERROR;

    if (tree->type == T_VAR)  // if there is a variable in subtree
        *var = 1;

    if (var != NULL && *var == 1)
        return 0;

    if (tree->type == T_OP)
    {
        switch (tree->data.operation)
        {
            case '^':
                return pow (calculator (tree->left, var), calculator (tree->right, var));
                break;

            case '+':
                return calculator (tree->left, var) + calculator (tree->right, var);
                break;

            case '-':
                return calculator (tree->left, var) - calculator (tree->right, var);
                break;

            case '*':
                return calculator (tree->left, var) * calculator (tree->right, var);
                break;

            case '/':
                return calculator (tree->left, var) / calculator (tree->right, var);
                break;
        }

    }
    else if (tree->type == T_OP_LONG)
    {
        switch (long_op_det (tree->data.operation_long))
        {
            case OP_LN:
                return log(calculator (tree->right, var));
                break;

            case OP_SIN:
                return sin(calculator (tree->right, var));
                break;

            case OP_EXP:
                return exp(calculator (tree->right, var));
                break;

            case OP_COS:
                return cos(calculator (tree->right, var));
                break;
        }
    }
    return tree->data.value;
}

CLASS_OPERATION long_op_det (char* operation)
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
            double temp = 0;
            return create_node (T_NUM, &temp, NULL, NULL);
            break;
        }

        case T_VAR:
        {
            if (node->type == var)
            {
                double temp = 1;
                return create_node (T_NUM, &temp, NULL, NULL);
            }
        }

        case T_OP:
            switch (node->data.operation)
            {
                case '+':
                ADD_DIFF (node);

                case '-':
                SUB_DIFF (node);

                case '*':
                MUL_DIFF (node);

                case '/':
                DIV_DIFF (node);

               /* case '^':
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
                }  */

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
            new_node->data.operation = *(unsigned char*) data;
            break;

        case T_VAR:
            new_node->data.var = *(unsigned char*) data;
            break;

        case T_OP_LONG:
            new_node->data.operation_long = (char*) calloc (MAX_OP_SIZE, sizeof (char));
            strcpy (new_node->data.operation_long, (char*) data);
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

void simplifier (struct Node* tree)
{
    int changed = 1;
    while (changed > 0)
    {
        changed = 0;
        simplifier_1 (tree, &changed);
        //simplifier_2 (tree, &changed);
    }
}

void simplifier_1 (struct Node* tree, int* changed)
{
    int var = 0;
    double temp = calculator (tree, &var);

    if (var == 0 && tree->type != T_NUM && tree->type != DEFUALT)
    {
        tree->data.value = temp;
        tree->type = T_NUM;

        SUBTREE_DTOR(tree->left);
        SUBTREE_DTOR(tree->right);
        *changed += 1;
    }
    if (tree->left != NULL)
    {
        simplifier_1 (tree->left, changed);
    }
    if (tree->right != NULL)
    {
        simplifier_1 (tree->right, changed);
    }
}

/*void simplifier_2 (struct Node* tree)
{
    if (tree->type == T_OP)
    {
        if (!strcmp (tree->data.operation, "*"))
        {
            if ()
            else if ()
        }
    }
}    */

void free_subtree (struct   Node* tree)
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
