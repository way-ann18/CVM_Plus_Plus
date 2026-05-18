# CVM++ — Stack-Based Virtual Machine & Custom Compiler

CVM++ is a lightweight scripting language that was built from scratch in C++. Custom `.cvm` scripts are compiled down to bytecode, which is then executed by a custom-built, stack-based Virtual Machine. This project was developed so that how real programming languages work under the hood - from raw source text all the way to execution — could be understood and demystified.

## Live Playground

CVM++ can be tried in the browser - no installation needed:

**[https://way-ann18.github.io/CVM_Plus_Plus](https://way-ann18.github.io/CVM_Plus_Plus)**

---

## How it works

`.cvm` source code is passed through four stages:

```
Source Code  →  Lexer  →  Parser  →  Compiler  →  Virtual Machine  →  Output
```

- **Lexer** - raw source text is read and broken into tokens
- **Parser** - tokens are arranged into an Abstract Syntax Tree (AST)
- **Compiler** - the AST is traversed and bytecode instructions are generated
- **Virtual Machine** - the bytecode is executed on a stack-based architecture

---

## Language features

| Feature | Syntax | Description |
|---|---|---|
| Variable declaration | `let x = 10` | Integer or boolean assignment |
| Arithmetic | `x + y`, `x - y`, `x * y`, `x / y` | Basic arithmetic |
| Modulo | `x % y` | Remainder operator |
| Comparison | `x == y`, `x < y`, `x > y` | Relational operators |
| Greater/less equal | `x >= y`, `x <= y` | Extended relational operators |
| Negative numbers | `let x = -10` | Negative integer support |
| Brackets | `(x + y) * z` | Grouping to override precedence |
| Boolean literals | `true`, `false` | Stored as 1 and 0 internally |
| Print output | `output x` | Print value to console |
| Read input | `input x` | Read integer from user |
| If / else if / else | `if condition:` ... `else if:` ... `else:` ... `end` | Multi-branch conditional |
| While loop | `while condition:` ... `end` | Condition-based loop |
| Nested blocks | `while` inside `if`, `if` inside `while` | Fully supported |
| Comments | `# this is a comment` | Line comments |

---

## Example scripts

### Hello World
```
let x = 42
output x
```

### Arithmetic and modulo
```
let a = 10
let b = 3
let sum = a + b
let remainder = a % b
output sum
output remainder
```

### Brackets and precedence
```
let x = 2 + 3 * 4
let y = (2 + 3) * 4
output x
output y
```

### Negative numbers
```
let x = -10
let y = -x
output x
output y
```

### If / else if / else
```
let marks = 75
if marks >= 90:
    output 1
else if marks >= 75:
    output 2
else if marks >= 60:
    output 3
else:
    output 4
end
```

### While loop
```
let i = 1
while i <= 5:
    output i
    let i = i + 1
end
```

### Nested blocks
```
let i = 1
while i <= 6:
    if i % 2 == 0:
        output 0
    else:
        output 1
    end
    let i = i + 1
end
```

### User input
```
input x
let y = x + 10
output y
```

### Factorial
```
input n
let result = 1
let i = 1
while i <= n:
    let result = result * i
    let i = i + 1
end
output result
```

---

## Getting started

### Requirements

- Windows (tested on Windows 10/11)
- [MSYS2](https://www.msys2.org) with MinGW-w64 (provides `g++` and `cmake`)
- [Visual Studio Code](https://code.visualstudio.com) (recommended)

### Install dependencies

MSYS2 can be downloaded and installed from [msys2.org](https://www.msys2.org). Once it is installed, the MSYS2 terminal should be opened and the following command should be run:

```bash
pacman -S --needed base-devel mingw-w64-ucrt-x86_64-gcc mingw-w64-ucrt-x86_64-cmake
```

The following path should then be added to the Windows PATH:
```
C:\msys64\ucrt64\bin
```

### Build from source

```powershell
git clone https://github.com/way-ann18/CVM_Plus_Plus.git
cd CVM_Plus_Plus
cd build
cmake .. -G "MinGW Makefiles"
cmake --build .
```

### Run a script

```powershell
.\cvm.exe ..\scripts\hello.cvm
```

---

## Project structure

```
CVM_Plus_Plus/
├── src/
│   ├── main.cpp        # Entry point and WebAssembly export
│   ├── lexer.h         # Token types and Lexer declaration
│   ├── lexer.cpp       # Lexer implementation
│   ├── parser.h        # AST node types and Parser declaration
│   ├── parser.cpp      # Parser implementation
│   ├── compiler.h      # Opcodes, Chunk, and Compiler declaration
│   ├── compiler.cpp    # Compiler implementation
│   ├── vm.h            # VM declaration
│   └── vm.cpp          # VM implementation
├── scripts/
│   └── hello.cvm       # Example scripts
├── web/
│   ├── index.html      # Browser playground UI
│   ├── cvm.js          # Emscripten JS glue
│   └── cvm.wasm        # Compiled WebAssembly binary
├── docs/               # GitHub Pages deployment
├── build/              # CMake build output
└── CMakeLists.txt      # Build configuration
```

---

## Instruction Set Architecture (ISA)

The following bytecode opcodes are supported by CVM++:

| Opcode | Description |
|---|---|
| `OP_CONSTANT` | A constant value is pushed onto the stack |
| `OP_TRUE` / `OP_FALSE` | A boolean value is pushed onto the stack |
| `OP_ADD` / `OP_SUBTRACT` / `OP_MULTIPLY` / `OP_DIVIDE` | Arithmetic operations are performed |
| `OP_MODULO` | Remainder operation is performed |
| `OP_EQUAL` / `OP_LESS` / `OP_GREATER` | Comparison operations are performed |
| `OP_LESS_EQUAL` / `OP_GREATER_EQUAL` | Extended comparison operations are performed |
| `OP_STORE` | The top of the stack is popped and stored into a variable |
| `OP_LOAD` | A variable's value is pushed onto the stack |
| `OP_JUMP` | An unconditional jump is made to an address |
| `OP_JUMP_IF_FALSE` | A jump is made if the top of the stack is false |
| `OP_OUTPUT` | The top of the stack is popped and printed |
| `OP_INPUT` | An integer is read from the user and pushed onto the stack |
| `OP_RETURN` | Program execution is ended |

---

## Error handling

Errors are attributed to the correct stage of the pipeline:

| Prefix | Stage | Example |
|---|---|---|
| `[Lexer Error]` | Tokenization | Unrecognized character |
| `[Parser Error]` | Parsing | Unexpected token, missing `:` or `end` |
| `[Compiler Error]` | Bytecode generation | Unknown operator |
| `[Runtime Error]` | Execution | Division by zero, undefined variable |

---

## Tech stack

- Language: C++17
- Build tool: CMake
- Browser deployment: Emscripten (WebAssembly) + Asyncify
- Hosting: GitHub Pages
- Concepts: Lexical analysis, recursive descent parsing, bytecode generation, stack-based execution

---
