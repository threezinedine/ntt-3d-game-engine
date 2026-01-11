from dataclasses import dataclass, field
from typing import List


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
from utils.signal import Signal
from di import *


@as_singleton
class ApplicationContext:
    def __init__(self) -> None:
        self.application: EditorApplication = EditorApplication()
        self.project_desc: ProjectDescription | None = None
        self.project_desc_signal = Signal()

        self.app_settings: ApplicationSetting | None = None
        self.app_settings_signal = Signal()
