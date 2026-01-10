from di import *
from .model import EditorMainWindowModel


@as_transient
@as_dependencies(EditorMainWindowModel)
class EditorMainWindowViewModel:
    def __init__(self, model: EditorMainWindowModel) -> None:
        self.model = model
