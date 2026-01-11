from di import *
from Engine import *


@as_singleton
class ApplicationContext:
    def __init__(self) -> None:
        self.application: EditorApplication = EditorApplication()
