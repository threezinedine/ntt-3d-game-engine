import clang.cindex as cindex


class PyTypedef:
    def __init__(self, tu: cindex.TranslationUnit, cursor: cindex.Cursor) -> None:
        self.name = cursor.spelling
        self.underlying_type = cursor.underlying_typedef_type.spelling
        self.namespace: str | None = None
        self.annotations: list[str] = []

        for child in cursor.get_children():
            if child.kind == cindex.CursorKind.ANNOTATE_ATTR:
                self.annotations.append(child.spelling)

    def __repr__(self) -> str:
        return f"<TypeDef name={self.name} underlying_type={self.underlying_type}>"


class PyTypeAlias(PyTypedef):
    def __init__(self, tu: cindex.TranslationUnit, cursor: cindex.Cursor) -> None:
        super().__init__(tu, cursor)

    def __repr__(self) -> str:
        return f"<TypeAlias name={self.name} underlying_type={self.underlying_type}>"
