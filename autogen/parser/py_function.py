import clang.cindex as cindex
from .py_object import PyObject
from .py_variable import PyVariable


class PyArgument(PyVariable):
    def __init__(self, tu: cindex.TranslationUnit, cursor: cindex.Cursor) -> None:
        super().__init__(tu, cursor)
        self.default_value: str | int | float | bool | None = None

    def __repr__(self) -> str:
        return f"<Argument name={self.name} type={self.type}>"


class PyFunction(PyObject):
    def __init__(self, tu: cindex.TranslationUnit, cursor: cindex.Cursor) -> None:
        super().__init__(tu, cursor)
        self.return_type = cursor.result_type.spelling
        self.arguments: list[PyArgument] = []

        for child in cursor.get_children():
            if child.kind == cindex.CursorKind.PARM_DECL:
                argument = PyArgument(tu, child)
                self.arguments.append(argument)

                tokens = list(tu.get_tokens(extent=child.extent))
                meet_equal = False
                for token in tokens:
                    if token.spelling == "=":
                        meet_equal = True
                    elif meet_equal:
                        value = token.spelling
                        if value == "true":
                            argument.default_value = True
                        elif value == "false":
                            argument.default_value = False
                        elif value.isdigit():
                            argument.default_value = int(value)
                        else:
                            try:
                                argument.default_value = float(value)
                            except ValueError:
                                argument.default_value = value
                        break

    def __repr__(self) -> str:
        return f"<Function name={self.name} return_type={self.return_type}>"
