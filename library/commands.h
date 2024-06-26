#ifndef COMMANDS_H_INCLUDED
#define COMMANDS_H_INCLUDED

enum Class_type
{
    T_NUM     =  0,
    T_VAR     =  1,
    T_OP      =  2,
    T_OP_LONG =  3,
    DEFUALT   = -1,
};

enum Class_operation
{
    OP_ADD = 1,
    OP_SUB = 2,
    OP_MUL = 3,
    OP_DIV = 4,
    OP_SIN = 5,
    OP_COS = 6,
    OP_TG  = 7,
    OP_CTG = 8,
    OP_LN  = 9,
    OP_SH  = 10,
    OP_CH  = 11,
    OP_DEG = 12,
    OP_EXP = 13
};

struct operation
{
   char name[4];
   int  code;
};

static struct operation array_op[] =
{
    {"sin", OP_SIN},
    {"cos", OP_COS},
    {"tg",  OP_TG},
    {"ctg", OP_CTG},
    {"ln",  OP_LN},
    {"sh",  OP_SH},
    {"ch",  OP_CH},
    {"^",   OP_DEG},
    {"exp", OP_EXP},
    {"+",   OP_ADD},
    {"-",   OP_SUB},
    {"*",   OP_MUL},
    {"/",   OP_DIV},
};

const int OP_NUM = sizeof (array_op) / sizeof (array_op[0]);

struct variable
{
    char   name;
    double value;
};

static struct variable array_vr[] =
{
    {'x', 0},
    {'y', 0},
    {'z', 0},
};

const int VAR_NUM = sizeof (array_vr) / sizeof (array_vr[0]);

#endif // COMMANDS_H_INCLUDED
