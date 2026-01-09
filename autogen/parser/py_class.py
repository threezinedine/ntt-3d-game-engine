import clang.cindex as cindex
from .py_struct import PyStruct


class PyClass(PyStruct):
    def __init__(self, tu: cindex.TranslationUnit, cursor: cindex.Cursor) -> None:
        super().__init__(tu, cursor)

    def __repr__(self) -> str:
        return f"<Class name={self.name} fields={len(self.fields)} methods={len(self.methods)}>"
