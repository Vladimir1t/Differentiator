#ifndef DSL_H_INCLUDED
#define DSL_H_INCLUDED

  /* Library of defines */


#define CHECK_PTR(ptr)                              \
    if (ptr == NULL)                                \
    {                                               \
        fprintf (error_file, "<< NULL ptr >>\n");   \
        return NULL_PTR;                            \
    }

#define REALLOC(elem, type, size)                            \
    do                                                       \
    {                                                        \
        void* clTmp = realloc (elem, size * sizeof (type));  \
        if (clTmp == NULL)                                   \
            return REALLOC_ERROR;                            \
        elem = (type*) clTmp;                                \
    }                                                        \
    while (0)

#define CALLOC(elem, type, size)                     \
    do                                               \
    {                                                \
        void* clTmp = calloc (size, sizeof (type));  \
        if (clTmp == NULL)                           \
            return CALLOC_ERROR;                     \
        elem = (type*) clTmp;                        \
    }                                                \
    while (0)

#define FOPEN(ptr, name, mode)       \
    FILE* ptr = fopen (name, mode);  \
    if (ptr == NULL)                 \
        return OPEN_ERROR;

#define NEXT_NODE(next)              \
    do                               \
    {                                \
        CALLOC (next, Node, 1);      \
    }                                \
    while(0)

#define COMPARE_DOUBLES(double_1, double_2)              \
    {                                                    \
        const double EPSILON = 0.00001;                  \
        return (fabs double - double) <= EPSILON);       \
    }

#define ADD_DIFF(node)                                                                     \
    {                                                                                      \
        unsigned char add = '+';                                                           \
        return create_node (T_OP, &add, diff (node->left, var), diff (node->right, var));  \
        break;                                                                             \
    }

#define SUB_DIFF(node)                                                                     \
    {                                                                                      \
        unsigned char sub = '-';                                                           \
        return create_node (T_OP, &sub, diff (node->left, var), diff (node->right, var));  \
        break;                                                                             \
    }

#define MUL_DIFF(node)                                                       \
    {                                                                        \
        unsigned char mul = '*', add = '+';                                  \
                                                                             \
        struct Node* du = diff (node->left, var);                            \
        struct Node* u  = copy_subtree (node->left);                         \
                                                                             \
        struct Node* dv = diff (node->right, var);                           \
        struct Node* v  = copy_subtree (node->right);                        \
                                                                             \
        return create_node (T_OP, &add, create_node (T_OP, &mul, du, v),     \
                                        create_node (T_OP, &mul, u, dv));    \
        break;                                                               \
    }

#define DIV_DIFF(node)                                                                      \
    {                                                                                       \
        unsigned char mul = '*', sub = '-', div = '/';                                      \
                                                                                            \
        struct Node* du = diff (node->left, var);                                           \
        struct Node* u  = copy_subtree (node->left);                                        \
                                                                                            \
        struct Node* dv = diff (node->right, var);                                          \
        struct Node* v  = copy_subtree (node->right);                                       \
                                                                                            \
        struct Node* nominator = create_node (T_OP, &sub, create_node (T_OP, &mul, du, v),  \
                                                          create_node (T_OP, &mul, u, dv)); \
        struct Node* denominator = create_node (T_OP, &mul, copy_subtree (u),               \
                                                            copy_subtree (u));              \
                                                                                            \
        return create_node (T_OP, &div, nominator, denominator);                            \
        break;                                                                              \
    }

#endif // DSL_H_INCLUDED
