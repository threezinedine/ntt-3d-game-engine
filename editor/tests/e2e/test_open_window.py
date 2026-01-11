from pytestqt import qtbot  # type: ignore
from pytestqt.qtbot import QtBot
from di import di_get
from windows.main_window import EditorMainWindow
from .e2e_utils import *


def test_open_window(setup_engine: EngineSetup, qtbot: QtBot) -> None:
    widget = di_get(EditorMainWindow)
    qtbot.addWidget(widget)
    widget.show()
    assert widget.isVisible()
