import subprocess


def main():
    subprocess.run("uv run pyinstaller main.py", shell=True, check=True)


if __name__ == "__main__":
    main()
