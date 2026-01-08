import clang.cindex as cindex  # type: ignore
from .py_enum import PyEnum


class Parser:
    def __init__(self) -> None:
        self._temps: dict[str, str] = {}
        self.tu: cindex.TranslationUnit | None = None
        self.Enums: list[PyEnum] = []

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
                py_enum = PyEnum(cursor)
                self.Enums.append(py_enum)
