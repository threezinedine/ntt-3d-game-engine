import clang.cindex as cindex  # type: ignore
from .py_object import PyObject


class PyConstant(PyObject):
    def __init__(self, cursor: cindex.Cursor) -> None:
        super().__init__(cursor)

        self.value: str | int | float | None = ""

        for child in cursor.get_children():
            print(child.kind)
            if child.kind == cindex.CursorKind.INTEGER_LITERAL:
                self.value = "".join(token.spelling for token in child.get_tokens())
            elif child.kind == cindex.CursorKind.FLOATING_LITERAL:
                self.value = "".join(token.spelling for token in child.get_tokens())
            elif child.kind == cindex.CursorKind.STRING_LITERAL:
                self.value = "".join(token.spelling for token in child.get_tokens())

    def __repr__(self) -> str:
        return f"<Constant name={self.name} value={self.value}>"
