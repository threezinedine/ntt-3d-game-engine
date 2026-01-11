from di import *
from .gl_shared import GLShared


@as_transient
@as_dependencies(GLShared)
class GLWidgetModel:
    def __init__(self, shared: GLShared) -> None:
        self.shared = shared
