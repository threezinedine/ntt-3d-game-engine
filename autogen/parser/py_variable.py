import clang.cindex as cindex
from .py_object import PyObject


class PyVariable(PyObject):
    def __init__(self, tu: cindex.TranslationUnit, cursor: cindex.Cursor) -> None:
        super().__init__(tu, cursor)
        self.type = cursor.type.spelling

    def __repr__(self) -> str:
        return f"<Variable name={self.name} type={self.type}>"
