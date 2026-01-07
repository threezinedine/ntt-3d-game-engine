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

        self.args = parser.parse_args()

    @property
    def verbose(self) -> bool:
        return self.args.verbose

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
