import clang.cindex as cindex
from .py_variable import PyVariable
from .py_function import PyFunction


class PyField(PyVariable):
    def __init__(self, tu: cindex.TranslationUnit, cursor: cindex.Cursor) -> None:
        super().__init__(tu, cursor)
        self.access = "public"

        if cursor.access_specifier == cindex.AccessSpecifier.PRIVATE:
            self.access = "private"
        elif cursor.access_specifier == cindex.AccessSpecifier.PROTECTED:
            self.access = "protected"
        elif cursor.access_specifier == cindex.AccessSpecifier.PUBLIC:
            self.access = "public"
        else:
            raise ValueError(f"Unknown access specifier: {cursor.access_specifier}")

    def __repr__(self) -> str:
        print(self.annotations)
        return f"<Field name={self.name} type={self.type} access={self.access}>"


class PyMethod(PyFunction):
    def __init__(self, tu: cindex.TranslationUnit, cursor: cindex.Cursor) -> None:
        super().__init__(tu, cursor)
        self.access = "public"
        self.is_static = cursor.is_static_method()
        self.is_constructor = cursor.kind == cindex.CursorKind.CONSTRUCTOR
        self.is_copy_constructor = False
        self.is_move_constructor = False
        self.is_pure_virtual = cursor.is_pure_virtual_method()

        if self.is_constructor:
            params = list(cursor.get_arguments())
            if len(params) == 1:
                param = params[0]
                parent_type = cursor.semantic_parent.type
                if (
                    parent_type.spelling
                    in param.type.get_pointee().get_canonical().spelling
                ):
                    if (
                        param.type.get_canonical().kind
                        == cindex.TypeKind.LVALUEREFERENCE
                    ):
                        self.is_copy_constructor = True
                    else:
                        self.is_move_constructor = True

        if cursor.access_specifier == cindex.AccessSpecifier.PRIVATE:
            self.access = "private"
        elif cursor.access_specifier == cindex.AccessSpecifier.PROTECTED:
            self.access = "protected"
        elif cursor.access_specifier == cindex.AccessSpecifier.PUBLIC:
            self.access = "public"
        else:
            raise ValueError(f"Unknown access specifier: {cursor.access_specifier}")

    def __repr__(self) -> str:
        return f"<Method name={self.name} return_type={self.return_type} args={self.arguments} access={self.access}>"


class PyBase:
    def __init__(self, tu: cindex.TranslationUnit, cursor: cindex.Cursor) -> None:
        self.name = cursor.spelling

        if cursor.access_specifier == cindex.AccessSpecifier.PRIVATE:
            self.access = "private"
        elif cursor.access_specifier == cindex.AccessSpecifier.PROTECTED:
            self.access = "protected"
        elif cursor.access_specifier == cindex.AccessSpecifier.PUBLIC:
            self.access = "public"
        else:
            raise ValueError(f"Unknown access specifier: {cursor.access_specifier}")
