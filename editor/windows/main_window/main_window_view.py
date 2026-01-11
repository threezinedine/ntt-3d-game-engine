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
from Engine import *  # type: ignore
from functools import partial


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
        self.viewModel.load_default_project()

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
        di_get(ApplicationContext).app_settings_signal.connect(
            self.on_app_settings_changed, True
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

    def on_app_settings_changed(self) -> None:
        app_settings = di_get(ApplicationContext).app_settings

        if app_settings is None or len(app_settings.recentProjects) == 0:
            editor_logger.info(f"No recent projects")

        self.ui.menuRecents.clear()

        assert app_settings is not None
        for recent_path in app_settings.recentProjects:
            action = self.ui.menuRecents.addAction(recent_path)

            def on_open_recent_project(checked: bool, path: str = recent_path) -> None:
                try:
                    app_context = di_get(ApplicationContext)
                    app_context.load_project_from_path(path)
                    editor_logger.info(f"Opened recent project at path: {path}")
                    app_settings.recentProjects.remove(path)
                    app_settings.recentProjects.insert(0, path)
                    app_context.save_app_settings()
                except Exception as e:
                    editor_logger.error(
                        f"Failed to open recent project at path: {path} with error: {e}"
                    )
                    app_settings.recentProjects.remove(path)

                di_get(ApplicationContext).save_app_settings()

            action.triggered.connect(
                partial(on_open_recent_project, False, recent_path)
            )
