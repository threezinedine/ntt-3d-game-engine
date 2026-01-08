from dataclasses import dataclass, field


@dataclass
class Binding:
    pass


@dataclass
class Blueprint:
    file: str = field(default="")
    output: str = field(default="")


@dataclass
class AutogenSettings:
    blueprints: list[Blueprint] | None = field(default=None)
