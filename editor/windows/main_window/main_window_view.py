from typing import Any

from PySide6.QtWidgets import QMainWindow, QDockWidget, QWidgetAction
from PySide6.QtGui import QKeyEvent
from PySide6.QtCore import Qt

from converted.editor_main_window import Ui_GameEngineEditorWindow
from di import *
from .main_window_view_model import EditorMainWindowViewModel
from .widgets import *

from windows.new_project_window.new_project_window_view import *


@as_singleton
@as_dependencies(EditorMainWindowViewModel, NewProjectWindowView)
class EditorMainWindow(QMainWindow):
    def __init__(
        self,
        viewModel: EditorMainWindowViewModel,
        newProjectWindow: NewProjectWindowView,
        *args: Any,
        **kwargs: Any,
    ) -> None:
        super().__init__(*args, **kwargs)

        self.ui = Ui_GameEngineEditorWindow()
        self.ui.setupUi(self)  # type: ignore

        self.viewModel = viewModel
        self.newProjectWindow = newProjectWindow

        self.logWidget = di_get(LogWidget)
        self.ui.logDockWidget.setWidget(self.logWidget)

        self.setup_menu()
        self.setup_docks()

        Logger.Log(
            LOG_LEVEL_WARN,
            LOG_TAG_MASK_SYSTEM,
            "Editor Main Window initialized.",
            "file.py",
            30,
        )

    def setup_menu(self) -> None:
        self.ui.actionNewProject.triggered.connect(self.newProjectWindow.show)

    def setup_docks(self) -> None:
        dockViews: list[QDockWidget] = [
            self.ui.logDockWidget,
        ]

        for dock in dockViews:
            name = dock.windowTitle()
            action = QWidgetAction(self)
            action.setText(name)
            action.setCheckable(True)
            action.setChecked(dock.isVisible())

            action.toggled.connect(dock.setVisible)
            dock.visibilityChanged.connect(action.setChecked)

            self.ui.menuWindow.addAction(action)

    def keyPressEvent(self, event: QKeyEvent) -> None:
        if event.key() == Qt.Key.Key_Escape:
            self.close()

        return super().keyPressEvent(event)
