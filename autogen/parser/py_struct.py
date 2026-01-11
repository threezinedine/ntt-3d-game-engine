import clang.cindex as cindex
from .py_object import PyObject
from .py_common import *


class PyStruct(PyObject):
    def __init__(self, tu: cindex.TranslationUnit, cursor: cindex.Cursor) -> None:
        super().__init__(tu, cursor)
        self.fields: list[PyField] = []
        self.methods: list[PyMethod] = []
        self.constructors: list[PyMethod] = []
        self.destructor: PyMethod | None = None
        self.base_classes: list[PyBase] = []
        self.is_abstract: bool = False

        for child in cursor.get_children():
            if child.kind == cindex.CursorKind.FIELD_DECL:
                field = PyField(tu, child)
                self.fields.append(field)
            elif child.kind == cindex.CursorKind.CXX_METHOD:
                method = PyMethod(tu, child)
                self.methods.append(method)
            elif child.kind == cindex.CursorKind.CONSTRUCTOR:
                constructor = PyMethod(tu, child)
                self.constructors.append(constructor)
            elif child.kind == cindex.CursorKind.DESTRUCTOR:
                destructor = PyMethod(tu, child)
                self.destructor = destructor
            elif child.kind == cindex.CursorKind.CXX_BASE_SPECIFIER:
                base = PyBase(tu, child)
                self.base_classes.append(base)

        for method in self.methods:
            if method.is_pure_virtual:
                self.is_abstract = True
                break

    def __repr__(self) -> str:
        return f"<Struct name={self.name} fields={self.fields} methods={len(self.methods)}>"
