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

        self._build_install_subparser(subparsers)
        self._build_clean_subparser(subparsers)

        self.args = parser.parse_args()

    def _build_clean_subparser(
        self,
        subparsers: Any,
    ) -> None:
        clean_parser = subparsers.add_parser(
            "clean",
            help="Clean build files for all projects specified in the projects.json file.",
        )

        clean_subparsers = clean_parser.add_subparsers(
            dest="project",
            help="The relative path to the project to clean, or 'all' to clean all projects.",
        )

        clean_subparsers.add_parser(
            "all",
            help="Clean all projects specified in the projects.json file.",
        )

        for project in self.settings.cleanable:
            if self.settings.is_python_project(project):
                clean_subparsers.add_parser(project)
            elif self.settings.is_cpp_project(project):
                cpp_parser = clean_subparsers.add_parser(project)
                self._add_cpp_project_option(cpp_parser)

    def _build_install_subparser(
        self,
        subparsers: Any,
    ) -> None:
        install_parser = subparsers.add_parser(
            "install",
            help="Install dependencies for a specified python project from the projects.json file.",
        )

        install_subparsers = install_parser.add_subparsers(
            dest="project",
            help="The relative path to the python project to install dependencies for.",
        )

        for project in self.settings.installables:
            install_subparsers.add_parser(project)

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
                python_project = self.settings.get_python_project(self.args.project)
                command_logger.debug(f"Syncing {self.args.project}...")
                sync_python_project(self.args.project)
                command_logger.debug(f"Running {self.args.project}...")
                if python_project.preRun is not None:
                    command_logger.debug(
                        f"Running pre-run command for {self.args.project}..."
                    )
                    run_command(
                        python_project.preRun.command,
                        directory=python_project.preRun.cwd,
                    )
                run_python_project(self.args.project, verbose=self.args.verbose)
            elif self.settings.is_cpp_project(self.args.project):
                command_logger.debug(f"Building {self.args.project}...")
                build_cpp_project(**self.vars)
                project = self.settings.get_cpp_project(self.args.project)
                command_logger.debug(
                    f"Running {self.args.project} with executable {project.executable}..."
                )
                run_cpp_project(
                    executable=project.executable,
                    **self.vars,
                )
            else:
                raise ValueError(
                    f"Unknown project type for project: {self.args.project}"
                )

        elif self.args.command == "build":
            if self.settings.is_cpp_project(self.args.project):
                command_logger.debug(f"Building {self.args.project}...")
                generate_cpp_project(**self.vars)
                build_cpp_project(**self.vars)
            else:
                raise ValueError(f"Project {self.args.project} is not a C++ project.")
        elif self.args.command == "install":
            if self.settings.is_python_project(self.args.project):
                command_logger.debug(
                    f"Installing dependencies for {self.args.project}..."
                )
                sync_python_project(self.args.project)
                project = self.settings.get_python_project(self.args.project)
                assert (
                    project.install is not None
                ), "Install command should not be None."

                if project.preInstall is not None:
                    command_logger.debug(
                        f"Running pre-install command for {self.args.project}..."
                    )
                    run_command(
                        project.preInstall.command,
                        directory=project.preInstall.cwd,
                    )

                run_command(
                    project.install.command,
                    directory=project.install.cwd,
                )
            else:
                raise ValueError(
                    f"Project {self.args.project} is not a Python project."
                )
        elif self.args.command == "clean":
            if self.args.project == "all":
                command_logger.debug("Cleaning all projects...")
                for project in self.settings.Projects.cppProjects:
                    clean_cpp_project(project=project.relPath)

                    for clean_item in project.cleans or []:
                        self._clean(clean_item)

                for project in self.settings.Projects.pythonProjects:
                    clean_python_project(project=project.relPath)

                    for clean_item in project.cleans or []:
                        self._clean(clean_item)

            elif self.settings.is_cpp_project(self.args.project):
                command_logger.debug(f"Cleaning C++ project {self.args.project}...")
                clean_cpp_project(**self.vars)

                project = self.settings.get_cpp_project(self.args.project)
                for clean_item in project.cleans or []:
                    self._clean(clean_item)

            elif self.settings.is_python_project(self.args.project):
                command_logger.debug(f"Cleaning Python project {self.args.project}...")
                clean_python_project(self.args.project)

                project = self.settings.get_python_project(self.args.project)
                for clean_item in project.cleans or []:
                    self._clean(clean_item)
            else:
                raise ValueError(
                    f"Unknown project type for project: {self.args.project}"
                )
        else:
            raise ValueError(f"Unknown command: {self.args.command}")

    def _clean(self, name: str) -> None:
        path = os.path.join(name)
        if not os.path.exists(path):
            return

        if os.path.isfile(path):
            os.remove(path)
        else:
            shutil.rmtree(path, ignore_errors=True)
