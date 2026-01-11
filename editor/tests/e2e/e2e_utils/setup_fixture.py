import os
import pytest
from typing import Generator
from Engine import *
from pyfakefs.fake_filesystem import FakeFilesystem
from constants import *
from di import *


class EngineSetup:
    def __init__(self):
        pass


@pytest.fixture
def setup_engine(fs: FakeFilesystem) -> Generator[EngineSetup, None, None]:
    """
    Fixture to set up and tear down the engine for end-to-end tests.
    """
    fs.add_real_directory(os.path.join(BASE_DIR, ".venv"))  # type: ignore
    # Setup code before the test
    engine_setup = EngineSetup()
    yield engine_setup
    Logger.Release()
    clean_instances()
    # Teardown code after the test
