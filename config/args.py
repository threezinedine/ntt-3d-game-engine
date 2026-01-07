from argparse import _SubParsersAction, ArgumentParser  # type: ignore
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
        self._build_run_subparser(subparsers)

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

        self._add_cpp_project_option(build_parser)

        self.args = parser.parse_args()

    def _build_run_subparser(
        self,
        subparsers: Any,
    ) -> None:
        run_parser = subparsers.add_parser(
            "run",
            help="Run a specified python project from the projects.json file.",
        )

        run_subparsers = run_parser.add_subparsers(
            dest="project",
            help="The relative path to the python project to run.",
        )

        for project in self.settings.runables:
            if self.settings.is_python_project(project):
                run_subparsers.add_parser(project)
            elif self.settings.is_cpp_project(project):
                cpp_parser = run_subparsers.add_parser(project)
                self._add_cpp_project_option(cpp_parser)
            else:
                raise ValueError(f"Unknown project type for project: {project}")

    def _add_cpp_project_option(
        self,
        parser: ArgumentParser,
    ) -> None:
        parser.add_argument(
            "-p",
            "--platform",
            type=str,
            default=CPP_BUILD_PLATFORMS[0],
            choices=CPP_BUILD_PLATFORMS,
            help="The target platform for the build (windows, linux, macos, web, android). Default is linux.",
        )

        parser.add_argument(
            "-t",
            "--type",
            type=str,
            default=CPP_BUILD_TYPES[0],
            choices=CPP_BUILD_TYPES,
            help="The type of build to perform (debug, release, test). Default is debug.",
        )

        parser.add_argument(
            "-r",
            "--reload",
            action="store_true",
            help="Regenerate the build files before building the project.",
        )

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
            elif self.settings.is_cpp_project(self.args.project):
                logger.debug(f"Building {self.args.project}...")
                build_cpp_project(**self.vars)
                logger.debug(
                    f"Running {self.args.project} with executable {self.settings.get_cpp_project_executable(self.args.project)}..."
                )
                run_cpp_project(
                    executable=self.settings.get_cpp_project_executable(
                        self.args.project
                    ),
                    **self.vars,
                )
            else:
                raise ValueError(
                    f"Unknown project type for project: {self.args.project}"
                )

        elif self.args.command == "build":
            if self.settings.is_cpp_project(self.args.project):
                logger.debug(f"Building {self.args.project}...")
                generate_cpp_project(**self.vars)
                build_cpp_project(**self.vars)
