from parser import *


def test_parse_typedef():
    parser = Parser()

    parser.add_code(
        "typedef.cpp",
        """
        typedef unsigned int uint;
        typedef float real_t;
        """,
    )

    parser.parse("typedef.cpp")

    assert len(parser.Typedefs) == 2
    typedef1 = parser.Typedefs[0]
    assert typedef1.name == "uint"
    assert typedef1.underlying_type == "unsigned int"

    typedef2 = parser.Typedefs[1]
    assert typedef2.name == "real_t"
    assert typedef2.underlying_type == "float"


def test_parse_typedef_in_namespace():
    parser = Parser()

    parser.add_code(
        "typedef_namespace.cpp",
        """
        namespace math {
            typedef double real64;
        }
        """,
    )

    parser.parse("typedef_namespace.cpp")

    assert len(parser.Typedefs) == 1
    typedef = parser.Typedefs[0]
    assert typedef.name == "real64"
    assert typedef.underlying_type == "double"
    assert typedef.namespace == "math"


def test_parse_typedef_pointer():
    parser = Parser()

    parser.add_code(
        "typedef_pointer.cpp",
        """
        typedef int* int_ptr;
        """,
    )

    parser.parse("typedef_pointer.cpp")

    assert len(parser.Typedefs) == 1
    typedef = parser.Typedefs[0]
    assert typedef.name == "int_ptr"
    assert typedef.underlying_type == "int *"


def test_parse_typedef_const():
    parser = Parser()

    parser.add_code(
        "typedef_const.cpp",
        """
        typedef const char* cstring;
        """,
    )

    parser.parse("typedef_const.cpp")

    assert len(parser.Typedefs) == 1
    typedef = parser.Typedefs[0]
    assert typedef.name == "cstring"
    assert typedef.underlying_type == "const char *"


def test_parse_typedef_function_pointer():
    parser = Parser()

    parser.add_code(
        "typedef_function_pointer.cpp",
        """
        typedef void (*Callback)(int);
        """,
    )

    parser.parse("typedef_function_pointer.cpp")

    assert len(parser.Typedefs) == 1
    typedef = parser.Typedefs[0]
    assert typedef.name == "Callback"
    assert typedef.underlying_type == "void (*)(int)"


def test_parse_typedef_annotations():
    parser = Parser()

    parser.add_code(
        "typedef_with_annotations.cpp",
        """
        typedef int __attribute__((annotate("py:special_int"))) special_int_t;
        """,
    )

    parser.parse("typedef_with_annotations.cpp")

    assert len(parser.Typedefs) == 1
    typedef = parser.Typedefs[0]
    assert typedef.name == "special_int_t"
    assert typedef.underlying_type == "int"
    assert len(typedef.annotations) == 1
    assert "py:special_int" in typedef.annotations


def test_parse_using():
    parser = Parser()

    parser.add_code(
        "using_alias.cpp",
        """
        using uint = unsigned int;
        using real_t = float;
        """,
    )

    parser.parse("using_alias.cpp")

    assert len(parser.Typedefs) == 2
    typedef1 = parser.Typedefs[0]
    assert typedef1.name == "uint"
    assert typedef1.underlying_type == "unsigned int"

    typedef2 = parser.Typedefs[1]
    assert typedef2.name == "real_t"
    assert typedef2.underlying_type == "float"


def test_parse_using_in_namespace():
    parser = Parser()

    parser.add_code(
        "using_namespace.cpp",
        """
        namespace math {
            using real64 = double;
        }
        """,
    )

    parser.parse("using_namespace.cpp")

    assert len(parser.Typedefs) == 1
    typedef = parser.Typedefs[0]
    assert typedef.name == "real64"
    assert typedef.underlying_type == "double"
    assert typedef.namespace == "math"


def test_parse_using_function_pointer():
    parser = Parser()

    parser.add_code(
        "using_function_pointer.cpp",
        """
        #include <functional>

        using Callback = std::function<void(int)>;
        """,
    )

    parser.parse("using_function_pointer.cpp")

    assert len(parser.Typedefs) != 0
    typedef = parser.Typedefs[-1]
    assert typedef.name == "Callback"
    assert typedef.underlying_type == "std::function<void (int)>"
