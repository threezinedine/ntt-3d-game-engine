from argparse import ArgumentParser
from .settings import Settings
from .utils import *


class Args:
    """
    The supply command line arguments for helper.py script. The `projects.json` file
    will be read and parsed according to the arguments provided, this instance will
    hold those arguments and the user can interact via the command line.
    """

    def __init__(self) -> None:
        parser = ArgumentParser(description="NTT 3D Game Engine")

        parser.add_argument(
            "-v",
            "--verbose",
            action="store_true",
        )

        self.settings = Settings()

        subparsers = parser.add_subparsers(dest="command", required=True)

        run_parser = subparsers.add_parser(
            "run",
            help="Run a specified python project from the projects.json file.",
        )

        run_parser.add_argument(
            "project",
            type=str,
            choices=self.settings.runables,
            help="The relative path to the python project to run.",
        )

        build_parser = subparsers.add_parser(
            "build",
            help="Build a specified cpp project from the projects.json file.",
        )

        build_parser.add_argument(
            "project",
            type=str,
            choices=self.settings.buildables,
            help="The relative path to the cpp project to build.",
        )

        build_parser.add_argument(
            "-r",
            "--reload",
            action="store_true",
            help="Regenerate the build files before building the project.",
        )

        build_parser.add_argument(
            "-t",
            "--type",
            type=str,
            default=CPP_BUILD_TYPES[0],
            choices=CPP_BUILD_TYPES,
            help="The type of build to perform (debug, release, test). Default is debug.",
        )

        build_parser.add_argument(
            "-p",
            "--platform",
            type=str,
            default=CPP_BUILD_PLATFORMS[0],
            choices=CPP_BUILD_PLATFORMS,
            help="The target platform for the build (windows, linux, macos, web, android). Default is linux.",
        )

        self.args = parser.parse_args()

    @property
    def verbose(self) -> bool:
        return self.args.verbose

    @property
    def vars(self) -> dict[str, Any]:
        return vars(self.args)

    def execute(self) -> None:
        """
        Process the command line arguments and execute the appropriate actions.
        """
        if self.args.command == "run":
            if self.settings.is_python_project(self.args.project):
                logger.debug(f"Syncing {self.args.project}...")
                sync_python_project(self.args.project)
                logger.debug(f"Running {self.args.project}...")
                run_python_project(self.args.project)

        elif self.args.command == "build":
            if self.settings.is_cpp_project(self.args.project):
                logger.debug(f"Building {self.args.project}...")
                generate_cpp_project(**self.vars)
                build_cpp_project(**self.vars)
