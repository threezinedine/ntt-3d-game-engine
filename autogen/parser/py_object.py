import clang.cindex as cindex  # type: ignore


class PyObject:
    def __init__(self, tu: cindex.TranslationUnit, cursor: cindex.Cursor) -> None:
        self.name = cursor.spelling
        self.annotations: dict[str, str] = {}
        self.comment = cursor.raw_comment if cursor.raw_comment else None
        self.namespace: str | None = None

        for child in cursor.get_children():
            if child.kind == cindex.CursorKind.ANNOTATE_ATTR:
                if ":" in child.spelling:
                    key, value = child.spelling.split(":", 1)
                    self.annotations[key] = value
                else:
                    self.annotations[child.spelling] = ""
