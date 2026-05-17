# Custom C++ Bytecode Interpreter & Compiler

A lightweight, stack-based custom programming language compiler and virtual machine (VM) written entirely in C++ from scratch. It features its own lexer, recursive-descent parser, bytecode compiler, and runtime interpreter.

## Features

The language supports a variety of core programming constructs:
* **Data Types**: Integers and Booleans (`true`, `false`).
* **Variables**: Dynamic assignment and state storage.
* **Arithmetic Operations**: Addition (`+`), Subtraction (`-`), Multiplication (`*`), Division (`/`).
* **Comparisons**: `>`, `<`, `>=`, `<=`, `==`, `!=`.
* **Control Flow**:
    * `if` / `else` statements
    * `while` loops
    * Block scopes (`{ ... }`)
* **I/O Built-ins**:
    * `print(value)`: Outputs a value to standard output.
    * `input()`: Reads an integer from standard input.

## Architecture Pipeline

The project is structured into four distinct sequential phases, separated into modular header files:

1.  **Lexical Analysis (`lexer.h`)**
    Reads the raw source string and transforms it into a sequence of `Token`s. It safely ignores whitespace and properly categorizes keywords, operators, numbers, and identifiers.
2.  **Parsing (`parsing.h`)**
    Consumes the token stream to construct an Abstract Syntax Tree (AST). It uses a recursive-descent parsing strategy to naturally handle operator precedence (e.g., terms, factors, expressions, comparisons, and control statements).
3.  **Bytecode Compilation (`bytecode.h`)**
    Traverses the generated AST and flattens it into an array of linear `Instruction` objects (opcodes). It also resolves variable names into index identifiers and patches jump addresses for control flow (loops and conditionals).
4.  **Virtual Machine Execution (`compiler.h`)**
    A stack-based runtime environment. It reads the bytecode instructions one by one, utilizing an evaluation stack for math/logic and an unordered map for variable storage to simulate the code execution.

## Example Usage

The entry point (`main.cpp`) compiles and runs the following built-in script by default:

```javascript
x = 0;
while (x < 5) {
    x = x + 1;
}
print(x); // Expected Output: 5

y = 2 + 3 * 4;
print(y); // Expected Output: 14

if (y > 10) {
    print(1); // Expected Output: 1
} else {
    print(0);
}

x = x + 10;
print(x); // Expected Output: 15