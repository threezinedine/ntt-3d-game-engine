from di import *
from Engine import *
from utils.signal import Signal


@as_singleton
class ApplicationContext:
    def __init__(self) -> None:
        self.application: EditorApplication = EditorApplication()
        self.project_desc: ProjectDescription | None = None
        self.project_desc_signal = Signal()
