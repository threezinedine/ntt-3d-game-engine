from typing import Any, Callable


class Signal:
    """
    Mechanism for updating multiple listeners when an event occurs.
    """

    def __init__(self) -> None:
        self._listeners: list[Callable[..., Any]] = []
        self._signals: list["Signal"] = []

    def connect(self, listener: Callable[..., Any]) -> None:
        """
        Connect a listener to the signal.

        :param listener: The listener to connect.
        """
        self._listeners.append(listener)

    def attach(self, signal: "Signal") -> None:
        """
        Attach another signal to this signal. When this signal is emitted,
        the attached signal will also be emitted.

        :param signal: The signal to attach.
        """
        self._signals.append(signal)

    def emit(
        self,
        *args: Any,
        passedSignals: set["Signal"] | None = None,
        **kwargs: Any,
    ) -> None:
        """
        Emit the signal, calling all connected listeners with the provided arguments.

        :param passedSignals: Set of signals that have already been emitted to avoid circular references.
        :param args: Positional arguments to pass to the listeners.
        :param kwargs: Keyword arguments to pass to the listeners.
        """
        if passedSignals is None:
            passedSignals = set()

        if self in passedSignals:
            return

        passedSignals.add(self)

        for signal in self._signals:
            signal.emit(passedSignals=passedSignals, *args, **kwargs)

        for listener in self._listeners:
            listener(*args, **kwargs)
