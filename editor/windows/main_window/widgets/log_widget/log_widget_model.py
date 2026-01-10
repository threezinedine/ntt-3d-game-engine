from di import *
from Engine import *


@as_singleton
class LogWidgetModel:
    def __init__(self) -> None:
        Logger.SetupEditorCallback(self.handleLogMessage)

    def handleLogMessage(self, record: LogRecord) -> None:
        print(record.message)
