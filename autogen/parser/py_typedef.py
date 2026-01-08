import clang.cindex as cindex


class PyTypedef:
    def __init__(self, cursor: cindex.Cursor) -> None:
        self.name = cursor.spelling
        self.underlying_type = cursor.underlying_typedef_type.spelling
        self.namespace: str | None = None
        self.annotations: list[str] = []

        for child in cursor.get_children():
            if child.kind == cindex.CursorKind.ANNOTATE_ATTR:
                self.annotations.append(child.spelling)

    def __repr__(self) -> str:
        return f"<TypeDef name={self.name} underlying_type={self.underlying_type}>"
