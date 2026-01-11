import os
from PySide6.QtGui import QCloseEvent, QKeyEvent
from common_models.application_context import ApplicationContext
from di import *
from PySide6.QtWidgets import QMainWindow
from converted.new_project import Ui_NewProjectWindow
from PySide6.QtWidgets import QFileDialog
from PySide6.QtCore import Qt

from .new_project_window_view_model import NewProjectWindowViewModel
from pyqttoast import Toast, ToastPreset, ToastPosition  # type: ignore

from Engine import *


@as_singleton
@as_dependencies(NewProjectWindowViewModel)
class NewProjectWindowView(QMainWindow):
    def __init__(
        self,
        viewModel: NewProjectWindowViewModel,
        *args: Any,
        **kwargs: Any,
    ) -> None:
        super().__init__(*args, **kwargs)

        self.viewModel = viewModel

        self.ui = Ui_NewProjectWindow()
        self.ui.setupUi(self)  # type: ignore

        self.setup()

    def setup(self) -> None:
        self.ui.confirmButtonBox.rejected.connect(self.hide)
        self.ui.confirmButtonBox.accepted.connect(self.on_create_project)

        self.ui.nameInput.textChanged.connect(self.viewModel.on_project_name_changed)
        self.ui.browseButton.clicked.connect(self.choose_path)

        self.viewModel.project_name_signal.connect(self.on_final_path_changed)
        self.viewModel.project_path_signal.connect(self.on_final_path_changed, True)

    def choose_path(self) -> None:
        folder = QFileDialog.getExistingDirectory(
            self,
            "Select Project Directory",
            "",
            QFileDialog.Option.ShowDirsOnly | QFileDialog.Option.DontResolveSymlinks,
        )

        if folder:
            self.viewModel.on_project_path_changed(folder)

    def on_final_path_changed(self) -> None:
        self.ui.finalProject.setText(
            f"Final Project Path: {self.viewModel.project_full_path}"
        )

    def on_project_path_changed(self) -> None:
        self.ui.pathInput.setText(self.viewModel.model.project_path)

    def on_create_project(self) -> None:
        if self.viewModel.model.project_name == "":
            assert self.parentWidget is not None
            toast = Toast(self.parentWidget())
            toast.setDuration(2000)
            toast.setText("Project name cannot be empty.")
            toast.applyPreset(ToastPreset.ERROR)
            toast.setPosition(ToastPosition.TOP_RIGHT)
            toast.show()
            return

        if os.path.exists(self.viewModel.project_full_path):
            assert self.parentWidget is not None
            toast = Toast(self.parentWidget())
            toast.setDuration(2000)
            toast.setText("Project already exists at the specified location.")
            toast.applyPreset(ToastPreset.ERROR)
            toast.setPosition(ToastPosition.TOP_RIGHT)
            toast.show()
            return

        # TODO: Update later with editor setting
        desc = ProjectDescription()
        desc.name = self.viewModel.model.project_name
        desc.version.major = 1
        desc.version.minor = 0
        desc.version.patch = 0

        os.makedirs(os.path.dirname(self.viewModel.project_full_path), exist_ok=True)

        with open(self.viewModel.project_full_path, "w") as f:
            f.write(ProjectDescriptionToJsonString(desc))

        appContext = di_get(ApplicationContext)
        appContext.application.LoadProject(self.viewModel.project_full_path)
        appContext.project_desc = desc
        appContext.project_desc_signal.emit()

        self.hide()

    def keyPressEvent(self, event: QKeyEvent) -> None:
        if event.key() == Qt.Key.Key_Escape:
            self.hide()
        return super().keyPressEvent(event)

    def closeEvent(self, event: QCloseEvent) -> None:
        self.viewModel.clear()
        return super().closeEvent(event)
