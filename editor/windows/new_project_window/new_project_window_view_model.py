import os
from di import *
from .new_project_window_model import NewProjectWindowModel
from utils import *
from constants import *


@as_singleton
@as_dependencies(NewProjectWindowModel)
class NewProjectWindowViewModel:
    def __init__(
        self,
        model: NewProjectWindowModel,
    ) -> None:
        self.model = model
        self.project_name_signal = Signal()
        self.project_path_signal = Signal()

    def on_project_name_changed(self, name: str) -> None:
        self.model.project_name = name
        self.project_name_signal.emit()

    def on_project_path_changed(self, path: str) -> None:
        self.model.project_path = path
        self.project_path_signal.emit()

    @property
    def project_full_path(self) -> str:
        final_path: str

        if self.model.project_path == "":
            final_path = os.path.join("./", self.model.project_name, PROJECT_FILE)
        else:
            final_path = os.path.join(
                self.model.project_path, self.model.project_name, PROJECT_FILE
            )

        # query relative path if possible
        try:
            final_path = os.path.relpath(final_path, os.getcwd())
        except ValueError:
            pass

        return final_path

    def clear(self) -> None:
        self.model.project_name = ""
        self.model.project_path = ""
