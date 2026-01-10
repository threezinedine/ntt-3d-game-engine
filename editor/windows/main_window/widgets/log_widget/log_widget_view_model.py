from di import *
from .log_widget_model import LogWidgetModel


@as_transient
@as_dependencies(LogWidgetModel)
class LogWidgetViewModel:
    def __init__(self, model: LogWidgetModel) -> None:
        self.model = model
