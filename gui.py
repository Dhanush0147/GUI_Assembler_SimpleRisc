import sys
import subprocess
import os
from PyQt5.QtWidgets import (QApplication, QMainWindow, QWidget, QVBoxLayout, QHBoxLayout, QTextEdit, QPushButton, QLabel)
from PyQt5.QtGui import QFont, QColor, QSyntaxHighlighter, QTextCharFormat
from PyQt5.QtCore import QRegExp, Qt

# Global variables for colors and sizes
BACKGROUND_COLOR = "#2E2B29"
TEXT_COLOR = "Green"
INPUT_BG_COLOR = "#3E3B39"
INPUT_TEXT_COLOR = "#E5E4E2"
DEBUG_BG_COLOR = "#1E1B19"
DEBUG_TEXT_COLOR = "#FF4500"
BINARY_TEXT_COLOR = "#FFD700"
HEX_TEXT_COLOR = "#FFA500"
BORDER_COLOR = "orange"
BORDER_WIDTH = 3
BORDER_RADIUS = 8
BUTTON_BG_COLOR = "#007BFF"
BUTTON_HOVER_COLOR = "#0056b3"
BUTTON_PRESSED_COLOR = "#004080"
BUTTON_TEXT_COLOR = "white"
BUTTON_RADIUS = 20
BUTTON_PADDING = 10
BUTTON_FONT_SIZE = 14
MAC_BUTTON_SIZE = 30
MAC_BUTTON_RADIUS = 15
MAC_BUTTON_FONT_SIZE = 18
CLOSE_BUTTON_COLOR = "red"
MINIMIZE_BUTTON_COLOR = "yellow"
MAXIMIZE_BUTTON_COLOR = "green"
LABEL_FONT_WEIGHT = "bold"
LABEL_FONT_SIZE = 18
LABEL_BG_COLOR = "white"
LABEL_TEXT_COLOR = "black"

class AsmHighlighter(QSyntaxHighlighter):
    def __init__(self, document):
        super().__init__(document)
        self.highlightingRules = []

        keywordFormat = QTextCharFormat()
        keywordFormat.setForeground(QColor("#FFDD44"))
        keywordFormat.setFontWeight(QFont.Bold)
        keywords = ["MOV", "ADD", "SUB", "MUL", "DIV", "JMP", "CMP", "AND", "OR", "XOR"]

        for keyword in keywords:
            pattern = QRegExp(f"\\b{keyword}\\b")
            self.highlightingRules.append((pattern, keywordFormat))

    def highlightBlock(self, text):
        for pattern, format in self.highlightingRules:
            expression = pattern
            index = expression.indexIn(text)
            while index >= 0:
                length = expression.matchedLength()
                self.setFormat(index, length, format)
                index = expression.indexIn(text, index + length)

