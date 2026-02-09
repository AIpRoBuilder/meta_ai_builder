/***************************

@File: MyMutable.h
@Time: 2023/11/15 21:22
@Desc: 
***************************/

#ifndef DAOGRAPH_MYMUTABLE_H
#define DAOGRAPH_MYMUTABLE_H

#include "DaoGraph.h"
#include "../MyParams/MyParam.h"

class MyMutable : public DaoGraph::GMutable {
public:
    CStatus reshape(DaoGraph::GElementPtrArr& elements) override {
        auto param = DAOGRAPH_GET_GPARAM_WITH_NO_EMPTY(MyParam, "param1")
        int count = param->iCount % 4;
        if (0 == count) {
            DaoGraph::DAOGRAPH_ECHO("---- run as a->[b,c]");
            (*elements[0])-->elements[1] & elements[2];
        } else if (1 == count) {
            DaoGraph::DAOGRAPH_ECHO("---- run as c(*3)->b->a");
            (*elements[2])--*3>elements[1];    // (*elements[2]) 循环3次，并且在 elements[1] 之前执行
            (*elements[1])-->elements[0];
        } else if (2 == count) {
            // 只有在这里，显示描述的element，被激活执行。这里， elements[1] 不执行
            DaoGraph::DAOGRAPH_ECHO("---- run as a->c, do not run b");
            (*elements[0])-->elements[2];
        } else if (3 == count) {
            DaoGraph::DAOGRAPH_ECHO("---- run as [a,b(*2),c]");
            (*elements[0])--;
            (*elements[1])--*2;
            (*elements[2])--;
        } else {
            DaoGraph::DAOGRAPH_ECHO("---- run nothing");    // 理论不会进入这种情况
        }

        return CStatus();
    }
};

#endif //DAOGRAPH_MYMUTABLE_H
