from argparse import ArgumentParser


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

        self.args = parser.parse_args()

    @property
    def verbose(self) -> bool:
        return self.args.verbose
