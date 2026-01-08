from .command import *


def sync_python_project(project: str) -> None:
    """
    Synchronizes a Python project by installing its dependencies using pip.

    :param project: The path to the Python project directory. The path is relative to the root of the repository.
    """
    run_command(f"uv sync --quiet", directory=project)


def run_python_project(project: str) -> None:
    """
    Runs a Python project.

    :param project: The path to the Python project directory. The path is relative to the root of the repository.
    """
    run_command(f"uv --quiet run main.py", directory=project)
