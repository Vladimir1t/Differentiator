# Differentiator

## Table of Contents
- [Overview](#overview)
- [Features](#features)
- [Files Included](#files-included)
- [Usage](#usage)
- [Dependencies](#dependencies)
- [Functions](#functions)
- [Example](#example)
  

## Overview
Differentiator is a c program designed to perform symbolic differentiation on mathematical expressions represented as trees. It utilizes a recursive algorithm to traverse the expression tree and apply the rules of calculus to compute the derivative with respect to a given variable.

## Features
- Symbolic differentiation of mathematical expressions
- Supports basic arithmetic operations: addition, subtraction, multiplication, division
- Handles advanced mathematical functions: logarithm, sine, cosine, exponential, tangent
- Generates Graphviz visualization of expression trees
- Simplification of expression trees
- Input from text files containing mathematical expressions

## Files
1. **main.cpp**: Entry point of the program. It includes the necessary headers and orchestrates the process of differentiation.
2. **differentiator.cpp**: Contains the core logic for symbolic differentiation and related operations such as simplification and substitution.
3. **input_output.cpp**: Handles input/output operations, including reading mathematical expressions from files and generating Graphviz visualizations.
4. **differentiator.h**: This header file contains declarations for functions and data structures used in the Differentiator program. It includes definitions for the expression tree node structure, function prototypes for various operations such as parsing expressions, building expression trees, differentiation, simplification, and outputting results.
5. **commands.h**: This header file defines enums and data structures related to mathematical operations and variables. It contains definitions for different types of expression nodes (numbers, variables, operations, etc.) and the corresponding operations (addition, subtraction, multiplication, etc.). Additionally, it defines arrays for supported operations and variables.
6. **library_of_defines.h**: This header file provides a library of macros used throughout the Differentiator program. It includes macros for error handling, memory allocation, file operations, and mathematical operations like differentiation rules for various functions (e.g., sin, cos, ln).

## Usage
1. **Input**: Provide a text file containing the mathematical expression you want to differentiate.
2. **Output**: The program generates a Graphviz visualization of the expression tree and computes the derivative.
3. **Visualization**: Optionally, you can visualize the expression tree using Graphviz.
4. **Result**: The resulting expression tree of the derivative is stored in a text file.


## Installation
1. Clone the repository to your local machine.
2. Compile the source files using a C++ compiler.
3. Ensure Graphviz is installed if you want to visualize expression trees.

## Dependencies
- C++ Compiler
- Graphviz (optional for visualization)

## Functions

### 1. `main.cpp`
- `main`: The main function responsible for orchestrating the program's execution. It reads input expressions from a file, builds expression trees, performs differentiation, and outputs the results to a file. Additionally, it provides options to visualize the expression trees and their derivatives using Graphviz.

### 2. `differentiator.cpp`
- `run_differentiator`: Performs symbolic differentiation on an expression tree and returns the resulting differentiated tree.
- `calculator`: Evaluates the value of an expression tree, optionally determining if a variable is present in the subtree.
- `diff`: Recursively calculates the derivative of an expression tree node.
- `simplifier`: Simplifies an expression tree by removing neutral elements and converting constant subtrees into single nodes.
- `count_differential_equation`: Calculates the value of a differential equation given a variable assignment.

### 3. `input_output.cpp`
- `get_database`: Reads expressions from a file and constructs the corresponding expression tree.
- `build_graphviz`: Generates a Graphviz DOT file representing an expression tree for visualization.
- `tree_output`: Outputs the expression represented by an expression tree to a file.
- Various helper functions for parsing expressions and handling input/output operations.

## Example
Suppose you have a file named `input.txt` containing the expression:
```
f(x) = x * 2 + x * (cos(x)) ^ ((3) + tg(x / 3))

```
Running the program with this input will compute the derivative of `f(x)` with respect to `x` and generate a visualization of the expression tree.

---

