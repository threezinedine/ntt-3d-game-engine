import re
from typing import Callable


def to_pyi_type(type_str: str) -> str:
    """
    Convert a C/C++ type string to a Python type hint string.

    :param type_str: The C/C++ type string.
    :return: The corresponding Python type hint string.
    """
    type_mappings: dict[str, str | Callable[[str], str]] = {
        "int": "int",
        "float": "float",
        "double": "float",
        "char": "str",
        "bool": "bool",
        "void": "None",
        "string": "str",
        "long": "int",
        "short": "int",
        "unsigned int": "int",
        "unsigned long": "int",
        "unsigned short": "int",
        "const char*": "str",
        "const char *": "str",
        "Array<.*>": lambda s: f"List[{to_pyi_type(s[6:-1].strip())}]",
        r"std::function<.*>": lambda s: convert_std_function(s[14:-1].strip()),
        r".*\s*::\s*.*": lambda s: to_pyi_type(s[s.rindex("::") + 2 :].strip()),
        r"char\[\s*\d*\s*\]": "str",
        r".*\s*\*\s*$": lambda s: f"Optional[{to_pyi_type(s[:-1].strip())}]",
        r"const (.*)": lambda s: to_pyi_type(s[6:].strip()),
        r".*\s*&\s*$": lambda s: to_pyi_type(s[:-1].strip()),
        r".*\s*\[\s*\d*\s*\]\s*$": lambda s: f"List[{to_pyi_type(s[:s.rindex('[')].strip())}]",
    }

    stripped = type_str.strip()

    for pattern, output in type_mappings.items():
        if pattern == stripped or re.fullmatch(pattern, stripped):
            if isinstance(output, str):
                return output
            else:
                return output(stripped)

    return stripped


def is_type_function_pointer(type_str: str) -> bool:
    stripped = type_str.strip()
    return stripped.endswith("(*)()") or (
        "(*)(" in stripped and stripped.index("(*)(") > 0
    )


def convert_function_pointer_type(type_str: str) -> str:
    stripped = type_str.strip()
    if not is_type_function_pointer(stripped):
        return stripped

    # Extract return type and parameters
    ret_and_params = stripped.split("(*)")
    return_type = to_pyi_type(ret_and_params[0].strip())
    params = ret_and_params[1].strip("() ")

    # Convert to Python callable type
    if params == "":
        python_type = f"Callable[[], {return_type}]"
    else:
        param_list = [to_pyi_type(param.strip()) for param in params.split(",")]
        python_type = f"Callable[[{', '.join(param_list)}], {return_type}]"

    return python_type


def convert_std_function(type_str: str) -> str:
    """
    convert from ret_type(params...) to Callable[[params...], ret_type]
    """
    parts = type_str.split("(")
    return_type = to_pyi_type(parts[0].strip())
    params = parts[1].rstrip(")").strip()

    if params == "":
        python_type = f"Callable[[], {return_type}]"
    else:
        param_list = [to_pyi_type(param.strip()) for param in params.split(",")]
        python_type = f"Callable[[{', '.join(param_list)}], {return_type}]"

    return python_type


def is_array_type(type_str: str) -> bool:
    stripped = type_str.strip()
    return (
        re.fullmatch(r".*\s*\[\s*\d*\s*\]\s*$", stripped) is not None
        or re.fullmatch(r"Array<.*>", stripped) is not None
    )


def get_array_element_type(type_str: str) -> str:
    stripped = type_str.strip()
    array_match = re.fullmatch(r"(.*)\s*\[\s*\d*\s*\]\s*$", stripped)
    if array_match:
        return to_pyi_type(array_match.group(1).strip())

    array_template_match = re.fullmatch(r"Array<(.*)>", stripped)
    if array_template_match:
        return to_pyi_type(array_template_match.group(1).strip())

    return stripped
