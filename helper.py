from config import *


def main():
    args = Args()

    if args.verbose:
        logger.setLevel(logging.DEBUG)
        logger.debug("Verbose mode enabled.")
    else:
        logger.setLevel(logging.INFO)

    check_system_command("gcc", required=True)
    check_system_command("git", required=True)
    check_system_command("cmake", required=True)
    check_system_command("uv", required=True)

    args.execute()


if __name__ == "__main__":
    try:
        main()
    except Exception as e:
        logger.error(f"An unhandled exception occurred: {e}")
        raise e
