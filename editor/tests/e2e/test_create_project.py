import os
from constants import *
from unittest.mock import patch
from pytestqt import qtbot  # type: ignore
from pytestqt.qtbot import QtBot
from di import di_get
from windows.main_window import EditorMainWindow
from .e2e_utils import *


def test_create_window(
    setup_engine: EngineSetup,
    qtbot: QtBot,
) -> None:
    os.mkdir("/projects")

    widget = di_get(EditorMainWindow)
    qtbot.addWidget(widget)
    widget.show()
    assert widget.isVisible()
    assert not widget.newProjectWindow.isVisible()

    # Open the new project window and then close it
    widget.ui.actionNewProject.trigger()
    assert widget.newProjectWindow.isVisible()
    newProjectWindow = widget.newProjectWindow
    newProjectWindow.ui.confirmButtonBox.rejected.emit()
    assert not newProjectWindow.isVisible()

    # Try to create a new project with empty name -> should not close the window
    widget.ui.actionNewProject.trigger()
    assert widget.newProjectWindow.isVisible()
    newProjectWindow.ui.confirmButtonBox.accepted.emit()
    assert newProjectWindow.isVisible()  # Not closed when the project name is empty

    # Create a new project successfully
    newProjectWindow.ui.nameInput.setText("TestProject")
    with patch(
        "PySide6.QtWidgets.QFileDialog.getExistingDirectory", return_value="/projects"
    ):
        newProjectWindow.ui.browseButton.clicked.emit()
    assert not os.path.exists(os.path.join("/projects", "TestProject", PROJECT_FILE))
    newProjectWindow.ui.confirmButtonBox.accepted.emit()
    assert not newProjectWindow.isVisible()
    assert os.path.exists(os.path.join("/projects", "TestProject", PROJECT_FILE))

    # Cannot create a project that already exists
    widget.ui.actionNewProject.trigger()
    assert widget.newProjectWindow.isVisible()
    newProjectWindow.ui.nameInput.setText("TestProject")
    with patch(
        "PySide6.QtWidgets.QFileDialog.getExistingDirectory", return_value="/projects"
    ):
        newProjectWindow.ui.browseButton.clicked.emit()
    newProjectWindow.ui.confirmButtonBox.accepted.emit()
    assert newProjectWindow.isVisible()  # Not closed when the project already exists
