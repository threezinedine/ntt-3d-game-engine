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
            command_logger.debug(f"Loaded {PROJECTS_FILE} settings.")

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

    @property
    def installables(self) -> list[str]:
        installables: list[str] = []
        for project in self.Projects.pythonProjects:
            if project.install is not None:
                installables.append(project.relPath)
        return installables

    @property
    def cleanable(self) -> list[str]:
        cleanable: list[str] = []
        for project in self.Projects.cppProjects:
            cleanable.append(project.relPath)
        for project in self.Projects.pythonProjects:
            cleanable.append(project.relPath)
        return cleanable

    @property
    def testable(self) -> list[str]:
        testable: list[str] = []
        for project in self.Projects.pythonProjects:
            if project.test is not None:
                testable.append(project.relPath)
        return testable

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

    def get_cpp_project(self, project: str) -> CPPProject:
        """
        Get the C++ project configuration for the given project path.

        :param project: The relative path to the C++ project.
        :return: The CppProject instance corresponding to the project.
        :raises ValueError: If the project is not found.
        """
        for cpp_project in self.Projects.cppProjects:
            if cpp_project.relPath == project:
                return cpp_project
        raise ValueError(f"C++ project not found: {project}")

    def get_python_project(self, project: str) -> PythonProject:
        """
        Get the Python project configuration for the given project path.

        :param project: The relative path to the Python project.
        :return: The PythonProject instance corresponding to the project.
        :raises ValueError: If the project is not found.
        """
        for py_project in self.Projects.pythonProjects:
            if py_project.relPath == project:
                return py_project
        raise ValueError(f"Python project not found: {project}")
