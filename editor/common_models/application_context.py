from dataclasses import dataclass, field, asdict
from typing import List
from constants import APP_SETTINGS_FILE
import json


@dataclass
class AppVersion:
    major: int = field(default=1)
    minor: int = field(default=1)
    patch: int = field(default=0)


@dataclass
class ApplicationSetting:
    version: AppVersion = field(default_factory=AppVersion)
    recentProjects: List[str] = field(default_factory=lambda: [])


from Engine import *  # type: ignore
from utils import *
from di import *


@as_singleton
class ApplicationContext:
    def __init__(self) -> None:
        self.application: EditorApplication = EditorApplication()
        self.project_desc: ProjectDescription | None = None
        self.project_desc_signal = Signal()

        self.app_settings: ApplicationSetting | None = None
        self.app_settings_signal = Signal()

    def save_app_settings(self) -> None:
        assert self.app_settings is not None

        editor_logger.info("Saving application settings...")

        self.app_settings.recentProjects = self.app_settings.recentProjects[:10]

        with open(APP_SETTINGS_FILE, "w", encoding="utf-8") as f:
            json.dump(asdict(self.app_settings), f, indent=4)

        self.app_settings_signal.emit()

    def load_project_from_path(self, path: str) -> None:
        self.application.LoadProject(path)
        with open(path, "r") as f:
            project_json = f.read()
            project_desc = ProjectDescriptionFromJsonString(project_json)
        self.project_desc = project_desc
        self.project_desc_signal.emit()
