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

    assert len(parser.Structs) == 1
    cls = parser.Structs[0]
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
