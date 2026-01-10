from parser import *


def test_parse_union():
    parser = Parser()

    parser.add_code(
        "union.cpp",
        """
        union Data {
            int intValue;
            float floatValue;
            char strValue[20];
        };
        """,
    )

    parser.parse("union.cpp")

    assert len(parser.Unions) == 1
    union = parser.Unions[0]
    assert union.name == "Data"

    assert len(union.fields) == 3
    assert union.fields[0].name == "intValue"
    assert union.fields[0].type == "int"
    assert union.fields[1].name == "floatValue"
    assert union.fields[1].type == "float"
    assert union.fields[2].name == "strValue"
    assert union.fields[2].type == "char[20]"


def test_parse_union_in_namespace():
    parser = Parser()

    parser.add_code(
        "union_namespace.cpp",
        """
        namespace storage {
            union Pixel {
                struct {
                    unsigned char r, g, b, a;
                } work;
                unsigned int value;
            };
        }
        """,
    )

    parser.parse("union_namespace.cpp")

    assert len(parser.Unions) == 1
    union = parser.Unions[0]
    assert union.name == "Pixel"
    assert union.namespace == "storage"

    assert len(union.fields) == 2

    assert union.fields[0].name == "work"
    assert union.fields[1].name == "value"
    assert union.fields[1].type == "unsigned int"


def test_parse_union_with_annotations():
    parser = Parser()

    parser.add_code(
        "union_with_annotations.cpp",
        """
        union __attribute__((annotate("py:data_union"))) Data {
            int intValue __attribute__((annotate("py:int_field")));
            float floatValue;
        };
        """,
    )

    parser.parse("union_with_annotations.cpp")

    assert len(parser.Unions) == 1
    union = parser.Unions[0]
    assert union.name == "Data"

    assert len(union.annotations) == 1
    assert "py" in union.annotations
    assert union.annotations["py"] == "data_union"

    assert len(union.fields) == 2
    assert union.fields[0].name == "intValue"
    assert union.fields[0].type == "int"
    assert len(union.fields[0].annotations) == 1
    assert "py" in union.fields[0].annotations
    assert union.fields[0].annotations["py"] == "int_field"

    assert union.fields[1].name == "floatValue"
    assert union.fields[1].type == "float"
    assert len(union.fields[1].annotations) == 0


def test_parse_union_with_comment():
    parser = Parser()

    parser.add_code(
        "union_with_comment.cpp",
        """
        /// This union represents a simple data container
        union Data {
            /// Integer value
            int intValue;
            /// Floating point value
            float floatValue;
        };
        """,
    )

    parser.parse("union_with_comment.cpp")

    assert len(parser.Unions) == 1
    union = parser.Unions[0]
    assert union.name == "Data"
    assert union.comment is not None
    assert "simple data container" in union.comment

    assert len(union.fields) == 2
    assert union.fields[0].name == "intValue"
    assert union.fields[0].type == "int"
    assert union.fields[0].comment is not None
    assert "Integer value" in union.fields[0].comment

    assert union.fields[1].name == "floatValue"
    assert union.fields[1].type == "float"
    assert union.fields[1].comment is not None
    assert "Floating point value" in union.fields[1].comment
