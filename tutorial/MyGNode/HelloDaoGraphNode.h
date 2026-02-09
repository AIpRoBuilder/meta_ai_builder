/***************************

@File: HelloDaoGraphNode.h
@Time: 2021/6/24 1:06 上午
@Desc: 
***************************/

#ifndef DAOGRAPH_HELLODAOGRAPHNODE_H
#define DAOGRAPH_HELLODAOGRAPHNODE_H

#include "DaoGraph.h"

class HelloDaoGraphNode : public DaoGraph::GNode {
public:
    CStatus run() override {
        std::cout << "Hello, DaoGraph." << std::endl;
        return CStatus();
    }
};

#endif //DAOGRAPH_HELLODAOGRAPHNODE_H
