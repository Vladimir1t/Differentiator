#ifndef DIFFERENTIATOR_H_INCLUDED
#define DIFFERENTIATOR_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>

#include "..\library\errors.h"
#include "..\library\library_of_defines.h"
#include "..\stack\include\stack.h"
#include "..\library\commands.h"

const int DATA_SIZE = 20;

const int MAX_OP_SIZE = 4;

enum POSITION
{
    LEFT  = 0,
    RIGHT = 1,
    ROOT  = 2,
};

union node_data
{
    unsigned char operation;       // +, -, * etc
    unsigned char var;             // x, y, z
    double        value;           // double number
    char*         operation_long;  // sin, cos, ln etc
};

struct Node
{
    Class_type   type;
    node_data    data;
    struct Node* left;
    struct Node* right;
    //struct Node* parent;
    int          num_in_tree;
};

static const Node* POISON = NULL;


int get_database (struct Node** root, char* file_input);

int check_var (char* buffer);

size_t file_size_measure (FILE* const file_p);

int build_graphviz (struct Node* root, const char* file_graph_txt);

int tree_output (struct Node* node, FILE* file_output);

void dump_node (struct Node *tree);

void tree_dtor (struct Node* node);

void clean_buffer ();

struct Node* run_differentiator (struct Node* tree, FILE* file_output);

struct Node* create_node (Class_type type, void* data, struct Node* left, struct Node* right);

struct Node* diff (const struct Node* node, char var);

double calculator (struct Node* tree, int* var);

void simplifier (struct Node* tree);

double count_differential_equation (struct Node* root, unsigned char var);

#endif // DIFFERENTIATOR_H_INCLUDED
