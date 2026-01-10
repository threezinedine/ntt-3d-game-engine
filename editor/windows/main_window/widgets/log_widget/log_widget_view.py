from PySide6.QtWidgets import QWidget, QTableWidgetItem
from PySide6.QtGui import QColor, QBrush

from typing import Any
from converted.log_widget import Ui_LogWidget
from di import *
from .log_widget_view_model import LogWidgetViewModel
from Engine import *


@as_singleton
@as_dependencies(LogWidgetViewModel)
class LogWidget(QWidget):
    def __init__(
        self, viewModel: LogWidgetViewModel, *args: Any, **kwargs: Any
    ) -> None:
        super().__init__(*args, **kwargs)

        self.ui = Ui_LogWidget()
        self.ui.setupUi(self)  # type: ignore

        self.viewModel = viewModel
        self.model = self.viewModel.model

        self._setup()

    def _setup(self) -> None:
        self.model.messagesSignal.connect(self.reload_logs)
        self.ui.levelFilterCombo.currentIndexChanged.connect(lambda: self.reload_logs())
        self.ui.clearLogButton.clicked.connect(self.viewModel.clear_logs)

    def reload_logs(self) -> None:
        self.ui.logTable.clearContents()
        self.ui.logTable.setRowCount(0)

        for i, log in enumerate(self.model.messages):
            if self.ui.levelFilterCombo.currentIndex() > int(log.level):
                continue

            self.ui.logTable.insertRow(i)

            bgColor = QColor(255, 255, 255)
            textColor = QColor(0, 0, 0)

            if log.level == LOG_LEVEL_ERROR:
                bgColor = QColor(255, 200, 200)
                textColor = QColor(150, 0, 0)
            elif log.level == LOG_LEVEL_WARN:
                bgColor = QColor(255, 255, 200)
                textColor = QColor(150, 150, 0)
            elif log.level == LOG_LEVEL_INFO:
                bgColor = QColor(200, 200, 255)
                textColor = QColor(0, 0, 150)
            elif log.level == LOG_LEVEL_DEBUG:
                bgColor = QColor(220, 220, 220)
                textColor = QColor(80, 80, 80)

            self.ui.logTable.setItem(
                i, 0, QTableWidgetItem(convertLoggerTagToString(log.tag))
            )
            col = self.ui.logTable.item(i, 0)
            assert col is not None
            col.setBackground(QBrush(bgColor))
            col.setForeground(QBrush(textColor))

            self.ui.logTable.setItem(
                i, 1, QTableWidgetItem(convertLoggerLevelToString(log.level))
            )
            col = self.ui.logTable.item(i, 1)
            assert col is not None
            col.setBackground(QBrush(bgColor))
            col.setForeground(QBrush(textColor))

            self.ui.logTable.setItem(i, 2, QTableWidgetItem(log.file))
            col = self.ui.logTable.item(i, 2)
            assert col is not None
            col.setBackground(QBrush(bgColor))
            col.setForeground(QBrush(textColor))

            self.ui.logTable.setItem(i, 3, QTableWidgetItem(log.line))
            col = self.ui.logTable.item(i, 3)
            assert col is not None
            col.setBackground(QBrush(bgColor))
            col.setForeground(QBrush(textColor))

            self.ui.logTable.setItem(i, 4, QTableWidgetItem(log.message))
            col = self.ui.logTable.item(i, 4)
            assert col is not None
            col.setBackground(QBrush(bgColor))
            col.setForeground(QBrush(textColor))
