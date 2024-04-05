#include "..\include\differentiator.h"

static FILE* file_error = fopen ("log\\input_output.txt", "w");

static void skip_symbols (int* ptr, char* buffer);

static int add_node_in_graph_1 (struct Node* node, FILE* file_graph, size_t* node_num);

static int add_node_in_graph_2 (struct Node* node, FILE* file_graph);

int get_database (struct Node** root, char* file_input)   // get data of tree in the following file
{
    CALLOC (*root, struct Node, 1);

    FOPEN (file_p, file_input, "rb");
    size_t file_size = file_size_measure (file_p);            // measures the size of a text
    printf ("size of file: %d\n", file_size);

    char* text_data = NULL;
    CALLOC (text_data, char, file_size + 1);

    size_t factual_size = fread (text_data, sizeof (char), file_size, file_p);
    if (factual_size != file_size)
    {
        fprintf (file_error, "<< Error. Wrong size of data_file >>\n");
        return ERROR;
    }
    fclose (file_p);

    construct_data_nodes (*root, text_data, file_size);

    return SUCCESS;
}

int construct_data_nodes (struct Node* root, char* text_data, size_t file_size)
{
    struct Node* prev_node = NULL;
    struct Node* node      = NULL;
    struct stack stk       = {0};

    printf (" DATA_BASE \n");
    stack_ctor (&stk, 2);
    int ptr = 0;
    for (int n = 0, position = ROOT; ptr < file_size; ptr++)
    {
        if (text_data[ptr] == '(' && position == ROOT)  // add a root of the tree
        {
            ptr++;
            get_element (text_data, &ptr, root);

            stack_push (&stk, (void**) &root);
            stack_push (&stk, (void**) &root);
            position = LEFT;
        }
        if (text_data[ptr] == '(')                      // add node in tree
        {
            printf ("{%d}\n", position);
            ptr++;
            CALLOC (node, struct Node, 1);

            get_element (text_data, &ptr, node);

            stack_pop (&stk, (void**) &prev_node);

            if (position == LEFT)                     // add node as a left leaf
                prev_node->left = node;
            else                                      // add node as a right leaf
                prev_node->right = node;
            //ptr++;

            if (text_data[ptr] == '(')                  // next node will be in a left position
            {
                stack_push (&stk, (void**) &node);
                stack_push (&stk, (void**) &node);
                position = LEFT;
            }
            ptr--;
        }
        else if (text_data[ptr] == ')')                 // if a next node will be, it will stay in a right position
        {
            position = RIGHT;
        }
    }
    printf ("\n");
    stack_dtor (&stk);

    return SUCCESS;
}

void skip_symbols (int* ptr, char* buffer)
{
    while (buffer[*ptr] != '#')               // search for a beginning of a new word
        *ptr++;
}

int get_element (char* text_data, int* ptr, struct Node* tree)
{
    if (text_data == NULL || tree == NULL)
        return ERROR;

    char* buffer = (char*) calloc (DATA_SIZE + 1, sizeof (char));
    int i = 0;

    while (text_data[*ptr] != ')' && text_data[*ptr] != '(')
    {
        buffer[i] = text_data[*ptr];
        printf ("[%c]\n", buffer[i]);
        i++;
        *ptr += 1;        //
    }

    if (check_var (buffer) != 0)                             // if element is variable
    {
        tree->type = T_VAR;
        tree->data.var = buffer[0];
        //tree->data.var[1] = '\0';
        printf ("<< %c >>\n", tree->data.var);
        return SUCCESS;
    }
    else
    {
        int x = sscanf (buffer, "%lf", &(tree->data.value));
        if (x == 1)                                          // if element is number
        {
            tree->type = T_NUM;
            free (buffer);
            printf ("<< %lf >>\n", tree->data.value);
            return SUCCESS;
        }
    }
    if (strlen (buffer) == 1)                                //  if element is operand
    {
        tree->data.operand = buffer[0];
        tree->type = T_OP;
        printf ("<< %c >>\n", tree->data.operand);
        free (buffer);
        return SUCCESS;
    }
    else                                                     // if element is long operand (sin, ln etc)
    {
        if (!strcmp (data_buffer, "null"))
        {
            tree->type = DEFUALT;
        }
        else
        {
            tree->type = T_OP_LONG;

            tree->data.operand_long = (char*) calloc (strlen (data_buffer) + 1, sizeof (char));
            strcpy (tree->data.operand_long, data_buffer);
        }
        free (buffer);
        return SUCCESS;
    }
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
    add_node_in_graph_1 (root, file_graph, &node_num);
    add_node_in_graph_2 (root, file_graph);
    fprintf (file_graph, "}");

    fclose (file_graph);

    return SUCCESS;
}

