import sys
import subprocess
from configparser import ConfigParser


def main():
    config = ConfigParser()
    config.read("project.cfg")

    main_file = config.get("config", "file", fallback="helper.py")

    subprocess.run(
        " ".join(["uv run", main_file] + sys.argv[1:]),
        check=True,
        shell=True,
    )


if __name__ == "__main__":
    try:
        main()
    except subprocess.CalledProcessError as e:
        sys.exit(e.returncode)
