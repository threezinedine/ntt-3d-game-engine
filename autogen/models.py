from dataclasses import dataclass, field


@dataclass
class Binding:
    output: str = field(default="")
    input: str = field(default="")
    dependencies: list[str] | None = field(default=None)
    includes: list[str] | None = field(default=None)
    template: str = field(default="")


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
    bindings: list[Binding] | None = field(default=None)
