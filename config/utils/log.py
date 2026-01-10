import logging
from typing import TextIO
from colorama import init, Fore


class CommandLoggerHandler(logging.StreamHandler[TextIO]):
    def __init__(self):
        super().__init__()
        init(autoreset=True)

    def emit(self, record: logging.LogRecord):
        color = Fore.WHITE
        if record.levelno == logging.INFO:
            color = Fore.GREEN
        elif record.levelno == logging.WARNING:
            color = Fore.YELLOW
        elif record.levelno == logging.ERROR:
            color = Fore.RED
        elif record.levelno == logging.DEBUG:
            color = Fore.CYAN
        elif record.levelno == logging.CRITICAL:
            color = Fore.MAGENTA

        assert self.formatter is not None
        print(f"{color}{self.formatter.format(record)}{Fore.RESET}")


class Logger:
    loggers: dict[str, logging.Logger] = {}

    @staticmethod
    def get_logger(name: str) -> logging.Logger:
        if name in Logger.loggers:
            return Logger.loggers[name]

        logger = logging.getLogger(name)

        handler = CommandLoggerHandler()
        formatter = logging.Formatter(
            "[%(name)7s] - [%(levelname)7s] - %(filename)20s:%(lineno)-4d - %(message)s"
        )
        handler.setFormatter(formatter)

        logger.addHandler(handler)

        Logger.loggers[name] = logger

        return logger


command_logger = Logger.get_logger("COMMAND")
editor_logger = Logger.get_logger("EDITOR")
autogen_logger = Logger.get_logger("AUTOGEN")
