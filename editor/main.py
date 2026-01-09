from PySide6.QtGui import QKeyEvent
from PySide6.QtWidgets import QApplication, QMainWindow
from PySide6.QtCore import Qt
from Engine import *
from utils import *


class MainWindow(QMainWindow):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("My Application")
        self.setGeometry(100, 100, 800, 600)

    def keyPressEvent(self, event: QKeyEvent) -> None:
        if event.key() == Qt.Key.Key_Escape:
            self.close()

        return super().keyPressEvent(event)


def main():
    editor_logger.setLevel(logging.DEBUG)
    Console.setColor(ConsoleColor.CONSOLE_COLOR_CYAN)
    Console.print("Starting the application...\n")
    app = QApplication([])
    window = MainWindow()
    editor_logger.info("Starting the application.")
    window.showMaximized()
    app.exec()


if __name__ == "__main__":
    main()
