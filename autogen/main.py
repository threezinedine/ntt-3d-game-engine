from argparse import ArgumentParser
from utils import *


def main():
    parser = ArgumentParser(description="Extract c project and create bindings.")
    parser.add_argument(
        "-v",
        "--verbose",
        action="store_true",
    )

    args = parser.parse_args()

    if args.verbose:
        logger.setLevel(logging.DEBUG)
        logger.debug("Verbose mode enabled.")
    else:
        logger.setLevel(logging.INFO)

    logger.info("This is a placeholder for the main functionality.")


if __name__ == "__main__":
    main()
