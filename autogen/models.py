from dataclasses import dataclass, field


@dataclass
class Binding:
    pass


@dataclass
class Blueprint:
    file: str = field(default="")
    output: str = field(default="")


@dataclass
class Command:
    files: str | list[str] = field(default="")
    command: str = field(default="")


@dataclass
class AutogenSettings:
    blueprints: list[Blueprint] | None = field(default=None)
    commands: list[Command] | None = field(default=None)
