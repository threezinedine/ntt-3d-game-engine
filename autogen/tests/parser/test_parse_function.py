from parser import *


def test_parse_function():
    parser = Parser()

    parser.add_code(
        "function.cpp",
        """
        int add(int a, int b) {
            return a + b;
        }

        void print_message(const char* msg);
        """,
    )

    parser.parse("function.cpp")

    assert len(parser.Functions) == 2

    func1 = parser.Functions[0]
    assert func1.name == "add"
    assert func1.return_type == "int"

    assert len(func1.arguments) == 2
    assert func1.arguments[0].name == "a"
    assert func1.arguments[0].type == "int"
    assert func1.arguments[1].name == "b"
    assert func1.arguments[1].type == "int"


def test_parse_in_namespace():
    parser = Parser()

    parser.add_code(
        "function_namespace.cpp",
        """
        namespace math {
            double multiply(double x, double y) {
                return x * y;
            }
        }
        """,
    )

    parser.parse("function_namespace.cpp")

    assert len(parser.Functions) == 1
    func = parser.Functions[0]
    assert func.name == "multiply"
    assert func.return_type == "double"
    assert func.namespace == "math"

    assert len(func.arguments) == 2
    assert func.arguments[0].name == "x"
    assert func.arguments[0].type == "double"
    assert func.arguments[1].name == "y"
    assert func.arguments[1].type == "double"


def test_parse_void_function():
    parser = Parser()

    parser.add_code(
        "void_function.cpp",
        """
        void log_event(const char* event) {
            // Log the event
        }
        """,
    )

    parser.parse("void_function.cpp")

    assert len(parser.Functions) == 1
    func = parser.Functions[0]
    assert func.name == "log_event"
    assert func.return_type == "void"

    assert len(func.arguments) == 1
    assert func.arguments[0].name == "event"
    assert func.arguments[0].type == "const char *"


def test_parse_function_no_arguments():
    parser = Parser()

    parser.add_code(
        "no_arg_function.cpp",
        """
        bool is_initialized() {
            return true;
        }
        """,
    )

    parser.parse("no_arg_function.cpp")

    assert len(parser.Functions) == 1
    func = parser.Functions[0]
    assert func.name == "is_initialized"
    assert func.return_type == "bool"

    assert len(func.arguments) == 0


def test_parse_function_pointer():
    parser = Parser()

    parser.add_code(
        "function_pointer.cpp",
        """
        typedef void (*Callback)(int);

        void register_callback(Callback cb) {
            // Register the callback
        }
        """,
    )

    parser.parse("function_pointer.cpp")

    assert len(parser.Functions) == 1
    func = parser.Functions[0]
    assert func.name == "register_callback"
    assert func.return_type == "void"

    assert len(func.arguments) == 1
    assert func.arguments[0].name == "cb"
    assert func.arguments[0].type == "Callback"


def test_parse_function_with_default_arguments():
    parser = Parser()

    parser.add_code(
        "function_default_args.cpp",
        """
        int compute_area(int width, int height = 10, float scale = 1.0, bool round_up = false, 
                char unit = 'm', const char* label = "area") {
            return width * height;
        }
        """,
    )

    parser.parse("function_default_args.cpp")

    assert len(parser.Functions) == 1
    func = parser.Functions[0]
    assert func.name == "compute_area"
    assert func.return_type == "int"

    assert len(func.arguments) == 6
    assert func.arguments[0].name == "width"
    assert func.arguments[0].type == "int"
    assert func.arguments[1].name == "height"
    assert func.arguments[1].type == "int"
    assert func.arguments[1].default_value == 10
    assert func.arguments[2].name == "scale"
    assert func.arguments[2].type == "float"
    assert func.arguments[2].default_value == 1.0
    assert func.arguments[3].name == "round_up"
    assert func.arguments[3].type == "bool"
    assert func.arguments[3].default_value == False
    assert func.arguments[4].name == "unit"
    assert func.arguments[4].type == "char"
    assert func.arguments[4].default_value == "'m'"
    assert func.arguments[5].name == "label"
    assert func.arguments[5].type == "const char *"
    assert func.arguments[5].default_value == '"area"'


def test_function_with_annotations():
    parser = Parser()

    parser.add_code(
        "function_annotations.cpp",
        """
        int __attribute__((annotate("nodiscard"))) calculate_sum(int a __attribute__((annotate("nonnull"))), int b) {
            return a + b;
        }
        """,
    )

    parser.parse("function_annotations.cpp")

    assert len(parser.Functions) == 1
    func = parser.Functions[0]
    assert func.name == "calculate_sum"
    assert func.return_type == "int"
    assert "nodiscard" in func.annotations

    assert len(func.arguments) == 2
    assert func.arguments[0].name == "a"
    assert func.arguments[0].type == "int"
    assert "nonnull" in func.arguments[0].annotations

    assert func.arguments[1].name == "b"
    assert func.arguments[1].type == "int"


def test_parse_function_with_comments():
    parser = Parser()

    parser.add_code(
        "function_with_comments.cpp",
        """
        /// This function adds two integers
        int add(int a, int b) {
            return a + b; // Return the sum
        }
        """,
    )

    parser.parse("function_with_comments.cpp")

    assert len(parser.Functions) == 1
    func = parser.Functions[0]
    assert func.name == "add"
    assert func.return_type == "int"
    assert func.comment is not None
    assert "This function adds two integers" in func.comment
