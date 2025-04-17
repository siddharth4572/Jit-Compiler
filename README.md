# 🚀 JIT Compiler for an Optimized Execution of a Programming Language

This project implements a simple **interpreter and JIT (Just-In-Time) compiler** using **Flex**, **Bison**, and **C++**, capable of executing a subset of a programming language with support for **arithmetic expressions**, **variables**, and **function calls**. It lays the foundation for a JIT compiler using LLVM, and currently works as a tree-walking interpreter over an **Abstract Syntax Tree (AST)**.

---

## 🔧 Features

- Variable declarations and assignments (`let x = 5;`)
- Arithmetic operations: `+`, `-`, `*`, `/`
- Parenthesized expressions for precedence control
- Built-in function: `print(expr);`
- AST-based evaluation
- Modular architecture (Lexer, Parser, AST, Evaluator)
- Clean CMake-based build system

---

## 📁 Project Structure

```
jit/
├── include/
│   └── ast.hpp         # AST structure: Expression & Statement nodes
├── src/
│   ├── lexer.l         # Tokenizer (Flex): breaks input into tokens
│   ├── parser.y        # Parser (Bison): builds AST using grammar rules
│   ├── helper.cpp      # Interpreter logic (AST evaluation)
│   └── main.cpp        # Main function: input loop and glue logic
├── build/              # (Generated) Build files from CMake
├── generated/          # Flex/Bison generated .cpp and .hpp files
└── CMakeLists.txt      # CMake configuration for building the project
```

---

## 🧠 How It Works

The compiler is structured into four main stages:

### 1. **Lexing (`lexer.l`)**
- Uses **Flex** to convert raw text input into tokens like `LET`, `IDENTIFIER`, `NUMBER`, `PRINT`, `=`, `+`, `;`, etc.
- Example:
  ```
  let x = 10;
  ```
  becomes tokens: `LET IDENTIFIER ASSIGN NUMBER SEMICOLON`

### 2. **Parsing (`parser.y`)**
- Uses **Bison** to define grammar rules and parse the token stream into an **AST**.
- For example:
  ```
  let x = 3 + 5 * 2;
  ```
  builds a tree representing:
  ```
        =
       / \
     x   +
         / \
        3   *
           / \
          5   2
  ```

### 3. **AST (`ast.hpp`)**
- Defines node structures for:
  - Binary operations
  - Literals
  - Variables
  - Function calls
  - Statement nodes (let, expression, etc.)
- The parser builds these nodes dynamically using `new` and pointer types.

### 4. **Evaluation (`helper.cpp`)**
- Traverses the AST recursively to **evaluate** expressions and **execute** statements.
- Maintains a simple **symbol table (map of variables)**.
- Supports function dispatch (like `print(...)`) by name.

---

## ▶️ Example Interaction

```text
> let a = 5;
> let b = 10;
> let c = a + b * 2;
> print(c);
25
```

### Supported Syntax

| Feature         | Example                    |
|----------------|----------------------------|
| Variable Decl   | `let x = 42;`              |
| Arithmetic      | `x + y * (z - 3)`          |
| Function Call   | `print(x);`                |

---

## 🛠️ Building and Running

### ✅ Prerequisites

- Linux-based system
- CMake (`>=3.10`)
- Flex (`>=2.6`)
- Bison (`>=3.0`)
- GCC or Clang

### 📦 Build Steps

```bash
cd jit/
mkdir -p build
cd build
cmake ..
make
./jit
```

This will launch a simple REPL-like prompt where you can paste your statements.

---

## 🚧 Known Limitations

- No support for user-defined functions yet
- No type system or error recovery
- Currently uses interpretation, not LLVM codegen (to be added)

---

## 🚀 Future Plans

- LLVM-based code generation for actual JIT compilation
- User-defined functions and parameters
- Scoped variables and block execution
- Type checking and better diagnostics
- Interactive REPL enhancements

---

## 👨‍💻 Team

| Name                | Role        |
|---------------------|-------------|
| Dhananjay Pundir    | Team Lead   |
| Aman Singh Rawat    | Developer   |
| Siddharth Katyal    | Developer   |

---

## 📄 License

This project is intended for academic and educational purposes and is released under an open-source license.
