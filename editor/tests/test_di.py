import pytest
from di import *
from typing import Generator


@pytest.fixture
def clean_di() -> Generator[None, None, None]:
    """
    Fixture to clean DI instances before and after each test.
    """
    clean_instances()
    yield
    clean_instances()


def test_read_singleton(clean_di: None) -> None:
    @as_singleton
    class MySingleton:
        def __init__(self, value: int) -> None:
            self.value = value

    instance1 = di_get(MySingleton, 34)
    instance2 = di_get(MySingleton, 56)

    assert instance1 is instance2
    assert instance1.value == 34


def test_read_transient(clean_di: None) -> None:
    @as_transient
    class MyTransient:
        def __init__(self, value: int) -> None:
            self.value = value

    instance1 = di_get(MyTransient, 34)
    instance2 = di_get(MyTransient, 56)

    assert instance1 is not instance2
    assert instance1.value == 34
    assert instance2.value == 56


def test_dependent_singleton(clean_di: None) -> None:
    @as_singleton
    class Dependency:
        def __init__(self, value: int) -> None:
            self.value = value

    @as_singleton
    @as_dependencies(Dependency)
    class Dependent:
        def __init__(self, dependency: Dependency) -> None:
            self.dependency = dependency

    di_get(Dependency, 42)

    instance = di_get(Dependent)

    assert instance.dependency.value == 42