class AssemblerGUI(QMainWindow):
    def __init__(self):
        super().__init__()
        self.setWindowFlags(Qt.FramelessWindowHint)  # Remove default title bar
        self.initUI()

    def initUI(self):
        self.setWindowTitle("GUI_simpleRisc_Assembler")
        self.setGeometry(100, 100, 900, 600)
        self.setStyleSheet(f"background-color: {BACKGROUND_COLOR}; color: {TEXT_COLOR};")

        mainWidget = QWidget()
        self.setCentralWidget(mainWidget)
        mainLayout = QVBoxLayout()

        # Mac-Style Title Bar
        titleBar = QHBoxLayout()
        titleBar.setSpacing(10)

        self.closeButton = QPushButton("✖")
        self.minimizeButton = QPushButton("─")
        self.maximizeButton = QPushButton("◻")

        buttonStyle = f"""
            QPushButton {{
                font-size: {MAC_BUTTON_FONT_SIZE}px;
                font-weight: bold;
                border: none;
                width: {MAC_BUTTON_SIZE}px;
                height: {MAC_BUTTON_SIZE}px;
                border-radius: {MAC_BUTTON_SIZE }px;
                text-align: center;
                line-height: {MAC_BUTTON_SIZE}px;
            }}
            QPushButton:pressed {{
                padding-top: 2px;
            }}
        """

        self.closeButton.setStyleSheet(f"background-color: {CLOSE_BUTTON_COLOR}; color: white;" + buttonStyle)
        self.minimizeButton.setStyleSheet(f"background-color: {MINIMIZE_BUTTON_COLOR}; color: black;" + buttonStyle)
        self.maximizeButton.setStyleSheet(f"background-color: {MAXIMIZE_BUTTON_COLOR}; color: white;" + buttonStyle)

        self.closeButton.clicked.connect(self.close)
        self.minimizeButton.clicked.connect(self.showMinimized)
        self.maximizeButton.clicked.connect(self.showMaximized)

        titleBar.addWidget(self.closeButton)
        titleBar.addWidget(self.minimizeButton)
        titleBar.addWidget(self.maximizeButton)
        titleBar.addStretch(1)

        # Add window title label
        windowTitleLabel = QLabel("GUI_simpleRisc_Assembler")
        windowTitleLabel.setStyleSheet(f"""
            font-weight: {LABEL_FONT_WEIGHT}; 
            font-size: {LABEL_FONT_SIZE}px; 
            color: {LABEL_TEXT_COLOR}; 
            background-color: {LABEL_BG_COLOR};
            padding: 5px;
            border-radius: 5px;
        """)
        titleBar.addWidget(windowTitleLabel)
        titleBar.addStretch(1)

        mainLayout.addLayout(titleBar)

        # Layout for Input and Output
        contentLayout = QHBoxLayout()

        # Left Side (Input Box)
        inputLayout = QVBoxLayout()
        inputLabel = QLabel("Input_Assembly_Code")
        inputLabel.setStyleSheet(f"""
            font-weight: {LABEL_FONT_WEIGHT}; 
            font-size: {LABEL_FONT_SIZE}px; 
            color: {LABEL_TEXT_COLOR}; 
            background-color: {LABEL_BG_COLOR};
            padding: 5px;
            border-radius: 5px;
        """)
        inputLabel.setAlignment(Qt.AlignCenter)
        
        self.asmInput = QTextEdit()
        self.asmInput.setFont(QFont("Calibri", 12))  # Set input font to Calibri
        self.asmInput.setStyleSheet(f"""
            background-color: {INPUT_BG_COLOR}; 
            color: {INPUT_TEXT_COLOR}; 
            border: {BORDER_WIDTH}px solid {BORDER_COLOR};
            border-radius: {BORDER_RADIUS}px;
        """)
        
        inputLayout.addWidget(inputLabel)
        inputLayout.addWidget(self.asmInput)
        contentLayout.addLayout(inputLayout, 1)

        # Right Side (Output Sections)
        outputLayout = QVBoxLayout()

        debugLabel = QLabel("Debug Output")
        debugLabel.setStyleSheet(f"""
            font-weight: {LABEL_FONT_WEIGHT}; 
            font-size: {LABEL_FONT_SIZE}px; 
            color: {LABEL_TEXT_COLOR}; 
            background-color: {LABEL_BG_COLOR};
            padding: 5px;
            border-radius: 5px;
        """)
        debugLabel.setAlignment(Qt.AlignCenter)
        
        self.debugOutput = QTextEdit()
        self.debugOutput.setReadOnly(True)
        self.debugOutput.setFont(QFont("Courier", 12))
        self.debugOutput.setStyleSheet(f"""
            background-color: {DEBUG_BG_COLOR}; 
            color: {DEBUG_TEXT_COLOR}; 
            border: {BORDER_WIDTH}px solid {BORDER_COLOR};
            border-radius: {BORDER_RADIUS}px;
        """)
        outputLayout.addWidget(debugLabel)
        outputLayout.addWidget(self.debugOutput)

        binaryLabel = QLabel("Binary Output")
        binaryLabel.setStyleSheet(f"""
            font-weight: {LABEL_FONT_WEIGHT}; 
            font-size: {LABEL_FONT_SIZE}px; 
            color: {LABEL_TEXT_COLOR}; 
            background-color: {LABEL_BG_COLOR};
            padding: 5px;
            border-radius: 5px;
        """)
        binaryLabel.setAlignment(Qt.AlignCenter)
        
        self.binaryOutput = QTextEdit()
        self.binaryOutput.setReadOnly(True)
        self.binaryOutput.setFont(QFont("Courier", 12))
        self.binaryOutput.setStyleSheet(f"""
            background-color: {DEBUG_BG_COLOR}; 
            color: {BINARY_TEXT_COLOR}; 
            border: {BORDER_WIDTH}px solid {BORDER_COLOR};
            border-radius: {BORDER_RADIUS}px;
        """)
        outputLayout.addWidget(binaryLabel)
        outputLayout.addWidget(self.binaryOutput)

        hexLabel = QLabel("Hex Output")
        hexLabel.setStyleSheet(f"""
            font-weight: {LABEL_FONT_WEIGHT}; 
            font-size: {LABEL_FONT_SIZE}px; 
            color: {LABEL_TEXT_COLOR}; 
            background-color: {LABEL_BG_COLOR};
            padding: 5px;
            border-radius: 5px;
        """)
        hexLabel.setAlignment(Qt.AlignCenter)
        
        self.hexOutput = QTextEdit()
        self.hexOutput.setReadOnly(True)
        self.hexOutput.setFont(QFont("Courier", 12))
        self.hexOutput.setStyleSheet(f"""
            background-color: {DEBUG_BG_COLOR}; 
            color: {HEX_TEXT_COLOR}; 
            border: {BORDER_WIDTH}px solid {BORDER_COLOR};
            border-radius: {BORDER_RADIUS}px;
        """)
        outputLayout.addWidget(hexLabel)
        outputLayout.addWidget(self.hexOutput)

        contentLayout.addLayout(outputLayout, 1)
        mainLayout.addLayout(contentLayout)

        # Button Layout at Bottom
        buttonLayout = QHBoxLayout()
        buttonStyle = f"""
            QPushButton {{
                background-color: {BUTTON_BG_COLOR};
                color: {BUTTON_TEXT_COLOR};
                border-radius: {BUTTON_RADIUS}px;
                padding: {BUTTON_PADDING}px;
                font-size: {BUTTON_FONT_SIZE}px;
            }}
            QPushButton:hover {{
                background-color: {BUTTON_HOVER_COLOR};
            }}
            QPushButton:pressed {{
                background-color: {BUTTON_PRESSED_COLOR};
                padding-top: 12px;
                padding-bottom: 8px;
            }}
        """

        self.runButton = QPushButton("Run")
        self.runButton.setStyleSheet(buttonStyle)
        self.runButton.clicked.connect(self.runAssembler)
        buttonLayout.addWidget(self.runButton)

        self.debugButton = QPushButton("Debug")
        self.debugButton.setStyleSheet(buttonStyle)
        self.debugButton.clicked.connect(self.showDebugOutput)
        buttonLayout.addWidget(self.debugButton)

        self.binaryButton = QPushButton("Output Binary")
        self.binaryButton.setStyleSheet(buttonStyle)
        self.binaryButton.clicked.connect(self.showBinaryOutput)
        buttonLayout.addWidget(self.binaryButton)

        self.hexButton = QPushButton("Output Hex")
        self.hexButton.setStyleSheet(buttonStyle)
        self.hexButton.clicked.connect(self.showHexOutput)
        buttonLayout.addWidget(self.hexButton)

        self.clearButton = QPushButton("Clear")
        self.clearButton.setStyleSheet(buttonStyle)
        self.clearButton.clicked.connect(self.clearAll)
        buttonLayout.addWidget(self.clearButton)

        mainLayout.addLayout(buttonLayout)
        mainWidget.setLayout(mainLayout)

        self.last_binary_output = ""
        self.last_hex_output = ""
        self.last_debug_output = ""

    def runAssembler(self):
        """ Clears outputs before running the assembler. """
        self.clearOutputs()
        with open("asm.txt", "w") as file:
            file.write(self.asmInput.toPlainText())
        process = subprocess.Popen("tiny.exe", stdout=subprocess.PIPE, stderr=subprocess.PIPE, shell=True)
        stdout, stderr = process.communicate()
        self.last_debug_output = stderr.decode() if stderr else stdout.decode()
        self.last_binary_output = self._readFile("output.txt", "Binary file not generated")
        self.last_hex_output = self._readFile("output.hex", "Hex file not generated")

    def clearAll(self):
        """ Clears input and output fields. """
        self.asmInput.clear()
        self.clearOutputs()

    def clearOutputs(self):
        """ Clears all output fields. """
        self.debugOutput.clear()
        self.binaryOutput.clear()
        self.hexOutput.clear()

    def showDebugOutput(self):
        self.runAssembler()
        self.debugOutput.setText(self.last_debug_output)

    def showBinaryOutput(self):
        self.runAssembler()
        self.binaryOutput.setText(self.last_binary_output)

    def showHexOutput(self):
        self.runAssembler()
        self.hexOutput.setText(self.last_hex_output)

    def _readFile(self, filename, defaultMessage):
        return open(filename).read() if os.path.exists(filename) else defaultMessage

if __name__ == "__main__":
    app = QApplication(sys.argv)
    gui = AssemblerGUI()
    gui.show()
    sys.exit(app.exec_())
