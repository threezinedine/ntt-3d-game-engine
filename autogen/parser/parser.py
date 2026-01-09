import clang.cindex as cindex  # type: ignore
from .py_enum import PyEnum
from .py_typedef import PyTypedef
from .py_variable import PyVariable
from .py_union import PyUnion
from .py_function import PyFunction
from .py_struct import PyStruct  # pyright: ignore[reportUnusedImport]


class Parser:
    def __init__(self) -> None:
        self._temps: dict[str, str] = {}
        self.tu: cindex.TranslationUnit | None = None
        self.Enums: list[PyEnum] = []
        self.Typedefs: list[PyTypedef] = []
        self.Variables: list[PyVariable] = []
        self.Unions: list[PyUnion] = []
        self.Functions: list[PyFunction] = []
        self.Structs: list[PyStruct] = []

    def add_code(self, name: str, code: str) -> None:
        self._temps[name] = code

    def parse(self, filename: str) -> None:
        index = cindex.Index.create()
        self.tu = index.parse(
            filename,
            args=[
                "-std=c++17",
                "-I/usr/include",
                "-I/usr/local/include",
            ],
            unsaved_files=list(self._temps.items()),
            options=cindex.TranslationUnit.PARSE_DETAILED_PROCESSING_RECORD,
        )

        assert self.tu is not None, "Translation unit is None"

        for cursor in self.tu.cursor.get_children():
            if cursor.kind == cindex.CursorKind.ENUM_DECL:
                py_enum = PyEnum(self.tu, cursor)
                self.Enums.append(py_enum)
            elif cursor.kind == cindex.CursorKind.TYPEDEF_DECL:
                py_typedef = PyTypedef(self.tu, cursor)
                self.Typedefs.append(py_typedef)
            elif cursor.kind == cindex.CursorKind.VAR_DECL:
                py_variable = PyVariable(self.tu, cursor)
                self.Variables.append(py_variable)
            elif cursor.kind == cindex.CursorKind.UNION_DECL:
                py_union = PyUnion(self.tu, cursor)
                self.Unions.append(py_union)
            elif cursor.kind == cindex.CursorKind.FUNCTION_DECL:
                py_function = PyFunction(self.tu, cursor)
                self.Functions.append(py_function)
            elif cursor.kind == cindex.CursorKind.STRUCT_DECL:
                py_struct = PyStruct(self.tu, cursor)
                self.Structs.append(py_struct)
            elif cursor.kind == cindex.CursorKind.NAMESPACE:
                namespace = cursor.spelling
                for child in cursor.get_children():
                    if child.kind == cindex.CursorKind.ENUM_DECL:
                        py_enum = PyEnum(self.tu, child)
                        py_enum.namespace = namespace
                        self.Enums.append(py_enum)
                    elif child.kind == cindex.CursorKind.TYPEDEF_DECL:
                        py_typedef = PyTypedef(self.tu, child)
                        py_typedef.namespace = namespace
                        self.Typedefs.append(py_typedef)
                    elif child.kind == cindex.CursorKind.VAR_DECL:
                        py_variable = PyVariable(self.tu, child)
                        py_variable.namespace = namespace
                        self.Variables.append(py_variable)
                    elif child.kind == cindex.CursorKind.UNION_DECL:
                        py_union = PyUnion(self.tu, child)
                        py_union.namespace = namespace
                        self.Unions.append(py_union)
                    elif child.kind == cindex.CursorKind.FUNCTION_DECL:
                        py_function = PyFunction(self.tu, child)
                        py_function.namespace = namespace
                        self.Functions.append(py_function)
                    elif child.kind == cindex.CursorKind.STRUCT_DECL:
                        py_struct = PyStruct(self.tu, child)
                        py_struct.namespace = namespace
                        self.Structs.append(py_struct)
