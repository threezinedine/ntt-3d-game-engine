import common_models  # type: ignore
from PySide6.QtWidgets import QApplication

from Engine import *
from utils import *
from windows.main_window.main_window_view import EditorMainWindow
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
        int(LOG_HANDLER_TYPE_EDITOR) | int(LOG_HANDLER_TYPE_CONSOLE),
        LOG_TAG_MASK_ALL,
    )

    app = QApplication([])
    window = di_get(EditorMainWindow)
    editor_logger.info("Starting the application.")
    window.showMaximized()
    app.exec()

    Logger.Release()


if __name__ == "__main__":
    try:
        main()
    except Exception as e:
        editor_logger.exception(f"Unhandled exception: {e}")
