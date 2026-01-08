import clang.cindex as cindex  # type: ignore
from .py_object import PyObject
from .py_constant import PyConstant


class PyEnum(PyObject):
    def __init__(self, cursor: cindex.Cursor) -> None:
        super().__init__(cursor)
        self.constants: list[PyConstant] = []

        for child in cursor.get_children():
            if child.kind == cindex.CursorKind.ENUM_CONSTANT_DECL:
                constant = PyConstant(child)
                self.constants.append(constant)
                constant.value = child.enum_value

    def __repr__(self) -> str:
        return f"<Enum name={self.name} constants={self.constants}>"
