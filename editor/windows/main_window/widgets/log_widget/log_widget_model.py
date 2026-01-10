from di import *
from Engine import *


@as_singleton
class LogWidgetModel:
    def __init__(self) -> None:
        Logger.SetupEditorCallback(self.handleLogMessage)
        self.messges: list[LogRecord] = []

    def handleLogMessage(self, record: LogRecord) -> None:
        self.messges.append(record)
