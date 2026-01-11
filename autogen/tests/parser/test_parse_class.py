from parser import *


def test_parse_class():
    parser = Parser()

    parser.add_code(
        "class.cpp",
        """
        class Animal {
        public:
            void speak();
        protected:
            int age;
        private:
            float weight;
        };
        """,
    )

    parser.parse("class.cpp")

    assert len(parser.Classes) == 1
    cls = parser.Classes[0]
    assert cls.name == "Animal"

    assert len(cls.methods) == 1
    method = cls.methods[0]
    assert method.name == "speak"
    assert method.return_type == "void"

    assert len(cls.fields) == 2
    field1 = cls.fields[0]
    assert field1.name == "age"
    assert field1.type == "int"

    field2 = cls.fields[1]
    assert field2.name == "weight"
    assert field2.type == "float"


def test_parse_static_method():
    parser = Parser()

    parser.add_code(
        "static_method.cpp",
        """
        class MathUtils {
        public:
            static int add(int a, int b);
        };
        """,
    )

    parser.parse("static_method.cpp")

    assert len(parser.Classes) == 1
    cls = parser.Classes[0]
    assert cls.name == "MathUtils"

    assert len(cls.methods) == 1
    method = cls.methods[0]
    assert method.name == "add"
    assert method.return_type == "int"
    assert method.is_static is True


def test_parse_inheritance():
    parser = Parser()

    parser.add_code(
        "inheritance.cpp",
        """
        class Vehicle {
        public:
            void move();
        };

        class Car : public Vehicle {
        public:
            void honk();
        };
        """,
    )

    parser.parse("inheritance.cpp")

    assert len(parser.Classes) == 2

    vehicle_cls = next(cls for cls in parser.Classes if cls.name == "Vehicle")
    assert len(vehicle_cls.methods) == 1
    vehicle_method = vehicle_cls.methods[0]
    assert vehicle_method.name == "move"

    car_cls = next(cls for cls in parser.Classes if cls.name == "Car")
    assert len(car_cls.methods) == 1
    car_method = car_cls.methods[0]
    assert car_method.name == "honk"

    assert len(car_cls.base_classes) == 1
    base = car_cls.base_classes[0]
    assert base.name == "Vehicle"


def test_parse_copy_constructor():
    parser = Parser()

    parser.add_code(
        "copy_constructor.cpp",
        """
        class Point {
        public:
            Point(int x, int y);
            Point(const Point& other); // Copy constructor
            Point(Point&& other); // Move constructor
        private:
            int x;
            int y;
        };
        """,
    )

    parser.parse("copy_constructor.cpp")

    assert len(parser.Classes) == 1
    cls = parser.Classes[0]
    assert cls.name == "Point"

    assert len(cls.constructors) == 3

    constructor1 = cls.constructors[0]
    assert constructor1.name == "Point"
    assert constructor1.is_constructor is True
    assert constructor1.is_copy_constructor is False
    assert constructor1.is_move_constructor is False

    constructor2 = cls.constructors[1]
    assert constructor2.name == "Point"
    assert constructor2.is_constructor is True
    assert constructor2.is_copy_constructor is True
    assert constructor2.is_move_constructor is False

    constructor3 = cls.constructors[2]
    assert constructor3.name == "Point"
    assert constructor3.is_constructor is True
    assert constructor3.is_copy_constructor is False
    assert constructor3.is_move_constructor is True


def test_parse_abstract_class():
    parser = Parser()

    parser.add_code(
        "abstract_class.cpp",
        """
        class Shape {
        public:
            virtual void draw() = 0; // Pure virtual function
        };

        class Circle : public Shape {
        public:
            void draw() override;
        };
        """,
    )

    parser.parse("abstract_class.cpp")

    assert len(parser.Classes) == 2
    cls = parser.Classes[0]
    assert cls.is_abstract is True

    assert len(cls.methods) == 1
    method = cls.methods[0]
    assert method.name == "draw"
    assert method.is_pure_virtual is True

    cls2 = parser.Classes[1]
    assert cls2.is_abstract is False
    assert len(cls2.methods) == 1
    method2 = cls2.methods[0]
    assert method2.name == "draw"
    assert method2.is_pure_virtual is False
