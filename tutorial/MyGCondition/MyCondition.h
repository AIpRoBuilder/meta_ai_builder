/***************************

@File: MyCondition.h
@Time: 2021/6/19 10:17 下午
@Desc: 
***************************/

#ifndef DAOGRAPH_MYCONDITION_H
#define DAOGRAPH_MYCONDITION_H

#include "DaoGraph.h"

class MyCondition : public DaoGraph::GCondition {
public:
    /**
     * 表示在condition中，需要执行第几个。
     * 0表示第0个；1表示第1个；以此类推
     * -1 表示最后一个
     * 其中，CIndex is for [int]
     * @return
     */
    CIndex choose () override {
        return 1;
    }
};

#endif // DAOGRAPH_MYCONDITION_H
