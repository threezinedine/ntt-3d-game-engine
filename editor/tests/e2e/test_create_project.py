from pytestqt import qtbot  # type: ignore
from pytestqt.qtbot import QtBot
from di import di_get
from windows.main_window import EditorMainWindow
from .e2e_utils import *


def test_create_window(setup_engine: EngineSetup, qtbot: QtBot) -> None:
    widget = di_get(EditorMainWindow)
    qtbot.addWidget(widget)
    widget.show()
    assert widget.isVisible()
    assert not widget.newProjectWindow.isVisible()

    widget.ui.actionNewProject.trigger()

    assert widget.newProjectWindow.isVisible()

    newProjectWindow = widget.newProjectWindow

    newProjectWindow.ui.confirmButtonBox.rejected.emit()

    assert not newProjectWindow.isVisible()

    widget.ui.actionNewProject.trigger()
    assert widget.newProjectWindow.isVisible()

    newProjectWindow.ui.confirmButtonBox.accepted.emit()
    assert newProjectWindow.isVisible()  # Not closed when the project name is empty
