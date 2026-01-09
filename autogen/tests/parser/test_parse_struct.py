from parser import Parser


def test_parse_simple_struct():
    parser = Parser()

    parser.add_code(
        "struct.cpp",
        """
        struct Point {
            int x;
            int y;
        };
        """,
    )

    parser.parse("struct.cpp")
    assert len(parser.Structs) == 1

    struct = parser.Structs[0]
    assert struct.name == "Point"
    assert len(struct.fields) == 2

    field1 = struct.fields[0]
    assert field1.name == "x"
    assert field1.type == "int"

    field2 = struct.fields[1]
    assert field2.name == "y"
    assert field2.type == "int"


def test_parse_struct_in_namespace():
    parser = Parser()

    parser.add_code(
        "struct_namespace.cpp",
        """
        namespace geometry {
            struct Rectangle {
                float width;
                float height;
            };
        }
        """,
    )

    parser.parse("struct_namespace.cpp")
    assert len(parser.Structs) == 1

    struct = parser.Structs[0]
    assert struct.name == "Rectangle"
    assert struct.namespace == "geometry"
    assert len(struct.fields) == 2

    field1 = struct.fields[0]
    assert field1.name == "width"
    assert field1.type == "float"

    field2 = struct.fields[1]
    assert field2.name == "height"
    assert field2.type == "float"


def test_parse_struct_with_annotations():
    parser = Parser()

    parser.add_code(
        "struct_with_annotations.cpp",
        """
        struct __attribute__((annotate("py:vector3"))) Vector3 {
            float x;
            float y __attribute__((annotate("py:important")));
            float z;
        };
        """,
    )

    parser.parse("struct_with_annotations.cpp")
    assert len(parser.Structs) == 1
    struct = parser.Structs[0]
    assert struct.name == "Vector3"
    assert len(struct.annotations) == 1
    assert "py:vector3" in struct.annotations

    assert len(struct.fields) == 3
    field1 = struct.fields[0]
    assert field1.name == "x"
    assert field1.type == "float"
    assert len(field1.annotations) == 0

    field2 = struct.fields[1]
    assert field2.name == "y"
    assert field2.type == "float"
    assert len(field2.annotations) == 1
    assert "py:important" in field2.annotations

    field3 = struct.fields[2]
    assert field3.name == "z"
    assert field3.type == "float"
    assert len(field3.annotations) == 0


def test_parse_struct_with_comment():
    parser = Parser()

    parser.add_code(
        "struct_with_comment.cpp",
        """
        /// Represents a 2D point in space
        struct Point2D {
            /// The X coordinate
            int x;
            /// The Y coordinate
            int y;
        };
        """,
    )

    parser.parse("struct_with_comment.cpp")
    assert len(parser.Structs) == 1
    struct = parser.Structs[0]
    assert struct.name == "Point2D"
    assert struct.comment is not None
    assert "Represents a 2D point in space" in struct.comment

    assert len(struct.fields) == 2
    field1 = struct.fields[0]
    assert field1.name == "x"
    assert field1.type == "int"
    assert field1.comment is not None
    assert "The X coordinate" in field1.comment

    field2 = struct.fields[1]
    assert field2.name == "y"
    assert field2.type == "int"
    assert field2.comment is not None
    assert "The Y coordinate" in field2.comment


def test_parse_struct_with_accesss():
    parser = Parser()

    parser.add_code(
        "struct_with_access.cpp",
        """
        struct Person {
        public:
            const char* name;
        private:
            int age;
        protected:
            float height;
        };
        """,
    )

    parser.parse("struct_with_access.cpp")
    assert len(parser.Structs) == 1
    struct = parser.Structs[0]
    assert struct.name == "Person"
    assert len(struct.fields) == 3

    field1 = struct.fields[0]
    assert field1.name == "name"
    assert field1.type == "const char *"
    assert field1.access == "public"

    field2 = struct.fields[1]
    assert field2.name == "age"
    assert field2.type == "int"
    assert field2.access == "private"

    field3 = struct.fields[2]
    assert field3.name == "height"
    assert field3.type == "float"
    assert field3.access == "protected"


