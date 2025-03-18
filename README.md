# Assembler GUI - User Guide

## Overview
This Assembler GUI provides a user-friendly interface to write and execute assembly code, view debugging information, and output binary and hexadecimal representations of the compiled code.

## Prerequisites
Before running the UI, ensure that you have the following installed on your system:
- Python 3.x
- PyQt5 (for GUI components)
- A working assembler (`tiny.exe` is used in this project)
- Git (for repository management)

## Directory Structure
Ensure that all required files are placed in a single directory for proper execution. Your project folder should be structured as follows:

```
AssemblerGUI/
│── main.py  # The main Python script containing the GUI
│── tiny.exe  # Assembler executable
│── asm.txt  # Temporary file for assembly code
│── output.txt  # Binary output file
│── output.hex  # Hex output file
│── README.md  # This guide
```

## Installation & Setup
1. Clone this repository from GitHub:
   ```sh
   git clone https://github.com/yourusername/AssemblerGUI.git
   cd AssemblerGUI
   ```

2. Install required dependencies:
   ```sh
   pip install PyQt5
   ```

## Running the GUI
To start the GUI, navigate to the project directory and run:
```sh
python main.py
```

## Using the GUI
- **Input Box:** Write assembly code in the left-side input box.
- **Run Button:** Executes the assembler and compiles the input code.
- **Debug Button:** Displays debugging output.
- **Binary Output Button:** Shows binary representation of the compiled code.
- **Hex Output Button:** Displays hexadecimal output.
- **Clear Button:** Clears all input and output fields.
- **Mac-Style Buttons:** Close, minimize, and maximize buttons are included for better window management.



