from dataclasses import dataclass, field
from .command import *


@dataclass
class CPPProject:
    relPath: str = field(default="")
    run: str | None = field(default=None)
    test: str | None = field(default=None)
    cleans: list[str] | None = field(default=None)


@dataclass
class PythonProject:
    relPath: str = field(default="")
    preRun: Command | None = field(default=None)
    preInstall: Command | None = field(default=None)
    install: Command | None = field(default=None)
    afterInstall: Command | None = field(default=None)
    test: Command | None = field(default=None)
    cleans: list[str] | None = field(default=None)
