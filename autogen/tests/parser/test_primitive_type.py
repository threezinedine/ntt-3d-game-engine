from parser import *


def test_is_primitive_type():
    parser = Parser()

    parser.add_code(
        "primitive_type.h",
        """
    typedef int u32;
""",
    )

    parser.parse("primitive_type.h")

    assert "u32" in parser.PrimitiveTypes


def test_struct_not_in_primitive_types():
    parser = Parser()

    parser.add_code(
        "struct_type.h",
        """
    struct MyStruct {
        int a;
        float b;
    };
""",
    )

    parser.parse("struct_type.h")

    assert "MyStruct" not in parser.PrimitiveTypes


# def test_array_is_not_primitive_type():
#     parser = Parser()

#     parser.add_code(
#         "array_type.h",
#         """
#         #include <vector>

#         template <typename T>
#         using Array = std::vector<T>;
# """,
#     )

#     parser.parse("array_type.h")

#     assert "" not in parser.PrimitiveTypes
