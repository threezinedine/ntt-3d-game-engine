from di import *


@as_singleton
class NewProjectWindowModel:
    def __init__(self) -> None:
        self.project_name: str = ""
        self.project_path: str = ""
