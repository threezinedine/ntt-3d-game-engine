from PySide6.QtWidgets import QApplication
from Engine import *
from utils import *
from windows.main_window import EditorMainWindow
from di import *
from argparse import ArgumentParser


def main():
    parser = ArgumentParser()
    parser.add_argument(
        "-v",
        "--verbose",
        action="store_true",
        help="Enable verbose logging",
    )

    if parser.parse_args().verbose:
        editor_logger.setLevel(logging.DEBUG)
        level = LOG_LEVEL_DEBUG
    else:
        editor_logger.setLevel(logging.INFO)
        level = LOG_LEVEL_INFO

    Logger.Setup(
        level,
        "",
        LOG_HANDLER_TYPE_EDITOR,
        LOG_TAG_MASK_ALL,
    )

    app = QApplication([])
    window = di_get(EditorMainWindow)
    editor_logger.info("Starting the application.")
    window.showMaximized()
    app.exec()


if __name__ == "__main__":
    main()
