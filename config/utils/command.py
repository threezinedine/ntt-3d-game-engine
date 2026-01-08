import subprocess
from .log import *


def run_command(command: str, directory: str | None = None) -> str | None:
    """
    Executes a shell command and returns its output as a string (if needed).

    :param command: The shell command to execute.
    :param directory: The directory in which to execute the command. If None, uses the current working directory.
    """

    command_logger.debug(f"Executing command: {command} in directory: {directory}")

    subprocess.run(
        command,
        shell=True,
        check=True,
        cwd=directory,
    )

    return None
