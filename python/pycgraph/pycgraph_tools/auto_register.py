from .. import GElement

def register(name: str = None):
    """
    Decorator to automatically register a GElement subclass with the factory.

    Usage:
        @register("MyNode")
        class MyNode(GNode):
            def run(self) -> CStatus:
                return CStatus()

        # Or use the class name automatically:
        @register()
        class MyNode(GNode):
            def run(self) -> CStatus:
                return CStatus()

        # Then create instances via:
        node = GElement.create("MyNode")

    Args:
        name: The name to register the class under. If None, uses the class name.
    """
    def decorator(cls):
        register_name = name if name is not None else cls.__name__
        GElement.registerClass(register_name, lambda: cls())
        return cls
    return decorator


def register_class(cls):
    """
    Simple decorator to register a class using its __name__.

    Usage:
        @register_class
        class MyNode(GNode):
            def run(self) -> CStatus:
                return CStatus()
    """
    GElement.registerClass(cls.__name__, lambda: cls())
    return cls
