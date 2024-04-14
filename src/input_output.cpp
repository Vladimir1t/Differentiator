#include "..\include\differentiator.h"

static FILE* file_error = fopen ("log\\input_output_error.txt", "w");

static struct Node* get_g (const char* text_data);

static struct Node* get_e (char** ptr);

static struct Node* get_t (char** ptr);

static struct Node* get_d (char** ptr);

static struct Node* get_p (char** ptr);

static struct Node* get_f (char** ptr);

static struct Node* get_n (char** ptr);

static struct Node* syntax_error ();

static void skip_symbols (int* ptr, char* buffer);

static int add_node_in_graph (struct Node* node, FILE* file_graph, size_t* node_num);

static int add_connection_in_graph (struct Node* node, FILE* file_graph);

int get_database (struct Node** root, char* file_input)       // get data of tree in the following file
{
    CALLOC (*root, struct Node, 1);

    FOPEN (file_p, file_input, "rb");
    size_t file_size = file_size_measure (file_p);            // measures the size of a text
    //printf ("size of file: %d\n", file_size);

    char* text_data = NULL;
    CALLOC (text_data, char, file_size + 1);

    size_t factual_size = fread (text_data, sizeof (char), file_size, file_p);
    if (factual_size != file_size)
    {
        fprintf (file_error, "<< Error. Wrong size of data_file >>\n");
        return ERROR;
    }
    fclose (file_p);
    text_data[file_size] = '$';

    *root = get_g (text_data);

    if (root == NULL)
        return ERROR;
    //construct_data_nodes (*root, text_data, file_size);

    return SUCCESS;
}


struct Node* get_g (const char* text_data)
{
    //printf ("G\n");
    char* ptr = (char*) text_data;
    struct Node* value = get_e (&ptr);

    while (*ptr == '\n' || *ptr == '\r')
        ptr += 1;

    if (*ptr == '$')
        ptr += 1;

    else
    {
        syntax_error ();
        return NULL;
    }

    return value;
}

struct Node* get_e (char** ptr)    //  + or -
{
    //printf ("E\n");
    struct Node* value_1 = get_t (ptr);
    while (**ptr == '+' || **ptr == '-')
    {
        const char op = **ptr;
        *ptr += 1;
        struct Node* value_2 = get_t (ptr);

        if (op == '+')
        {
            unsigned char add = '+';
            value_1 = create_node (T_OP, &add, value_1, value_2);
        }

        else if (op == '-')
        {
            unsigned char sub = '-';
            value_1 = create_node (T_OP, &sub, value_1, value_2);
        }
    }

    return value_1;
}

struct Node* get_t (char** ptr)   //   * or /
{
    //printf ("T\n");
    struct Node* value_1 = get_p (ptr);
    while (**ptr == '*' || **ptr == '/')
    {
        const char op = **ptr;
        *ptr += 1;
        struct Node* value_2 = get_p (ptr);

        if (op == '*')
        {
            unsigned char mul = '*';
            value_1 = create_node (T_OP, &mul, value_1, value_2);
        }

        else if (op == '/')
        {
            unsigned char div = '/';
            value_1 = create_node (T_OP, &div, value_1, value_2);
        }
    }

    return value_1;
}

struct Node* get_p (char** ptr)   //  (, )  and determine functions
{
    //printf ("P\n");
    struct Node* value = NULL;

    if (**ptr == '(')
    {
        *ptr += 1;
        value = get_e (ptr);

        if (**ptr == ')')
        {
            *ptr += 1;
            if (**ptr == '^')
            {
                *ptr += 1;
                unsigned char pow = '^';
                struct Node* value_2 = get_p (ptr);
                value = create_node (T_OP, &pow, value, value_2);
            }
            return value;
        }
    }
    else if (**ptr == 'l' && *(*ptr + 1) == 'n')
        value = get_d (ptr);

    else if (isalpha (**ptr) && isalpha (*(*ptr + 1))  && isalpha (*(*ptr + 2)))
        value = get_f (ptr);

    else
        value = get_n (ptr);

    return value;
}

struct Node* get_f (char** ptr)    //  cos, sin, exp
{
    //printf ("F\n");
    struct Node* value_1 = NULL;
    char op = **ptr;
    *ptr += 3;

    if (**ptr == '(')
    {
        *ptr += 1;
        if (op =='c')
        {
            char* cos = "cos";
            value_1 = get_e (ptr);
            value_1 = create_node (T_OP_LONG, cos, value_1, NULL);
        }
        else if (op == 's')
        {
            char* sin = "sin";
            value_1 = get_e (ptr);
            value_1 = create_node (T_OP_LONG, sin, value_1, NULL);
        }
        else if (op == 'e')
        {
            char* exp = "exp";
            value_1 = get_e (ptr);
            value_1 = create_node (T_OP_LONG, exp, value_1, NULL);
        }
        else
        {
            syntax_error ();
            return NULL;
        }
        if (**ptr == ')')
            *ptr += 1;
    }
    return value_1;
}

struct Node* get_d (char** ptr)   //   ln
{
    //printf ("D\n");
    *ptr += 2;
    struct Node* value_1 = NULL;

    if (**ptr == '(')
    {
        *ptr += 1;
        char* ln = "ln";
        value_1 = get_e (ptr);
        value_1 = create_node (T_OP_LONG, ln, value_1, NULL);
        if (**ptr == ')')
        {
            *ptr += 1;
        }
    }

    return value_1;
}

struct Node* get_n (char** ptr)   //determine numbers and variables
{
    //printf ("N\n");
    const char* old_ptr = *ptr;

