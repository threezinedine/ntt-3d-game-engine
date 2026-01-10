from PySide6.QtWidgets import QWidget
from typing import Any
from converted.log_widget import Ui_LogWidget
from di import *
from .log_widget_view_model import LogWidgetViewModel


@as_transient
@as_dependencies(LogWidgetViewModel)
class LogWidget(QWidget):
    def __init__(
        self, viewModel: LogWidgetViewModel, *args: Any, **kwargs: Any
    ) -> None:
        super().__init__(*args, **kwargs)

        self.ui = Ui_LogWidget()
        self.ui.setupUi(self)  # type: ignore

        self.viewModel = viewModel
