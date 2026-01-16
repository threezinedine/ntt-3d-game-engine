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
        self.shared = self.viewModel.model.shared

        if self.viewModel.getSharedContext() is not None:
            self.setShareContext(self.viewModel.getSharedContext())  # type: ignore
        else:
            self.viewModel.setSharedContext(self.context())

    def initializeGL(self) -> None:
        self.makeCurrent()
        editor_logger.debug("GLWidget: OpenGL initialized.")
        assert self.shared.application is not None
        if not self.shared.application.IsInitialized():
            self.shared.application.Start()

    def paintGL(self) -> None:
        editor_logger.debug("GLWidget: paintGL called.")
        if self.shared.application:
            self.shared.application.Update(
                0.016
            )  # Assuming a fixed timestep for simplicity

    def resizeGL(self, w: int, h: int) -> None:
        editor_logger.debug(f"GLWidget: resizeGL called. New size: {w}x{h}")

    def closeEvent(self, event: QCloseEvent) -> None:
        super().closeEvent(event)
