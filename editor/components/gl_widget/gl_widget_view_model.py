from di import *
from .gl_widget_model import GLWidgetModel

from PySide6.QtGui import QOpenGLContext


@as_transient
@as_dependencies(GLWidgetModel)
class GLWidgetViewModel:
    def __init__(self, model: GLWidgetModel) -> None:
        self.model = model

    def setSharedContext(self, context: QOpenGLContext) -> None:
        self.model.shared.context = context

    def getSharedContext(self) -> QOpenGLContext | None:
        return self.model.shared.context
