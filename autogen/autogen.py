import json
from blueprint import generate_blueprints
from dacite import from_dict
from models import *


class Autogen:
    def __init__(self) -> None:
        with open("settings.json", "r") as f:
            settings_dict = json.load(f)
            self.settings: AutogenSettings = from_dict(
                data_class=AutogenSettings, data=settings_dict
            )

    def Execute(self) -> None:
        generate_blueprints(self.settings)
