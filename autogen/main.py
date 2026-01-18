import os
from argparse import ArgumentParser
from utils import *
import logging
from log import autogen_logger
from autogen import Autogen
from constants import *
from clang.cindex import Config


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

    if os.name == "nt":
        libclang_path = os.environ.get("NTT_LIBCLANG_PATH", "")
        assert libclang_path, "Please set NTT_LIBCLANG_PATH environment variable."
        autogen_logger.info("Setting libclang path for Windows.")
        Config.set_library_path(libclang_path)

    autogen = Autogen()
    autogen.Execute()


if __name__ == "__main__":
    main()
