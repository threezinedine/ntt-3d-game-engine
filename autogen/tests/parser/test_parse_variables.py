from parser import *


def test_parse_variables():
    parser = Parser()

    parser.add_code(
        "variables.cpp",
        """
        int global_var = 42;
        static float static_var = 3.14f;
        const char* const_var = "Hello, World!";
        """,
    )

    parser.parse("variables.cpp")

    assert len(parser.Variables) == 3

    var1 = parser.Variables[0]
    assert var1.name == "global_var"
    assert var1.type == "int"

    var2 = parser.Variables[1]
    assert var2.name == "static_var"
    assert var2.type == "float"

    var3 = parser.Variables[2]
    assert var3.name == "const_var"
    assert var3.type == "const char *"


def test_parse_variable_in_namespace():
    parser = Parser()

    parser.add_code(
        "variable_namespace.cpp",
        """
        namespace config {
            double threshold = 0.75;
        }
        """,
    )

    parser.parse("variable_namespace.cpp")

    assert len(parser.Variables) == 1

    var = parser.Variables[0]
    assert var.name == "threshold"
    assert var.type == "double"
    assert var.namespace == "config"


def test_parse_variable_without_initializer():
    parser = Parser()

    parser.add_code(
        "variable_no_initializer.cpp",
        """
        extern int external_var;
        """,
    )

    parser.parse("variable_no_initializer.cpp")

    assert len(parser.Variables) == 1

    var = parser.Variables[0]
    assert var.name == "external_var"
    assert var.type == "int"


def test_parse_variable_with_comment():
    parser = Parser()

    parser.add_code(
        "variable_with_comment.cpp",
        """
        /// This variable stores the maximum number of connections
        int max_connections = 100;
        """,
    )

    parser.parse("variable_with_comment.cpp")

    assert len(parser.Variables) == 1

    var = parser.Variables[0]
    assert var.name == "max_connections"
    assert var.type == "int"
    assert var.comment is not None
    assert "maximum number of connections" in var.comment


def test_parse_variable_with_annotations():
    parser = Parser()

    parser.add_code(
        "variable_with_annotations.cpp",
        """
        int __attribute__((annotate("py:configurable"))) __attribute__((annotate("hidden"))) config_value = 10;
        """,
    )

    parser.parse("variable_with_annotations.cpp")

    assert len(parser.Variables) == 1

    var = parser.Variables[0]
    assert var.name == "config_value"
    assert var.type == "int"
    assert len(var.annotations) == 2
    assert "py:configurable" in var.annotations
    assert "hidden" in var.annotations
