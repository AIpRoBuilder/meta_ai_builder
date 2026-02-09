from pycgraph import GElement, GNode, GPipeline, CStatus, register, register_class
import copy



@register_class
class MyNode(GNode):
    signature = "MyNode"

    def run(self) -> CStatus:
        print("MyNode running from factory-created instance")
        return CStatus()
    def clone(self):
        """Create a copy of this node"""
        return self

@register_class
class OtherNode(GNode):
    signature = "OtherNode"

    def run(self) -> CStatus:
        print("OtherNode running from factory-created instance")
        return CStatus()
    def clone(self):
        """Create a copy of this node"""
        return self
def main():
    # Classes are automatically registered via decorators above
    # No need to manually call GElement.registerClass() anymore

    # Create an instance by name
    # refire.ai
    node = GElement.create("MyNode")
    

    # Check registered classes
    print(GElement.registeredClasses())
    pipeline = GPipeline()
    a = pipeline.buildFromJson("/Users/xiechuxi/Desktop/codes/meta_ai_builder/python/tutorial/example.json")
    print("Pipeline built from JSON:", a.getInfo())
    pipeline.process()
    pipeline.destroy()


if __name__ == "__main__":
    main()
