"""

@File: HelloDaoGraphPyNode
@Time: 2025/2/21 23:30
@Desc: 
"""

from pydaograph import GNode, CStatus

class HelloDaoGraphNode(GNode):
    def run(self):
        print("Hello, pydaograph.")
        return CStatus()