def test_parse_struct_with_methods():
    parser = Parser()

    parser.add_code(
        "struct_with_methods.cpp",
        """
        struct Calculator {
        public:
            int add(int a, int b);
        private:
            int subtract(int a, int b);
        protected:
            int multiply(int a, int b);
        };
        """,
    )

    parser.parse("struct_with_methods.cpp")
    assert len(parser.Structs) == 1
    struct = parser.Structs[0]
    assert struct.name == "Calculator"
    assert len(struct.methods) == 3

    method1 = struct.methods[0]
    assert method1.name == "add"
    assert method1.return_type == "int"
    assert method1.access == "public"

    method2 = struct.methods[1]
    assert method2.name == "subtract"
    assert method2.return_type == "int"
    assert method2.access == "private"

    method3 = struct.methods[2]
    assert method3.name == "multiply"
    assert method3.return_type == "int"
    assert method3.access == "protected"


def test_parse_struct_with_constructor():
    parser = Parser()

    parser.add_code(
        "struct_with_constructor.cpp",
        """
        struct Vector {
            float x;
            float y;
            float z;

            Vector(float x, float y, float z) : x(x), y(y), z(z) {}
            Vector();
            ~Vector();
        };
        """,
    )

    parser.parse("struct_with_constructor.cpp")
    assert len(parser.Structs) == 1
    struct = parser.Structs[0]
    assert struct.name == "Vector"

    assert len(struct.fields) == 3

    assert len(struct.constructors) == 2
    constructor = struct.constructors[0]
    assert constructor.name == "Vector"
    assert constructor.return_type == "void"
    assert len(constructor.arguments) == 3
    assert constructor.arguments[0].name == "x"
    assert constructor.arguments[0].type == "float"
    assert constructor.arguments[1].name == "y"
    assert constructor.arguments[1].type == "float"
    assert constructor.arguments[2].name == "z"
    assert constructor.arguments[2].type == "float"

    default_constructor = struct.constructors[1]
    assert default_constructor.name == "Vector"
    assert default_constructor.return_type == "void"
    assert len(default_constructor.arguments) == 0

    assert struct.destructor is not None
    destructor = struct.destructor
    assert destructor.name == "~Vector"
    assert destructor.return_type == "void"
    assert len(destructor.arguments) == 0


def test_parse_struct_with_inheritance():
    parser = Parser()

    parser.add_code(
        "struct_with_inheritance.cpp",
        """
        struct Base {
            int base_field;
        };

        namespace test
        {
            struct Base2 {
                float another_field;
            };
        }

        struct Derived : public Base, private test::Base2 {
            int derived_field;
        };
        """,
    )

    parser.parse("struct_with_inheritance.cpp")
    assert len(parser.Structs) == 3

    base_struct = parser.Structs[0]
    assert base_struct.name == "Base"
    assert len(base_struct.fields) == 1
    base_field = base_struct.fields[0]
    assert base_field.name == "base_field"

    base2_struct = parser.Structs[1]
    assert base2_struct.name == "Base2"
    assert len(base2_struct.fields) == 1
    base2_field = base2_struct.fields[0]
    assert base2_field.name == "another_field"

    derived_struct = parser.Structs[2]
    assert derived_struct.name == "Derived"
    assert len(derived_struct.fields) == 1
    derived_field = derived_struct.fields[0]
    assert derived_field.name == "derived_field"

    assert len(derived_struct.base_classes) == 2
    base_class = derived_struct.base_classes[0]
    assert base_class.name == "Base"
    assert base_class.access == "public"

    assert len(derived_struct.base_classes) == 2
    base_class2 = derived_struct.base_classes[1]
    assert base_class2.name == "test::Base2"
    assert base_class2.access == "private"
