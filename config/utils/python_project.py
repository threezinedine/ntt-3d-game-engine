from .command import *
import os
import shutil


def sync_python_project(project: str) -> None:
    """
    Synchronizes a Python project by installing its dependencies using pip.

    :param project: The path to the Python project directory. The path is relative to the root of the repository.
    """
    run_command(f"uv sync --quiet", directory=project)


def run_python_project(project: str, verbose: bool = False) -> None:
    """
    Runs a Python project.

    :param project: The path to the Python project directory. The path is relative to the root of the repository.
    """
    if verbose:
        run_command(f"uv --quiet run main.py --verbose", directory=project)
    else:
        run_command(f"uv --quiet run main.py", directory=project)


def clean_python_project(project: str) -> None:
    """
    Cleans the build files for a given project.

    :param project: The relative path to the project to clean.
    """
    shutil.rmtree(os.path.join(project, ".venv"), ignore_errors=True)
    shutil.rmtree(os.path.join(project, "build"), ignore_errors=True)
    shutil.rmtree(os.path.join(project, "dist"), ignore_errors=True)

    command_logger.info(f"Cleaned Python project at {project}.")
