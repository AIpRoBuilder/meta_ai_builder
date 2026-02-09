/***************************

@File: TestGParams.h
@Time: 2024/1/1 16:58
@Desc: 
***************************/

#ifndef DAOGRAPH_TESTGPARAMS_H
#define DAOGRAPH_TESTGPARAMS_H

#include "TestCommonDefine.h"

class TestGMessageParam : public DaoGraph::GMessageParam {
public:
    unsigned int num_ {0};
};


class TestAdd1GParam : public DaoGraph::GParam {
public:
    int cnt_ = 0;
};

#endif //DAOGRAPH_TESTGPARAMS_H