    struct Node* val = (struct Node*) calloc (1, sizeof (struct Node));

    if (isalpha (**ptr))
    {
        val->type = T_VAR;
        val->data.var = **ptr;
        *ptr += 1;
    }
    else
    {
        val->type = T_NUM;

        char sign = '+';
        if (**ptr == '-')
            sign = '-';

        while ('0'<= **ptr && **ptr <= '9')
        {
            if (sign == '+')
                val->data.value = val->data.value * 10 + (**ptr - '0');
            else
                val->data.value = val->data.value * 10 - (**ptr - '0');
            *ptr += 1;
        }
    }

    if (*ptr == old_ptr)
    {
        syntax_error ();
        return NULL;
    }

    return val;
}

struct Node* syntax_error ()
{
    printf ("Syntax Error\n");

    return NULL;
}

struct Node* create_node (Class_type type, void* data, struct Node* left, struct Node* right)
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

        default:
            return NULL;
    }

    new_node->left  = left;
    new_node->right = right;

    return new_node;
}

int check_var (char* buffer)
{
    if (buffer == NULL)
        return ERROR;

    for (int i = 0; i < VAR_NUM; i++)
    {
        if (buffer[0] == array_vr[i].name)
            return 1;
    }
    return 0;
}

size_t file_size_measure (FILE* const file_p)
{
    assert (file_p != NULL);

    int start_position = ftell (file_p);
    fseek (file_p, 0, SEEK_END);
    size_t len = (size_t) ftell (file_p);

    fseek (file_p, start_position, SEEK_SET);

    return len;
}


int build_graphviz (struct Node* root, const char* file_name)
{
    size_t node_num = 0;
    if (root == NULL)
        return ERROR;

    FOPEN (file_graph, file_name, "w");

    fprintf (file_graph, "digraph DIFFERENTIATOR{\n"
                         "label = < DIFFERENTIATOR >;\n"
                         "bgcolor = \"#BAF0EC\";\n"
                         "node [shape = record ];\n"
                         "edge [style = filled ];\n");

        /*   create a tree in graphviz   */
    add_node_in_graph (root, file_graph, &node_num);
    add_connection_in_graph (root, file_graph);
    fprintf (file_graph, "}");

    fclose (file_graph);

    return SUCCESS;
}

static int add_node_in_graph (struct Node* node, FILE* file_graph, size_t* node_num)
{
    if (node->right == NULL && node->left == NULL)
    {
        if (node->type == T_VAR)
            fprintf (file_graph, " %d [shape = Mrecord, style = filled, fillcolor = YellowGreen, label = \"%c\" ];\n", *node_num, node->data.var);

        else if (node->type == T_NUM)
            fprintf (file_graph, " %d [shape = Mrecord, style = filled, fillcolor = YellowGreen, label = \"%lf\" ];\n", *node_num, node->data.value);
    }
    else
    {
        if (node->type == T_OP)
            fprintf (file_graph, " %d [shape = Mrecord, style = filled, fillcolor = Peru, label = \"%c\" ];\n", *node_num, node->data.operation);

        else if (node->type == T_OP_LONG)
            fprintf (file_graph, " %d [shape = Mrecord, style = filled, fillcolor = Peru, label = \"%s\" ];\n", *node_num, node->data.operation_long);
    }

    node->num_in_tree = *node_num;
    if (node->left != NULL)
    {
        *node_num += 1;
        add_node_in_graph (node->left, file_graph, node_num);
    }

    if (node->right != NULL)
    {
        *node_num += 1;
        add_node_in_graph (node->right, file_graph, node_num);
    }

    return SUCCESS;
}

static int add_connection_in_graph (struct Node* node, FILE* file_graph)
{
    if (node->left != NULL)
    {
        fprintf (file_graph, "%d -> %d[ color = Peru ];\n", node->num_in_tree, (node->left)->num_in_tree);
        add_connection_in_graph (node->left, file_graph);
    }

    if (node->right != NULL && node->right->type != DEFUALT)
    {
        fprintf (file_graph, "%d -> %d[ color = Peru ];\n", node->num_in_tree, (node->right)->num_in_tree);
        add_connection_in_graph (node->right, file_graph);
    }

    return SUCCESS;
}

int tree_output (struct Node* node, FILE* file_output)
{
    if (node->left != NULL)
        tree_output (node->left, file_output);

    //dump_node (node);
    if (node->type == T_VAR)
        fprintf (file_output, "( %c ", node->data.var);
    else if (node->type == T_OP)
        fprintf (file_output, " %c ", node->data.operation);
    else if (node->type == T_NUM)
        fprintf (file_output, "( %lf ", node->data.value);

    fprintf (file_output, ")");

    if (node->right != NULL)
        tree_output (node->right, file_output);

    return 0;
}

void dump_node (struct Node *tree)
{
    printf ("\n---------------NODE-------------\n");
    printf ("type - %d\n", tree->type);

    if (tree->type == T_NUM)
        printf ("# %lf #", tree->data.value);
    else if (tree->type == T_OP)
        printf ("# %c #", tree->data.operation);
    else if (tree->type == T_VAR)
        printf ("# %c #", tree->data.var);

    printf ("\n--------------------------------\n");
}

void tree_dtor (struct Node* node)
{
    if (node->left != NULL)
        tree_dtor (node->left);

    if (node->right != NULL)
        tree_dtor (node->right);

    free (node);
}

void clean_buffer ()
{
    int symbol = 0;
    do
    {
        symbol = getchar ();
    }
    while (symbol != '\n' && symbol != EOF);
}


