from utils import *
from unittest.mock import MagicMock


def test_signal_normal_flow():
    signal = Signal()

    mock_listener1 = MagicMock()
    mock_listener2 = MagicMock()

    signal.connect(mock_listener1)
    signal.connect(mock_listener2)

    signal.emit(42, "hello")

    mock_listener1.assert_called_once_with(42, "hello")
    mock_listener2.assert_called_once_with(42, "hello")


def test_signal_connect_with_differnt_signal():
    signal1 = Signal()
    signal2 = Signal()

    mock_listener1 = MagicMock()
    mock_listener2 = MagicMock()

    signal1.connect(mock_listener1)
    signal2.connect(mock_listener2)

    signal1.attach(signal2)

    signal1.emit(100)

    mock_listener1.assert_called_once_with(100)
    mock_listener2.assert_called_once_with(100)

    signal2.emit(200)
    mock_listener1.assert_called_once_with(100)
    mock_listener2.assert_called_with(200)


def test_signal_emit_with_circular_reference():
    signal1 = Signal()
    signal2 = Signal()
    signal3 = Signal()

    mock_listener1 = MagicMock()
    mock_listener2 = MagicMock()
    mock_listener3 = MagicMock()

    signal1.connect(mock_listener1)
    signal2.connect(mock_listener2)
    signal3.connect(mock_listener3)

    signal1.attach(signal2)
    signal2.attach(signal3)
    signal3.attach(signal1)  # Circular reference

    signal1.emit("test")

    mock_listener1.assert_called_once_with("test")
    mock_listener2.assert_called_once_with("test")
    mock_listener3.assert_called_once_with("test")


def test_signal_attach_with_autorun_limit():
    signal = Signal()

    mock_listener = MagicMock()

    signal.connect(mock_listener, True)

    mock_listener.assert_called_once()
