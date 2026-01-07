import os
from .command import run_command
from ..constants import *
from typing import Any
import shutil


def generate_cpp_project(
    project: str,
    type: str,
    platform: str,
    generator: str | None = None,
    reload: bool = False,
    **kwargs: Any,
) -> None:
    """
    Generates build files for a C++ project using CMake.

    :param project: The path to the C++ project directory. The path is relative to the root of the repository.
    :param type: The type of build files to generate (must be in "debug", "release", "test").
    :param platform: The target platform for the build files (must be in "windows", "linux", "macos", "web", "android").
    :param generator: The CMake generator to use (if None, the default generator will be used).
    :param reload: Whether to regenerate the build files even if they already exist.
    """
    assert (
        type.lower() in CPP_BUILD_TYPES
    ), f"Invalid build type: {type}, must be one of {CPP_BUILD_TYPES}"
    assert (
        platform.lower() in CPP_BUILD_PLATFORMS
    ), f"Invalid build platform: {platform}, must be one of {CPP_BUILD_PLATFORMS}"

    target_folder = f"build/{platform.lower()}_{type.lower()}"

    if reload:
        shutil.rmtree(os.path.join(project, target_folder), ignore_errors=True)

    final_command = f"cmake -S . -B {target_folder}"

    if generator is not None:
        final_command += f' -G "{generator}"'

    run_command(final_command, directory=project)


def build_cpp_project(
    project: str,
    type: str,
    platform: str,
    **kwargs: Any,
) -> None:
    """
    Builds a C++ project using CMake.

    :param project: The path to the C++ project directory. The path is relative to the root of the repository.
    :param type: The type of build to perform (must be in "debug", "release", "test").
    :param platform: The target platform for the build (must be in "windows", "linux", "macos", "web", "android").
    """
    assert (
        type.lower() in CPP_BUILD_TYPES
    ), f"Invalid build type: {type}, must be one of {CPP_BUILD_TYPES}"
    assert (
        platform.lower() in CPP_BUILD_PLATFORMS
    ), f"Invalid build platform: {platform}, must be one of {CPP_BUILD_PLATFORMS}"

    target_folder = f"build/{platform.lower()}_{type.lower()}"

    final_command = f"cmake --build {target_folder} --config {type.capitalize()}"

    run_command(final_command, directory=project)
