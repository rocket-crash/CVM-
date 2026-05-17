# Custom C++ Bytecode Interpreter & Compiler

A lightweight, stack-based custom programming language compiler and virtual machine (VM) written entirely in C++ from scratch. It features its own lexer, recursive-descent parser, bytecode compiler, and runtime interpreter.

## Features
The language supports a variety of core programming constructs:
* **Data Types**: Integers and Booleans (`true`, `false`).
* **Variables**: Dynamic assignment and static storage.
* **Arithmetic Operations**: Addition (`+`), Subtraction (`-`), Multiplication (`*`), Division (`/`), 
* **Comparisons**: `>`, `<`, `>=`, `<=`, `==`, `!=`.
* **Control Flow**:
    * `if` / `else` statements
    * `while` loops
    * Block scopes (`{ ... }`)
    * Nested Blocks
    * Ignore // comment
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
    Traverses the generated AST and flattens it into an array of linear `Instruction` objects . It also resolves variable names into index identifiers and patches jump addresses for control flow (loops and conditionals).
4.  **Virtual Machine Execution (`compiler.h`)**
    A stack-based runtime environment. It reads the bytecode instructions one by one, utilizing an evaluation stack for math/logic and an unordered map for variable storage to simulate the code execution.

## Design Choices: Benefits & Trade-offs

Building a language from scratch requires balancing simplicity, performance, and development time. Here is a breakdown of the architectural choices made in this project.

### 🌟 Benefits & Strengths

* **Zero Dependencies:** The entire pipeline (Lexer, Parser, Compiler, and VM) is written in pure standard C++ (C++14). It requires no external tools like Lex/Yacc, Bison, or LLVM, making it highly portable and easy to build on any system.
* **Separation of Concerns:** The pipeline is strictly modular. The VM doesn't know about the Lexer, and the AST doesn't know about the Bytecode. This makes it incredibly easy to swap out the "frontend" (syntax) without changing the "backend" (execution engine).
* **Bytecode over Tree-Walking:** Instead of executing code directly by traversing the AST (which is slow and memory-intensive), the compiler flattens the AST into a linear array of Bytecode opcodes. This drastically improves execution speed and mimics how production languages like Java and Python work.
* **Memory Safety:** The AST uses C++ `std::unique_ptr` extensively, ensuring that the tree structure is automatically and safely deallocated without memory leaks or the need for a custom garbage collector.

### ⚖️ Trade-offs & Current Limitations

* **Integer-Only Stack:** The Virtual Machine is heavily optimized for mathematical computation, meaning the evaluation stack is strictly typed to `int`. It currently does not support strings, floating-point numbers, or complex objects.
* **Hash Map Variable Storage:** Variables are stored in an `std::unordered_map` at runtime. While this is conceptually simple and handles dynamic assignment well, it is slower than allocating variables directly into static stack frames or using an array-based lookup.
* **No Abstract Syntax Tree (AST) Optimizations:** The bytecode compiler currently emits instructions exactly as the AST dictates. It does not perform intermediate optimization passes like constant folding (e.g., pre-calculating `2 + 3` at compile time instead of runtime).
* **Basic Error Reporting:** Syntax and execution errors trigger standard C++ exceptions (`std::runtime_error`) which halt execution safely. However, it currently lacks line number and column number tracking, making debugging complex scripts slightly more manual.
## Example Usage

The entry point (`main.cpp`) compiles and runs the following built-in script by default:

a=input();
b=input();
result = (a + b) * 2 - (a / b); 
print(result);

is_true = true;
is_false = false;
if (a >= 10) {
    if (b != 5) {
        print(1);
    } else {
        print(0);
    }
} else {
    print(0);
}

counter = 0;
while (counter < 3) {
    print(counter);
    counter = counter + 1;
}

countdown = 5;
while (countdown > 0) {
    if (countdown == 3) {
        print(9999);
    } else {
        print(countdown);
    }
    countdown = countdown - 1;
}
