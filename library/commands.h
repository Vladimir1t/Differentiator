#ifndef COMMANDS_H_INCLUDED
#define COMMANDS_H_INCLUDED

enum class_type
{
    T_NUM  = 0,
    T_VAR  = 1,
    T_OP   = 2,
};

enum CLASS_OPERATION
{
    OP_ADD = 1,
    OP_SUB = 2,
    OP_MUL = 3,
    OP_DIV = 4,
    OP_SIN = 5,
    OP_COS = 6,
    OP_TG  = 7,
    OP_CTG = 8,
    OP_DEG = 9,
};

struct operation
{
   char name[4];
   int  code;
};

static struct operation array_op[] =
{
    {"+",   OP_ADD},
    {"-",   OP_SUB},
    {"*",   OP_MUL},
    {"/",   OP_DIV},
    {"sin", OP_SIN},
    {"cos", OP_COS},
    {"tg",  OP_TG},
    {"ctg", OP_CTG},
    {"^",   OP_DEG}
};

const int OP_NUM = sizeof (array_op) / sizeof (array_op[0]);

struct variable
{
    char          name;
    unsigned char code;
};

static struct variable array_vr[] =
{
    {'x', 1},
    {'y', 2},
    {'z', 3},
};

#endif // COMMANDS_H_INCLUDED
