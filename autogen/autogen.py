import json
from blueprint import generate_blueprints
from command import generate_commands
from dacite import from_dict
from models import *
from utils import *


class Autogen:
    def __init__(self) -> None:
        with open("settings.json", "r") as f:
            settings_dict = json.load(f)
            self.settings: AutogenSettings = from_dict(
                data_class=AutogenSettings, data=settings_dict
            )

    def Execute(self) -> None:
        final_processed_files: set[str] = set()

        processed_files = generate_blueprints(self.settings)
        final_processed_files.update(processed_files)

        processed_files = generate_commands(self.settings)
        final_processed_files.update(processed_files)

        for file in final_processed_files:
            update_file_cache(file)
