import os
import subprocess


def main():
    subprocess.run(
        "uv run pyinstaller --onefile --noconsole main.py",
        check=True,
        shell=True,
    )

    BASE_DIR = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))

    exe_file = "main.exe" if os.name == "nt" else "main"

    copy_command = "cp" if os.name != "nt" else "copy"
    src = os.path.join(BASE_DIR, "runtool", "dist", exe_file)
    dst = os.path.join(BASE_DIR, exe_file)

    print("Running copy command:", f"{copy_command} {src} {dst}")
    subprocess.run(
        f"{copy_command} {src} {dst}",
        check=True,
        shell=True,
    )


if __name__ == "__main__":
    main()
