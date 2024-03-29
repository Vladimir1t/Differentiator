#ifndef DIFFERENTIATOR_H_INCLUDED
#define DIFFERENTIATOR_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "..\library\errors.h"
#include "..\library\DSL.h"
//#include "..\"

union token_data
{
    unsigned char operation;
    double        value;
};

struct token
{
    int              type;
    union token_data data;
    struct token*    left;
    struct token*    right;
}

enum CLASS_TYPE
{
    T_Nm  = 0,
    T_Vr  = 1,
    T_Op  = 2,
};

enum CLASS_OPERATION
{
    OP_Add = 0,
    OP_SUB = 1,
    OP_MUL = 2,
    OP_DIV = 3,
};

enum CLASS_VARIABLE
{
    VR_X = 0,
    VR_Y = 1,
    VR_Z = 2,
};

const char array_op[] =
{
    "+",
    "-",
    "*",
    ":",
};

const char array_vr[] =
{
    "x",
    "y",
    "z",
};

#endif // DIFFERENTIATOR_H_INCLUDED
