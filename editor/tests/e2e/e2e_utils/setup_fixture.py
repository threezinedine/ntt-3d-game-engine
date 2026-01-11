import pytest
from typing import Generator
from Engine import *


class EngineSetup:
    def __init__(self):
        pass


@pytest.fixture
def setup_engine() -> Generator[EngineSetup, None, None]:
    """
    Fixture to set up and tear down the engine for end-to-end tests.
    """
    # Setup code before the test
    engine_setup = EngineSetup()
    yield engine_setup
    Logger.Release()
    # Teardown code after the test
