from .project import *
from dataclasses import dataclass, field


@dataclass
class Projects:
    cppProjects: list[CPPProject] = field(default_factory=list)  # type: ignore
    pythonProjects: list[PythonProject] = field(default_factory=list)  # type: ignore
