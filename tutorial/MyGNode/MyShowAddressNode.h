/***************************

@File: MyShowAddressNode.h
@Time: 2021/10/31 12:50 上午
@Desc: 
***************************/

#ifndef DAOGRAPH_MYSHOWADDRESSNODE_H
#define DAOGRAPH_MYSHOWADDRESSNODE_H

#include "DaoGraph.h"
#include "../MyParams/MyParam.h"

class MyShowAddressNode : public DaoGraph::GNode {
public:
    CStatus init() override {
        CStatus status = DAOGRAPH_CREATE_GPARAM(MyParam, "param2")
        return status;
    }

    CStatus run() override {
        auto myParam = DAOGRAPH_GET_GPARAM(MyParam, "param2")
        if (nullptr == myParam) {
            return CStatus("get param2 failed");
        }

        int cnt = 0;
        {
            DAOGRAPH_PARAM_WRITE_CODE_BLOCK(myParam)
            cnt = ++myParam->iCount;
        }

        DaoGraph::DAOGRAPH_ECHO("singleton node run, name is [%s], this address is [0x%x], cnt is [%d].",
                            this->getName().c_str(), (unsigned int*)this, cnt);
        return CStatus();
    }
};

#endif //DAOGRAPH_MYSHOWADDRESSNODE_H
