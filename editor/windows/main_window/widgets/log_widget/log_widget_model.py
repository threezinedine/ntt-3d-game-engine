from di import *
from Engine import *
from utils import *


@as_singleton
class LogWidgetModel:
    def __init__(self) -> None:
        Logger.SetupEditorCallback(self.handleLogMessage)
        self.messages: list[LogRecord] = []
        self.messagesSignal = Signal()

    def handleLogMessage(self, record: LogRecord) -> None:
        self.messages.append(record)
        self.messagesSignal.emit()
