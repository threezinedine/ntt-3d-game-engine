from argparse import ArgumentParser
from utils import *
import logging
from log import autogen_logger
from autogen import Autogen


def main():
    parser = ArgumentParser(description="Extract c project and create bindings.")
    parser.add_argument(
        "-v",
        "--verbose",
        action="store_true",
    )

    args = parser.parse_args()

    if args.verbose:
        autogen_logger.setLevel(logging.DEBUG)
        autogen_logger.debug("Verbose mode enabled.")
    else:
        autogen_logger.setLevel(logging.INFO)

    autogen = Autogen()
    autogen.Execute()


if __name__ == "__main__":
    main()
