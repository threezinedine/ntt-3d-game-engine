import os
from common_models.application_context import ApplicationContext
from di import *
from .new_project_window_model import NewProjectWindowModel
from utils import *
from constants import *
from Engine import *


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

    def create_project(self) -> bool:
        # TODO: Update later with editor setting
        desc = ProjectDescription()
        desc.name = self.model.project_name
        desc.version.major = 1
        desc.version.minor = 0
        desc.version.patch = 0

        os.makedirs(os.path.dirname(self.project_full_path), exist_ok=True)

        with open(self.project_full_path, "w") as f:
            f.write(ProjectDescriptionToJsonString(desc))

        appContext = di_get(ApplicationContext)
        appContext.application.LoadProject(self.project_full_path)
        appContext.project_desc = desc
        appContext.project_desc_signal.emit()

        editor_logger.info(f"Created new project at path: {self.project_full_path}")

        return True
