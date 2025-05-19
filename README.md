# 🚀 JIT Compiler for Optimized Execution of a Programming Language

This project implements a basic interpreter and a **JIT (Just-In-Time) compiler** using **Flex**, **Bison**, **C++**, and **LLVM**. It can execute a subset of a programming language with support for arithmetic expressions, variables, and function calls. The system currently includes both an **AST-based interpreter** and **LLVM-based code generation** for JIT compilation.

---

## 🔧 Features

- Variable declarations and assignments (`let x = 5;`)
- Arithmetic operations: `+`, `-`, `*`, `/`
- Parenthesized expressions for precedence control
- Built-in function: `print(expr);`
- AST-based evaluation
- **LLVM-based JIT compilation** (generates LLVM IR)
- Modular architecture (Lexer, Parser, AST, Evaluator, Codegen)
- Clean CMake-based build system

---

## 📁 Project Structure

```
jit/
├── include/
│   ├── ast.hpp         # AST structure: Expression & Statement nodes
│   └── codegen.hpp     # LLVM code generation logic
├── src/
│   ├── lexer.l         # Tokenizer (Flex): breaks input into tokens
│   ├── parser.y        # Parser (Bison): builds AST using grammar rules
│   ├── helper.cpp      # Interpreter logic (AST evaluation)
│   ├── codegen.cpp     # Generates LLVM IR from AST
│   └── main.cpp        # Main entry: REPL interface and glue logic
├── build/              # Build artifacts (Makefile, output.ll, etc.)
├── generated/          # Flex/Bison generated .cpp and .hpp files
└── CMakeLists.txt      # CMake configuration
```

---

## 🧠 How It Works

### 1. Lexing (`lexer.l`)
Uses **Flex** to tokenize the input into meaningful units like keywords, identifiers, and operators.

**Input:**
```js
let x = 10;
```
**Tokens:**
```
LET IDENTIFIER ASSIGN NUMBER SEMICOLON
```

---

### 2. Parsing (`parser.y`)
Uses **Bison** to parse tokens into an **Abstract Syntax Tree (AST)** using grammar rules.

**Input:**
```js
let x = 3 + 5 * 2;
```

**AST Output:**
```
      =
     / \
   x   +
       / \
      3   *
         / \
        5   2
```

---

### 3. AST Definition (`ast.hpp`)
Defines node structures for:
- Binary operations
- Literals
- Variables
- Function calls
- Statements (e.g., `let`, expression statements)

---

### 4. Evaluation (`helper.cpp`)
- Walks the AST recursively
- Evaluates expressions
- Maintains a symbol table
- Supports built-in functions like `print(...)`

---

### 5. LLVM Code Generation (`codegen.cpp`, `codegen.hpp`)
- Converts AST into LLVM Intermediate Representation (IR)
- Uses LLVM APIs for instruction creation and module handling
- Outputs LLVM IR (`output.ll`)
- Intended to enable real-time JIT execution using LLVM's execution engine (experimental)

---

## ▶️ Example Interaction

```text
> let a = 5;
> let b = 10;
> let c = a + b * 2;
> print(c);
25
```

---

## ✅ Supported Syntax

| Feature          | Example                   |
|------------------|---------------------------|
| Variable Decl    | `let x = 42;`             |
| Arithmetic       | `x + y * (z - 3)`         |
| Function Call    | `print(x);`               |

---

## 🛠️ Building and Running

### Prerequisites

- Linux-based system
- CMake (>= 3.10)
- Flex (>= 2.6)
- Bison (>= 3.0)
- LLVM development libraries
- GCC or Clang

### Build Instructions

```bash
cd jit/
mkdir -p build
cd build
cmake ..
make
./jit
```

This will launch a REPL interface where you can write and execute statements interactively.

---

## 🚀 Roadmap

- ✅ LLVM-based code generation (early support)
- 🔄 Full JIT integration using LLVM Execution Engine
- 🔧 User-defined functions with parameters
- 📦 Lexical scoping and blocks
- 🧪 Static type checking and type inference
- 🖥️ Better REPL diagnostics and formatting

---

## 👨‍💻 Team

| Name               | Role        |
|--------------------|-------------|
| Dhananjay Pundir   | Team Lead   |
| Aman Singh Rawat   | Developer   |
| Siddharth Katyal   | Developer   |

---

## 📄 License

This project is intended for academic and educational use. It is released under an open-source license.
