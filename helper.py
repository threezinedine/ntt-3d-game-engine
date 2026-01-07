from config import *


def main():
    args = Args()

    if args.verbose:
        logger.setLevel(logging.DEBUG)
        logger.debug("Verbose mode enabled.")
    else:
        logger.setLevel(logging.INFO)

    logger.debug("This is a debug message.")
    logger.info("Test")


if __name__ == "__main__":
    main()
