# Assembler GUI - User Guide

## Key Constraints
- All instructions must be written in **lowercase**.
- Immediate values should be in **0x8F99** format, with alphabets in uppercase.

### Example of Valid Assembly Code:
```assembly
start:
    addu r1, r2, 0x3
    nop
    b skp
    nop
    b 0x1
    nop
    b start
skp:
    hlt
```

---

## Installation & Setup
### Step 1: Clone the Repository
```bash
git clone https://github.com/yourusername/your-repository.git
cd your-repository
```

### Step 2: Install Dependencies
Ensure you have **Python 3** and **PyQt5** installed.
```bash
pip install PyQt5
```

### Step 3: Organize Files
Place the following files in a **single directory**:
- `assembler_gui.py` (UI code)
- `tiny.exe` (Assembler executable)
- Any additional dependency files

---

## Running the UI
To launch the Assembler GUI, run:
```bash
python assembler_gui.py
```

---

## Using the UI
### 1. **Input Section**
- Enter assembly instructions in the left editor box.

### 2. **Output Sections**
- **Debug Output**: Displays any errors or debug information.
- **Binary Output**: Shows the assembled binary code.
- **Hex Output**: Displays the hex representation of the output.

### 3. **Control Buttons**
- `Run` - Assembles the code and processes output.
- `Debug` - Shows debugging information.
- `Output Binary` - Displays the binary result.
- `Output Hex` - Displays the hex result.
- `Clear` - Clears all fields.

---

