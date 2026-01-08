import clang.cindex as cindex
from .py_object import PyObject
from .py_variable import PyVariable


class PyUnion(PyObject):
    def __init__(self, tu: cindex.TranslationUnit, cursor: cindex.Cursor) -> None:
        super().__init__(tu, cursor)
        self.fields: list[PyVariable] = []

        for child in cursor.get_children():
            if child.kind == cindex.CursorKind.FIELD_DECL:
                field = PyVariable(tu, child)
                self.fields.append(field)

    def __repr__(self) -> str:
        return f"<Union name={self.name} fields={self.fields}>"
