# GUI_Assembler_SimpleRisc
# Tiny Assembler for Simple RISC Processor

This is a simple assembler for a custom 21-instruction, 16-register Simple RISC processor. The assembler is written in C++ and includes a basic GUI using PyQt to provide a user-friendly interface for entering and processing assembly instructions.

## Features
- Converts assembly code into binary and hexadecimal formats.
- Supports 21 instructions and 16 registers.
- Simple PyQt-based GUI for user input.

## Installation
### Prerequisites
- C++ Compiler (g++)
- Python (>=3.7)
- PyQt Library (`pip install PyQt5`)

### Setup
1. Compile the assembler:
   ```sh
   g++ tiny.cpp -o tiny.exe
   ```
2. Ensure the following files are in the same directory as `tiny.exe` and `gui.py`:
   - `asm.txt` (for input assembly code)
   - `output.bin` (binary output file)
   - `output.hex` (hexadecimal output file)
3. Run the GUI:
   ```sh
   python gui.py
   ```

## Usage
1. Enter your assembly code in the GUI input field.
2. Click 'Assemble' to generate the corresponding machine code.
3. The hexadecimal output will be displayed below.

## Example Input
```assembly
start:
    nop
    b next
next:
    mov r1, 0x0001       ;fact = 1
    mov r2, r0      ;the number is stored in input port register r0
loop:
    mul r1, r1, r2 ;fact = fact * i
    subh r2, r2, 0x0001   ;decrement i
    cmp r2, 0x0001       ;compare i > 1
    bgt loop        ;if i > 1 then remain in loop
    mov r3,r1      ;else the result is stored in output port register r3
    ld r2,0x001[r3]
    hlt             ;stops program counter to be incremented
```

## Notes
- Ensure the required files are present before running the GUI.
- The assembler supports only the specific instruction set for this Simple RISC processor.
- The GUI simplifies input but can also be bypassed if you wish to run `tiny.exe` manually.



