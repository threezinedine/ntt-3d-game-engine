from parser import *


def test_parse_simple_enum():
    parser = Parser()

    parser.add_code(
        "enum.cpp",
        """
        enum Color {
            RED,
            GREEN,
            BLUE,
            CYAN = 5,
        };
        """,
    )

    parser.parse("enum.cpp")

    assert len(parser.Enums) == 1
    enum = parser.Enums[0]
    assert enum.name == "Color"

    assert len(enum.constants) == 4
    assert enum.constants[0].name == "RED"
    assert enum.constants[0].value == 0
    assert enum.constants[1].name == "GREEN"
    assert enum.constants[1].value == 1
    assert enum.constants[2].name == "BLUE"
    assert enum.constants[2].value == 2
    assert enum.constants[3].name == "CYAN"
    assert enum.constants[3].value == 5


def test_parse_enum_with_annotations():
    parser = Parser()

    parser.add_code(
        "enum_with_annotations.cpp",
        """
        enum __attribute__((annotate("py:active_status"))) Status {
            ACTIVE,
            INACTIVE __attribute__((annotate("hidden"))),
            PENDING
        };
        """,
    )

    parser.parse("enum_with_annotations.cpp")

    assert len(parser.Enums) == 1
    enum = parser.Enums[0]
    assert enum.name == "Status"
    assert len(enum.annotations) == 1
    assert "py:active_status" in enum.annotations

    assert len(enum.constants) == 3
    assert enum.constants[0].name == "ACTIVE"
    assert enum.constants[0].value == 0
    assert enum.constants[0].annotations == []

    assert enum.constants[1].name == "INACTIVE"
    assert enum.constants[1].value == 1
    assert "hidden" in enum.constants[1].annotations

    assert enum.constants[2].name == "PENDING"
    assert enum.constants[2].value == 2
    assert enum.constants[2].annotations == []


def test_parse_enum_with_comment():
    parser = Parser()

    parser.add_code(
        "enum_with_comment.cpp",
        """
        /// This enum represents different levels of logging
        enum LogLevel {
            DEBUG,   ///< Detailed information for debugging
            INFO,    ///< General information about application operation
            WARNING, ///< Indication of potential issues
            ERROR    ///< Error events that might still allow the application to continue running
        };
        """,
    )

    parser.parse("enum_with_comment.cpp")

    assert len(parser.Enums) == 1
    enum = parser.Enums[0]
    assert enum.name == "LogLevel"
    assert enum.comment == "/// This enum represents different levels of logging"

    assert len(enum.constants) == 4
    assert enum.constants[0].name == "DEBUG"
    assert enum.constants[0].value == 0
    assert enum.constants[0].comment == "///< Detailed information for debugging"

    assert enum.constants[1].name == "INFO"
    assert enum.constants[1].value == 1
    assert (
        enum.constants[1].comment
        == "///< General information about application operation"
    )


def test_parse_namespaced_enum():
    parser = Parser()

    parser.add_code(
        "namespaced_enum.cpp",
        """
        namespace Graphics {
            enum Shape {
                CIRCLE,
                SQUARE,
                TRIANGLE
            };
        }
        """,
    )

    parser.parse("namespaced_enum.cpp")
    assert len(parser.Enums) == 1
    enum = parser.Enums[0]
    assert enum.name == "Shape"
    assert enum.namespace == "Graphics"
