import os
from constants import BASE_DIR


def _get_stamp_path(file: str) -> str:
    """
    Get the path to the stamp file for a given file.

    :param file: The original file path, relative to BASE_DIR.
    """
    return os.path.join("temp", f"{file}.stamp")


def is_file_cached(file: str) -> bool:
    """
    Check if a file is cached by verifying the existence of its stamp file.

    :param file: The original file path, relative to BASE_DIR.
    :return: True if the stamp file exists, False otherwise.
    """
    stamp_path = _get_stamp_path(file)
    complete_file = os.path.join(BASE_DIR, file)
    return os.path.exists(stamp_path) and os.path.getmtime(
        stamp_path
    ) >= os.path.getmtime(complete_file)


def update_file_cache(file: str) -> None:
    """
    Update the cache for a given file by creating or updating its stamp file.

    :param file: The original file path, relative to BASE_DIR.
    """
    stamp_path = _get_stamp_path(file)
    os.makedirs(os.path.dirname(stamp_path), exist_ok=True)
    with open(stamp_path, "w") as f:
        f.write("")
