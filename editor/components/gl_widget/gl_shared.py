from di import *
from PySide6.QtGui import QOpenGLContext
from Engine import EditorApplication


@as_singleton
class GLShared:
    def __init__(self) -> None:
        self.context: QOpenGLContext | None = None
        self.application: EditorApplication | None = None
