import glob
import os
import subprocess
from models import *
from log import autogen_logger
from utils import *


def generate_commands(settings: AutogenSettings) -> list[str]:
    """
    Handling all templates defined in settings.json commands section.

    :param settings: AutogenSettings dataclass instance, containing list of commands to process.
    :return: List of input command file paths processed.
    """
    if settings.commands is None:
        return []

    processed_files: list[str] = []

    for command in settings.commands:
        processed_files.extend(_generate_command(command))

    return processed_files


def _generate_command(command: Command) -> list[str]:
    """
    Using template for auto-create output file from blueprint file.

    :param blueprint: Blueprint dataclass instance, containing file and output paths. If the input file is cached and unchanged, skip regeneration.
    :return: True if needa regeneration, False otherwise.
    """
    files = not isinstance(command.files, list) and [command.files] or command.files
    processed_files: list[str] = []

    for file in files:
        complete_file = os.path.join(BASE_DIR, file)
        autogen_logger.debug(
            f"Processing command files matching: {complete_file} {glob.glob(complete_file)}"
        )
        for file_path in glob.glob(complete_file):
            relative_path = os.path.relpath(file_path, BASE_DIR)

            is_cached = is_file_cached(relative_path)

            autogen_logger.debug(f"Cached status for {relative_path}: {is_cached}")

            if is_cached:
                autogen_logger.debug(
                    f"Command {relative_path} is cached and unchanged. Skipping generation."
                )
                continue

            final_command = render_template_str(
                command.command,
                INPUT=relative_path,
            )

            autogen_logger.debug(f"Generating command: {final_command}")

            subprocess.run(
                final_command,
                shell=True,
                check=True,
                cwd=BASE_DIR,
            )

            autogen_logger.info(
                f"Generated command: {relative_path} with command: {final_command}"
            )

            processed_files.append(relative_path)

    return list(set(processed_files))
