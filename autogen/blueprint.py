import os
from models import *
from log import autogen_logger
from constants import *


def generate_blueprints(settings: AutogenSettings) -> None:
    if settings.blueprints is None:
        return

    for blueprint in settings.blueprints:
        _generate_blueprint(blueprint)


def _generate_blueprint(blueprint: Blueprint) -> None:
    complete_input = os.path.join(BASE_DIR, blueprint.file)
    complete_output = os.path.join(BASE_DIR, blueprint.output)

    assert os.path.exists(
        complete_input
    ), f"Blueprint file {blueprint.file} does not exist."

    with open(complete_input, "r") as f:
        content = f.read()

    with open(complete_output, "w") as f:
        f.write(content)

    autogen_logger.info(
        f"Generated blueprint: {blueprint.output} from {blueprint.file}"
    )
