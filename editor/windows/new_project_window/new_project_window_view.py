from di import *
from PySide6.QtWidgets import QMainWindow
from converted.new_project import Ui_NewProjectWindow


@as_singleton
class NewProjectWindowView(QMainWindow):
    def __init__(self):
        super().__init__()

        self.ui = Ui_NewProjectWindow()
        self.ui.setupUi(self)  # type: ignore
