import os
from .command import run_command
from ..constants import *
from typing import Any
import shutil
from .log import *


def _get_target_folder(
    type: str,
    platform: str,
    generator: str | None = None,
) -> str:
    """
    Used for getting the target build folder for C++ projects.
    :param type: The type of build files to generate (must be in "debug", "release", "test").
    :param platform: The target platform for the build files (must be in "windows", "linux", "macos", "web", "android").
    :param generator: The CMake generator to use (if None, the default generator will be used).
    :return: The target build folder path.
    """

    assert (
        type.lower() in CPP_BUILD_TYPES
    ), f"Invalid build type: {type}, must be one of {CPP_BUILD_TYPES}"
    assert (
        platform.lower() in CPP_BUILD_PLATFORMS
    ), f"Invalid build platform: {platform}, must be one of {CPP_BUILD_PLATFORMS}"

    if generator is not None:
        safe_generator = generator.lower().replace(" ", "_").replace("-", "_")
        return f"build/{platform.lower()}_{type.lower()}_{safe_generator}"

    if platform.lower() != "windows":
        return f"build/{platform.lower()}_{type.lower()}"

    raise NotImplementedError(
        f"Target folder generation not implemented for platform: {platform}"
    )


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

    target_folder = _get_target_folder(type, platform, generator)

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
    generator: str | None = None,
    **kwargs: Any,
) -> None:
    """
    Builds a C++ project using CMake.

    :param project: The path to the C++ project directory. The path is relative to the root of the repository.
    :param type: The type of build to perform (must be in "debug", "release", "test").
    :param platform: The target platform for the build (must be in "windows", "linux", "macos", "web", "android").
    :param generator: The CMake generator to use (if None, the default generator will be used).
    """
    target_folder = _get_target_folder(type, platform, generator)

    final_command = f"cmake --build {target_folder} --config {type.capitalize()}"

    run_command(final_command, directory=project)


def run_cpp_project(
    project: str,
    type: str,
    platform: str,
    generator: str | None = None,
    executable: str | None = None,
    **kwargs: Any,
) -> None:
    """
    Runs a built C++ project executable.

    :param project: The path to the C++ project directory. The path is relative to the root of the repository.
    :param type: The type of build to run (must be in "debug", "release", "test").
    :param platform: The target platform for the build (must be in "windows", "linux", "macos", "web", "android").
    :param generator: The CMake generator used to build the project (if None, the default generator will be used).
    :param executable: The name of the executable to run (if None, the default executable name will be used).
    """
    target_folder = _get_target_folder(type, platform, generator)

    if executable is not None:
        executable_name = executable
    else:
        executable_name = os.path.basename(os.path.abspath(project))

    if platform.lower() == "windows":
        executable_name += ".exe"

    if generator is not None and "Visual Studio" in generator:
        executable_path = os.path.join(
            target_folder, type.capitalize(), executable_name
        )
    else:
        executable_path = os.path.join(target_folder, executable_name)

    assert os.path.exists(
        os.path.join(project, executable_path)
    ), f"Executable not found at {executable_path}. Please build the project first."

    command_logger.info(f"Running executable: {executable_path} at {project}")
    run_command(executable_path, directory=project)


def clean_cpp_project(project: str, **kwargs: Any) -> None:
    """
    Cleans the build files for a given C++ project.

    :param project: The relative path to the C++ project to clean.
    :param type: The type of build files to clean (must be in "debug", "release", "test").
    :param platform: The target platform for the build files (must be in "windows", "linux", "macos", "web", "android").
    :param generator: The CMake generator used to build the project (if None, the default generator will be used).
    """
    build_path = os.path.join(project, "build")

    if os.path.exists(build_path):
        shutil.rmtree(build_path)
        command_logger.info(f"Cleaned build files at {build_path}")
    else:
        command_logger.info(f"No build files found at {build_path} to clean.")
