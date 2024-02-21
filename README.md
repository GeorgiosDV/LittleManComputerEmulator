# LittleManComputerEmulator

## Overview

LittleManComputerEmulator is an interpreter for the Little Man Computer (LMC). This interpreter lets users write, load, and run precompiled LMC programs.

## Features

- Simulates the LMC architecture.
- Executes LMC programs from `.lmcb` files, allowing for easy sharing and testing of code.
- Provides a command-line interface for executing LMC programs.
- Includes example LMC programs in the `Examples` folder to get users started quickly.

## File Extensions

- **LMCP Files**: Files with the extension `.lmcp` contain human-readable instructions that define the behavior of the LMC program. 

- **LMCB Files**: Files with the extension `.lmcb` are precompiled files generated from the LMC program code. These files contain the compiled machine code the interpreter reads and executes directly.

## Getting Started

### Prerequisites

To run the LittleManComputerEmulator, you need:

- A C++ compiler (GCC, Clang, MSVC, etc.)
- Basic knowledge of using command-line interfaces

### Compilation

To compile the interpreter, navigate to the project directory and run the following command :

```bash
g++ -o LMC main.cpp
```

This command compiles the `main.cpp` file (and any other necessary source files) into an executable named `LMC`.

### Running the Interpreter

To run the interpreter and load an example program, use the following command:

```bash
./LMC <Examples/example.lmcb>
```

Replace `<Examples/example.lmcb>` with the actual path to a `.lmcb` file in the `Examples` folder. If the program requires input, the interpreter will prompt you during execution.
