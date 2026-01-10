from di import *
from dataclasses import dataclass, field


@dataclass
class Version:
    major: int = field(default=1)
    minor: int = field(default=1)
    patch: int = field(default=0)


@as_singleton
@dataclass
class AppSettings:
    version: Version = field(default_factory=Version)
