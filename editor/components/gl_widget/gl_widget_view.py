from typing import Any
from PySide6.QtOpenGLWidgets import QOpenGLWidget
from PySide6.QtGui import QCloseEvent

from utils import *
from di import *

from .gl_widget_view_model import GLWidgetViewModel
from Engine import *


class GLWidget(QOpenGLWidget):
    def __init__(
        self,
        *args: Any,
        **kwargs: Any,
    ) -> None:
        super().__init__(*args, **kwargs)

        self.viewModel = di_get(GLWidgetViewModel)

        if self.viewModel.getSharedContext() is not None:
            self.setShareContext(self.viewModel.getSharedContext())  # type: ignore
        else:
            self.viewModel.setSharedContext(self.context())

        self.application = EditorApplication("test.txt")

    def initializeGL(self) -> None:
        editor_logger.debug("GLWidget: OpenGL initialized.")
        self.application.Start()

    def paintGL(self) -> None:
        editor_logger.debug("GLWidget: paintGL called.")
        self.application.Update(0)

    def resizeGL(self, w: int, h: int) -> None:
        editor_logger.debug(f"GLWidget: resizeGL called. New size: {w}x{h}")

    def closeEvent(self, event: QCloseEvent) -> None:
        self.application.Shutdown()
        super().closeEvent(event)
