from typing import Any
from PySide6.QtWidgets import QMainWindow
from converted.editor_main_window import Ui_GameEngineEditorWindow
from di import *
from .view_model import EditorMainWindowViewModel
from PySide6.QtGui import QKeyEvent
from PySide6.QtCore import Qt


@as_transient
@as_dependencies(EditorMainWindowViewModel)
class EditorMainWindow(QMainWindow):
    def __init__(
        self,
        viewModel: EditorMainWindowViewModel,
        *args: Any,
        **kwargs: Any,
    ) -> None:
        super().__init__(*args, **kwargs)

        self.ui = Ui_GameEngineEditorWindow()
        self.ui.setupUi(self)  # type: ignore

        self.viewModel = viewModel

    def keyPressEvent(self, event: QKeyEvent) -> None:
        if event.key() == Qt.Key.Key_Escape:
            self.close()

        return super().keyPressEvent(event)