static int add_node_in_graph_1 (struct Node* node, FILE* file_graph, size_t* node_num)
{
    if (node->right == NULL && node->left == NULL)
    {
        if (node->type == T_VAR)
            fprintf (file_graph, " %d [shape = Mrecord, style = filled, fillcolor = YellowGreen, label = \"%c\" ];\n", *node_num, node->data.var);
        else if (node->type == T_OP)
            fprintf (file_graph, " %d [shape = Mrecord, style = filled, fillcolor = YellowGreen, label = \"%c\" ];\n", *node_num, node->data.operand);
        else
             fprintf (file_graph, " %d [shape = Mrecord, style = filled, fillcolor = YellowGreen, label = \"%lf\" ];\n", *node_num, node->data.value);
    }
    else
    {
        if (node->type == T_VAR)
            fprintf (file_graph, " %d [shape = Mrecord, style = filled, fillcolor = Peru, label = \"%c\" ];\n", *node_num, node->data.var);
        else if (node->type == T_OP)
            fprintf (file_graph, " %d [shape = Mrecord, style = filled, fillcolor = Peru, label = \"%c\" ];\n", *node_num, node->data.operand);
        else
             fprintf (file_graph, " %d [shape = Mrecord, style = filled, fillcolor = Peru, label = \"%lf\" ];\n", *node_num, node->data.value);
    }

    node->num_in_tree = *node_num;
    if (node->left != NULL)
    {
        *node_num += 1;
        add_node_in_graph_1 (node->left, file_graph, node_num);
    }

    if (node->right != NULL)
    {
        *node_num += 1;
        add_node_in_graph_1 (node->right, file_graph, node_num);
    }

    return SUCCESS;
}

static int add_node_in_graph_2 (struct Node* node, FILE* file_graph)
{
    if (node->left != NULL)
    {
        fprintf (file_graph, "%d -> %d[ color = Peru ];\n", node->num_in_tree, (node->left)->num_in_tree);
        add_node_in_graph_2 (node->left, file_graph);
    }

    if (node->right != NULL)
    {
        fprintf (file_graph, "%d -> %d[ color = Peru ];\n", node->num_in_tree, (node->right)->num_in_tree);
        add_node_in_graph_2 (node->right, file_graph);
    }

    return SUCCESS;
}

int tree_output (struct Node* node, FILE* file_output)
{
    dump_node (node);
    if (node->type == T_VAR)
        fprintf (file_output, "(%c", node->data.var);
    else if (node->type == T_OP)
        fprintf (file_output, "(%c", node->data.operand);
    else if (node->type == T_NUM)
        fprintf (file_output, "(%lf", node->data.value);

    if (node->left != NULL)
        tree_output (node->left, file_output);

    if (node->right != NULL)
        tree_output (node->right, file_output);

    fprintf (file_output, ")");

    return 0;
}

void dump_node (struct Node *tree)
{
    printf("\n---------------NODE_DUMP-------------\n");
    printf("type - %d\n", tree->type);

    if (tree->type == T_NUM)
        printf("#%lf#", tree->data.value);
    else if (tree->type == T_OP)
        printf("#%c#", tree->data.operand);
    else if (tree->type == T_VAR)
        printf("#%c#", tree->data.var);

    printf("\n---------------DUMP_END--------------\n");
}

void tree_dtor (struct Node* node)
{
    if (node->left != NULL)
        tree_dtor (node->left);

    if (node->right != NULL)
        tree_dtor (node->right);

    free (node);
}


