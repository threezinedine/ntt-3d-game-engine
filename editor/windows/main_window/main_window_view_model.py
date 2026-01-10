from di import *
from .main_window_model import EditorMainWindowModel


@as_transient
@as_dependencies(EditorMainWindowModel)
class EditorMainWindowViewModel:
    def __init__(self, model: EditorMainWindowModel) -> None:
        self.model = model
