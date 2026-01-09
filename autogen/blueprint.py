import os
from models import *
from log import autogen_logger
from utils import *


def generate_blueprints(settings: AutogenSettings) -> list[str]:
    """
    Handling all templates defined in settings.json blueprints section.

    :param settings: AutogenSettings dataclass instance, containing list of blueprints to process.
    :return: List of input blueprint file paths processed.
    """
    if settings.blueprints is None:
        return []

    processed_files: list[str] = []

    for blueprint in settings.blueprints:
        if _generate_blueprint(blueprint):
            processed_files.append(blueprint.file)

    return processed_files


def _generate_blueprint(blueprint: Blueprint) -> bool:
    """
    Using template for auto-create output file from blueprint file.

    :param blueprint: Blueprint dataclass instance, containing file and output paths. If the input file is cached and unchanged, skip regeneration.
    :return: True if needa regeneration, False otherwise.
    """
    complete_input = os.path.join(BASE_DIR, blueprint.file)
    complete_output = os.path.join(BASE_DIR, blueprint.output)

    assert os.path.exists(
        complete_input
    ), f"Blueprint file {blueprint.file} does not exist."

    is_cached = is_file_cached(blueprint.file)

    autogen_logger.debug(f"Cached status for {blueprint.file}: {is_cached}")

    if is_cached and os.path.exists(complete_output):
        autogen_logger.debug(
            f"Blueprint {blueprint.file} is cached and unchanged. Skipping generation."
        )
        return False

    with open(complete_input, "r") as f:
        content = f.read()

    output_content = render_template_str(
        content,
        INPUT=blueprint.file,
        OUTPUT=blueprint.output,
    )

    with open(complete_output, "w") as f:
        f.write(output_content)

    autogen_logger.info(
        f"Generated blueprint: {blueprint.output} from {blueprint.file}"
    )

    return True
