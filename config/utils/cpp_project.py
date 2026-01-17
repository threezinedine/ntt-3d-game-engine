import os
from .command import run_command
from ..constants import *
from typing import Any
import shutil
from .log import *
from configparser import ConfigParser


def _get_target_folder(
    type: str,
    platform: str,
    generator: str | None,
    sections: list[str] | None,
) -> tuple[str, list[str]]:
    """
    Used for getting the target build folder for C++ projects.
    :param type: The type of build files to generate (must be in "debug", "release", "test").
    :param platform: The target platform for the build files (must be in "windows", "linux", "macos", "web", "android").
    :param generator: The CMake generator to use (if None, the default generator will be used).
    :param sections: Additional configuration sections to include.
    :return: The target build folder path.
    """

    assert (
        type.lower() in CPP_BUILD_TYPES
    ), f"Invalid build type: {type}, must be one of {CPP_BUILD_TYPES}"
    assert (
        platform.lower() in CPP_BUILD_PLATFORMS
    ), f"Invalid build platform: {platform}, must be one of {CPP_BUILD_PLATFORMS}"

    assert (
        generator is None or generator.lower() in CPP_GENERATORS
    ), f"Invalid build generator: {generator}, must be one of {CPP_GENERATORS}"

    tempSections = list(set(sections.copy())) if sections is not None else []
    tempSections.remove("common") if "common" in tempSections else None
    tempSections.remove(type.lower()) if type.lower() in tempSections else None
    tempSections.remove(platform.lower()) if platform.lower() in tempSections else None

    tempSections.insert(0, type.lower())
    tempSections.insert(1, platform.lower())
    if generator is not None:
        tempSections.insert(2, generator.lower())

    string_builder = "_".join(tempSections)
    assert string_builder != "", "Target folder string cannot be empty."

    target_folder = f"build/{string_builder}"

    return target_folder, tempSections


def generate_cpp_project(
    project: str,
    type: str,
    platform: str,
    generator: str | None = None,
    reload: bool = False,
    sections: list[str] | None = None,
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

    target_folder, _ = _get_target_folder(type, platform, generator, sections)

    if reload:
        shutil.rmtree(os.path.join(project, target_folder), ignore_errors=True)

    final_command = f"cmake -Wno-dev -S . -B {target_folder}"

    options = _get_config_values(project, type, platform, generator, sections)

    for key, value in options.items():
        final_command += f" -D{key.upper()}={value}"

    if generator is not None:
        if generator.lower() == "msvc":
            final_command += ' -G "Visual Studio 17 2022"'
        elif generator.lower() == "mingw":
            final_command += ' -G "MinGW Makefiles"'

    run_command(final_command, directory=project)


def build_cpp_project(
    project: str,
    type: str,
    platform: str,
    generator: str | None = None,
    sections: list[str] | None = None,
    **kwargs: Any,
) -> None:
    """
    Builds a C++ project using CMake.

    :param project: The path to the C++ project directory. The path is relative to the root of the repository.
    :param type: The type of build to perform (must be in "debug", "release", "test").
    :param platform: The target platform for the build (must be in "windows", "linux", "macos", "web", "android").
    :param generator: The CMake generator to use (if None, the default generator will be used).
    """
    target_folder, _ = _get_target_folder(type, platform, generator, sections)

    final_command = (
        f"cmake --build {target_folder} --config {type.capitalize()} --parallel 5"
    )

    run_command(final_command, directory=project)


def run_cpp_executable(
    project: str,
    type: str,
    platform: str,
    generator: str | None = None,
    executable: str | None = None,
    sections: list[str] | None = None,
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
    target_folder, _ = _get_target_folder(type, platform, generator, sections)

    if executable is not None:
        executable_name = executable
    else:
        executable_name = os.path.basename(os.path.abspath(project))

    if platform.lower() == "windows":
        executable_name += ".exe"
    else:
        executable_name = "./" + executable_name

    if generator is not None and "Visual Studio" in generator:
        target_folder = os.path.join(target_folder, type.capitalize())

    final_cwd = os.path.join(project, target_folder)

    assert os.path.exists(
        os.path.join(final_cwd, executable_name)
    ), f"Executable not found at {os.path.join(final_cwd, executable_name)}. Please build the project first."

    command_logger.info(f"Running executable: {executable_name} at {final_cwd}")
    run_command(executable_name, directory=final_cwd)


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


def _get_config_values(
    project: str,
    type: str,
    platform: str,
    generator: str | None = None,
    sections: list[str] | None = None,
) -> dict[str, str]:
    """
    Retrieves configuration values from config.cfg for the specified build type.

    :param project: The path to the C++ project directory. The path is relative to the root of the repository.
    :param type: The type of build files to generate (must be in "debug", "release", "test").
    :param platform: The target platform for the build files (must be in "windows", "linux", "macos", "web", "android").
    :param generator: The CMake generator to use (if None, the default generator will be used).
    :param sections: Additional configuration sections to include.
    :return: A dictionary of configuration key-value pairs.
    """

    global_config_path = os.path.join(BASE_DIR, "config.cfg")
    global_config = ConfigParser()
    global_config.read(global_config_path)

    config_path = os.path.join(project, "config.cfg")
    global_config.read(config_path)

    configs = [global_config, global_config]

    _, sections = _get_target_folder(
        type, platform, generator, sections
    )  # Validate inputs

    final_config: dict[str, str] = {}

    for config in configs:
        if "common" in config:
            final_config.update(config["common"])

        command_logger.debug(f"Loading configuration sections: {sections}")

        for section in sections:
            if section in config:
                final_config.update(config[section])

    return final_config
