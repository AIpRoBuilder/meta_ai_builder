/***************************

@File: MyTemplateV2Node.h
@Time: 2024/2/1 21:51
@Desc: 
***************************/

#ifndef DAOGRAPH_MYTEMPLATEV2NODE_H
#define DAOGRAPH_MYTEMPLATEV2NODE_H

#include "DaoGraph.h"

template<int TID>
class MyTemplateV2Node : public DaoGraph::GNode {
public:
    CStatus run() override {
        DaoGraph::DAOGRAPH_ECHO("[MyTemplateV2Node] template id = [%d] ", TID);
        return CStatus();
    }
};

#endif //DAOGRAPH_MYTEMPLATEV2NODE_H
