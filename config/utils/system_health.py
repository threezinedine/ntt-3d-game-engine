from .command import *
from .log import *


def check_system_command(command: str, required: bool = True) -> None:
    """
    Checks if a given command is available on the system. If `required` is True and the command is not found,
    an exception is raised.

    :param command: The command to check.
    :param required: If True, raises an exception if the command is not found.
    """
    try:
        subprocess.run(
            f"{command} --version",
            shell=True,
            stdout=subprocess.DEVNULL,
            stderr=subprocess.DEVNULL,
            check=True,
        )
        logger.debug(f"Command '{command}' is available on the system.")
    except subprocess.CalledProcessError:
        if required:
            raise EnvironmentError(
                f"Required command '{command}' is not available on the system."
            )
        else:
            logger.warning(
                f"Optional command '{command}' is not available on the system."
            )
