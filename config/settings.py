import os
import json
from .utils import *
from dacite import from_dict
from .models import *


PROJECTS_FILE = "projects.json"


class Settings:
    """
    Automatically get the `projects.json` file settings and parse them into this
    instance for easy access.
    """

    def __init__(self) -> None:
        assert os.path.exists(PROJECTS_FILE), f"Could not find {PROJECTS_FILE} file."

        with open(PROJECTS_FILE, "r", encoding="utf-8") as file:
            data = json.load(file)

            self.Projects = from_dict(data_class=Projects, data=data)
            logger.debug(f"Loaded {PROJECTS_FILE} settings.")

    @property
    def runables(self) -> list[str]:
        runables: list[str] = []
        for project in self.Projects.pythonProjects:
            runables.append(project.relPath)
        for project in self.Projects.cppProjects:
            if project.executable != "":
                runables.append(project.relPath)
        return runables

    @property
    def buildables(self) -> list[str]:
        buildables: list[str] = []
        for project in self.Projects.cppProjects:
            buildables.append(project.relPath)
        return buildables

    def is_python_project(self, project: str) -> bool:
        """
        Check if the given project path corresponds to a Python project.

        :param project: The relative path to the project.
        :return: True if the project is a Python project, False otherwise.
        """
        for py_project in self.Projects.pythonProjects:
            if py_project.relPath == project:
                return True
        return False

    def is_cpp_project(self, project: str) -> bool:
        """
        Check if the given project path corresponds to a C++ project.

        :param project: The relative path to the project.
        :return: True if the project is a C++ project, False otherwise.
        """
        for cpp_project in self.Projects.cppProjects:
            if cpp_project.relPath == project:
                return True
        return False

    def get_cpp_project_executable(self, project: str) -> str:
        """
        Get the executable name for the given C++ project.

        :param project: The relative path to the C++ project.
        :return: The name of the executable for the project.
        :raises ValueError: If the project is not found or has no executable defined.
        """
        for cpp_project in self.Projects.cppProjects:
            if cpp_project.relPath == project:
                if cpp_project.executable != "":
                    return cpp_project.executable
                else:
                    raise ValueError(
                        f"No executable defined for C++ project: {project}"
                    )
        raise ValueError(f"C++ project not found: {project}")
