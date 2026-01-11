from typing import Any

from PySide6.QtWidgets import QMainWindow, QDockWidget, QWidgetAction
from PySide6.QtGui import QKeyEvent, QCloseEvent
from PySide6.QtCore import Qt

from constants import *
from converted.editor_main_window import Ui_GameEngineEditorWindow
from di import *
from .main_window_view_model import EditorMainWindowViewModel
from .widgets import *
from common_models import *  # type: ignore

from windows.new_project_window.new_project_window_view import *  # type: ignore
from components import *  # type: ignore


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
        self.viewModel.load_appsetting()

        self.newProjectWindow = newProjectWindow

        self.logWidget = di_get(LogWidget)
        self.ui.logDockWidget.setWidget(self.logWidget)

        self.application = di_get(ApplicationContext).application
        shared = di_get(GLShared)
        shared.application = self.application

        self.setup_menu()
        self.setup_docks()

    def setup_menu(self) -> None:
        self.ui.actionNewProject.triggered.connect(self.newProjectWindow.show)

        di_get(ApplicationContext).project_desc_signal.connect(
            self.on_open_project, True
        )

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

    def closeEvent(self, event: QCloseEvent) -> None:
        self.application.Shutdown()
        return super().closeEvent(event)

    def on_open_project(self) -> None:
        final_title: str
        project_desc = di_get(ApplicationContext).project_desc

        if project_desc is not None:
            final_title = WINDOW_TITLE.format(project_desc.name)
        else:
            final_title = WINDOW_TITLE.format(NO_PROJECT_LOADED)

        self.setWindowTitle(final_title)
