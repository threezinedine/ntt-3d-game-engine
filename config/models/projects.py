from .project import *
from dataclasses import dataclass, field


@dataclass
class Projects:
    cppProjects: list[CPPProject] = field(default_factory=list)  # type: ignore
    pythonProjects: list[PythonProject] = field(default_factory=list)  # type: ignore
    commands: dict[str, Command] = field(default_factory=dict)  # type: ignore
