# CVM++ - Stack-Based Virtual Machine & Custom Compiler

CVM++ is a lightweight scripting language that was built from scratch in C++. Custom `.cvm` scripts are compiled down to bytecode, which is then executed by a custom-built, stack-based Virtual Machine. This project was developed so that how real programming languages work under the hood — from raw source text all the way to execution — could be understood and demystified.

---

## How it works

`.cvm` source code is passed through four stages:

```
Source Code  →  Lexer  →  Parser  →  Compiler  →  Virtual Machine  →  Output
```

- **Lexer** — raw source text is read and broken into tokens
- **Parser** — tokens are arranged into an Abstract Syntax Tree (AST)
- **Compiler** — the AST is traversed and bytecode instructions are generated
- **Virtual Machine** — the bytecode is executed on a stack-based architecture

---

## Language features

| Feature | Syntax |
|---|---|
| Variable declaration | `let x = 10` |
| Arithmetic | `x + y`, `x - y`, `x * y`, `x / y` |
| Comparison | `x == y`, `x < y`, `x > y` |
| Boolean literals | `true`, `false` |
| Print output | `output x` |
| Read input | `input x` |
| If / else | `if condition:` ... `end` |
| While loop | `while condition:` ... `end` |

---

## Example scripts

### Hello World
```
let x = 42
output x
```

### Arithmetic
```
let a = 10
let b = 20
let c = a + b
output c
```

### If / Else
```
let x = 15

if x < 20:
    output x
    let x = x + 5
    output x
end
```

### While Loop
```
let x = 1

while x < 6:
    output x
    let x = x + 1
end
```

### User Input
```
input x
let y = x + 10
output y
```

### Nested blocks
```
let x = 0

while x < 10:
    let x = x + 1
    if x == 5:
        output x
    end
end
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
git clone https://github.com/YourUsername/cvm-plus-plus.git
cd cvm-plus-plus
cd build
cmake .. -G "MinGW Makefiles"
cmake --build .
```

### Run a script

```powershell
.\cvm.exe ..\scripts\hello.cvm
```

### Run the REPL (interactive mode)

```powershell
.\cvm.exe
```

---

## Project structure

```
cvm-plus-plus/
├── src/
│   ├── main.cpp        # Entry point — REPL and file runner
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
| `OP_EQUAL` / `OP_LESS` / `OP_GREATER` | Comparison operations are performed |
| `OP_STORE` | The top of the stack is popped and stored into a variable |
| `OP_LOAD` | A variable's value is pushed onto the stack |
| `OP_JUMP` | An unconditional jump is made to an address |
| `OP_JUMP_IF_FALSE` | A jump is made if the top of the stack is false |
| `OP_OUTPUT` | The top of the stack is popped and printed |
| `OP_INPUT` | An integer is read from the user and pushed onto the stack |
| `OP_RETURN` | Program execution is ended |

---

## Debug modes

Flags can be passed when the program is run to see internal details:

```powershell
.\cvm.exe --debug-tokens script.cvm     # all tokens from the lexer are printed
.\cvm.exe --debug-ast script.cvm        # the Abstract Syntax Tree is printed
.\cvm.exe --debug-bytecode script.cvm   # the compiled bytecode is printed
```

---

## Tech stack

- Language: C++17
- Build tool: CMake
- Concepts: Lexical analysis, recursive descent parsing, bytecode generation, stack-based execution

---

## References

- [Crafting Interpreters](https://craftinginterpreters.com) by Robert Nystrom — the definitive guide for building lexers, parsers, and virtual machines from scratch

---

## License

MIT License — free to use, modify, and distribute.