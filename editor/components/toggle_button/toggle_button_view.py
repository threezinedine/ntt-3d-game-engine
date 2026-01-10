from di import *
from .toggle_button_viewmodel import ToggleButtonViewModel
from PySide6.QtWidgets import QPushButton


@as_transient
@as_dependencies(ToggleButtonViewModel)
class ToggleButton(QPushButton):
    def __init__(
        self, viewModel: ToggleButtonViewModel, *args: Any, **kwargs: Any
    ) -> None:
        super().__init__(*args, **kwargs)

        self.viewModel = viewModel
        self.setCheckable(True)

        self.setStyleSheet(
            """
            QPushButton {
                background-color: transparent;
                border: none;
                padding: 5px;
                border-radius: 3px;
            }

            QPushButton:hover {
                background-color: rgba(100, 100, 100, 30);
            }

            QPushButton:checked {
                background-color: rgba(100, 100, 100, 50);
                border: 1px solid rgba(150, 150, 150, 80);
            }
            """
        )
