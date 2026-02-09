"""

@File: T00-HelloDaoGraph
@Time: 2025/2/21 23:25
@Desc: 
"""

from pydaograph import GPipeline

from MyGNode.HelloDaoGraphNode import HelloDaoGraphNode


def tutorial_hello_daograph():
    pipeline = GPipeline()
    pipeline.registerGElement(HelloDaoGraphNode())
    pipeline.process()


if __name__ == '__main__':
    tutorial_hello_daograph()
