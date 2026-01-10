from parser import *


def test_multiple_annotations():
    parser = Parser()

    parser.add_code(
        "annotations.cpp",
        """
        #define ALIAS(x) __attribute__((annotate("alias:" #x)))
        #define DEFAULT(x) __attribute__((annotate("default:" #x)))

        struct Result
        {
            int value ALIAS(result_value) DEFAULT(42);
        };
        """,
    )

    parser.parse("annotations.cpp")

    assert len(parser.Structs) == 1
    struct = parser.Structs[0]
    assert struct.name == "Result"
    assert len(struct.fields) == 1
    field = struct.fields[0]
    assert field.name == "value"
    assert field.type == "int"
    assert len(field.annotations) == 2
    assert "alias" in field.annotations
    assert field.annotations["alias"] == "result_value"
    assert "default" in field.annotations
    assert field.annotations["default"] == "42"
