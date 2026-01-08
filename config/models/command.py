from dataclasses import dataclass, field


@dataclass
class Command:
    command: str = field(default="")
    cwd: str | None = field(default=None)
