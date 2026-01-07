from dataclasses import dataclass, field


@dataclass
class CPPProject:
    relPath: str = field(default="")
    executable: str = field(default="")


@dataclass
class PythonProject:
    relPath: str = field(default="")
