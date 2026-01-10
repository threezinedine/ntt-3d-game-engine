from di import *
from .log_widget_model import LogWidgetModel
from Engine import *


@as_singleton
@as_dependencies(LogWidgetModel)
class LogWidgetViewModel:
    def __init__(self, model: LogWidgetModel) -> None:
        self.model = model

    def clear_logs(self) -> None:
        self.model.messages = []
        self.model.messagesSignal.emit()
