# Dependency injector utilities
from typing import TypeVar, Callable, Type, Dict, Any, cast

T = TypeVar("T")


_instances: Dict[Type[Any], Any] = {}
_instanceFactories: Dict[Type[Any], Callable[[], Any]] = {}
_transientFactories: Dict[Type[Any], Callable[[], Any]] = {}
_dependentClasses: Dict[Type[Any], list[Type[Any]]] = {}


def clean_instances() -> None:
    """
    Clean all singleton instances. Used for testing purposes.
    """
    _instances.clear()
    _instanceFactories.clear()
    _transientFactories.clear()
    _dependentClasses.clear()


def di_get(clsName: Type[T], *args: Any, **kwargs: Any) -> T:
    """
    Get the singleton instance of the given class.

    :param clsName: The class to get the singleton instance for.
    :return: The singleton instance of the class.
    """
    if clsName in _instances:
        return _instances[clsName]

    if clsName in _instanceFactories:
        instance = _instanceFactories[clsName](*args, **kwargs)
        _instances[clsName] = instance
        return cast(T, instance)

    if clsName in _transientFactories:
        instance = _transientFactories[clsName](*args, **kwargs)
        return cast(T, instance)

    raise ValueError(f"Singleton {clsName} is not registered.")


def as_singleton(cls: Type[T]) -> Type[T]:
    """
    Decorator to register a class as a singleton.
    """
    assert (
        cls not in _instances
    ), f"Singleton {cls} cannot be registered as it is already instantiated."
    assert (
        cls not in _transientFactories
    ), f"Singleton {cls} cannot be registered as transient."
    assert cls not in _instanceFactories, f"Singleton {cls} is already registered."

    def _factory(*args: Any, **kwargs: Any) -> T:
        deps: list[Any] = []
        for depCls in _dependentClasses.get(cls, []):
            deps.append(di_get(depCls))
        return cls(*deps, *args, **kwargs)

    _instanceFactories[cls] = _factory

    return cls


def as_transient(cls: Type[T]) -> Type[T]:
    """
    Decorator to register a class as transient.
    """
    assert (
        cls not in _instances
    ), f"Transient {cls} cannot be registered as it is already instantiated."
    assert (
        cls not in _instanceFactories
    ), f"Transient {cls} cannot be registered as singleton."
    assert cls not in _transientFactories, f"Transient {cls} is already registered."

    def _factory(*args: Any, **kwargs: Any) -> T:
        deps: list[Any] = []
        for depCls in _dependentClasses.get(cls, []):
            assert (
                depCls not in _instances and depCls not in _instanceFactories
            ), f"Transient {cls} cannot depend on singleton {depCls}."
            deps.append(di_get(depCls))
        return cls(*deps, *args, **kwargs)

    _transientFactories[cls] = _factory

    return cls


def as_dependencies(*depClasses: Type[Any]) -> Callable[[Type[T]], Type[T]]:
    """
    Decorator to register dependencies for a class.
    """

    def _decorator(cls: Type[T]) -> Type[T]:
        _dependentClasses[cls] = list(set(depClasses))  # type: ignore
        return cls

    return _decorator
