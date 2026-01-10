from parser import *


def test_to_pyi_type():
    assert to_pyi_type("int") == "int"
    assert to_pyi_type("float") == "float"
    assert to_pyi_type("double") == "float"
    assert to_pyi_type("char") == "str"
    assert to_pyi_type("bool") == "bool"
    assert to_pyi_type("void") == "None"
    assert to_pyi_type("std::string") == "str"
    assert to_pyi_type("long") == "int"
    assert to_pyi_type("short") == "int"
    assert to_pyi_type("unsigned int") == "int"
    assert to_pyi_type("unsigned long") == "int"
    assert to_pyi_type("unsigned short") == "int"
    assert to_pyi_type("const char*") == "str"
    assert to_pyi_type("int*") == "Optional[int]"
    assert to_pyi_type("const float*") == "Optional[float]"
    assert to_pyi_type("double&") == "float"
    assert to_pyi_type("const bool&") == "bool"
    assert to_pyi_type("int[23]") == "List[int]"
    assert to_pyi_type("const std::string[10]") == "List[str]"
    assert to_pyi_type("CustomType[]") == "List[CustomType]"
    assert to_pyi_type("CustomType[18]") == "List[CustomType]"
    assert to_pyi_type("char[25]") == "str"


def test_is_type_function_pointer():
    assert is_type_function_pointer("void (*)()") is True
    assert convert_function_pointer_type("void (*)()") == "Callable[[], None]"

    assert is_type_function_pointer("int (*)(float, double)") is True
    assert (
        convert_function_pointer_type("int (*)(float, double)")
        == "Callable[[float, float], int]"
    )

    assert is_type_function_pointer("float") is False
    assert is_type_function_pointer("int*") is False


def test_convert_std_function():
    assert to_pyi_type("std::function<void()>") == "Callable[[], None]"
    assert (
        to_pyi_type("std::function<int(float, double)>")
        == "Callable[[float, float], int]"
    )
    assert (
        to_pyi_type("std::function<std::string(const char*)>") == "Callable[[str], str]"
    )


def test_convert_array_type():
    assert to_pyi_type("char[256]") == "str"
    assert to_pyi_type("int[10]") == "List[int]"
    assert to_pyi_type("float[5]") == "List[float]"
    assert to_pyi_type("CustomType[20]") == "List[CustomType]"
    assert to_pyi_type("Array<int>") == "List[int]"
    assert to_pyi_type("Array<CustomType>") == "List[CustomType]"
