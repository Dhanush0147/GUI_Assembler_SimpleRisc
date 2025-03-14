import sys
import subprocess
from PyQt5.QtWidgets import QApplication, QWidget, QVBoxLayout, QTextEdit, QPushButton
from PyQt5.QtGui import QFont, QPalette, QColor

class AssemblerGUI(QWidget):
    def __init__(self):
        super().__init__()

        self.setWindowTitle("Assembler GUI")
        self.setGeometry(100, 100, 600, 400)

        # Set Dark Theme
        palette = QPalette()
        palette.setColor(QPalette.Window, QColor("#1e1e1e"))  # Dark background
        palette.setColor(QPalette.WindowText, QColor("#ffffff"))  # White text
        self.setPalette(palette)
        self.setStyleSheet("border: 3px solid green;")  # Green border

        # Layout
        layout = QVBoxLayout()

        # Assembly Code Textbox
        self.text_edit = QTextEdit()
        self.text_edit.setFont(QFont("Courier", 12))
        self.text_edit.setStyleSheet("background-color: #2b2b2b; color: #ffffff;")
        layout.addWidget(self.text_edit)

        # Run Button
        self.run_button = QPushButton("Run Assembler")
        self.run_button.setFont(QFont("Arial", 12, QFont.Bold))
        self.run_button.setStyleSheet("background-color: green; color: black; padding: 5px;")
        self.run_button.clicked.connect(self.run_assembler)
        layout.addWidget(self.run_button)

        # Output Display
        self.output_display = QTextEdit()
        self.output_display.setFont(QFont("Courier", 12))
        self.output_display.setStyleSheet("background-color: #2b2b2b; color: #00ff00;")  # Green text
        self.output_display.setReadOnly(True)
        layout.addWidget(self.output_display)

        self.setLayout(layout)

    def run_assembler(self):
        # Save input assembly code to a file
        with open("asm.txt", "w") as file:
            file.write(self.text_edit.toPlainText())

        # Run the C++ assembler (Replace "assembler.exe" with your compiled program)
        try:
            subprocess.run(["./tiny"], check=True)
        except Exception as e:
            self.output_display.setText(f"Error: {e}")
            return

        # Read and display the output from output.hex
        try:
            with open("output.hex", "r") as file:
                output_data = file.read()
                self.output_display.setText(output_data)
        except FileNotFoundError:
            self.output_display.setText("Error: output.hex not found!")

if __name__ == "__main__":
    app = QApplication(sys.argv)
    window = AssemblerGUI()
    window.show()
    sys.exit(app.exec_())
