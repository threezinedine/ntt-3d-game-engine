import os
import sysconfig

CPP_BUILD_TYPES = ["debug", "release", "test"]
CPP_BUILD_PLATFORMS = ["linux", "windows", "macos", "web", "android"]
CPP_GENERATORS = ["unix", "mingw", "msvc"]

BASE_DIR = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))

if os.name == "nt":
    PLATFORM_IS_WINDOWS = True
    PLATFORM_IS_LINUX = False
elif os.name == "posix":
    PLATFORM_IS_WINDOWS = False
    PLATFORM_IS_LINUX = True
else:
    PLATFORM_IS_WINDOWS = False
    PLATFORM_IS_LINUX = False

PYTHON_INCLUDE_DIR = sysconfig.get_paths()["include"]

DELETE_FILE_COMMAND = "del" if PLATFORM_IS_WINDOWS else "rm -f"
