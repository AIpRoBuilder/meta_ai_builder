/***************************

@File: MyVersionParam.h
@Time: 2022/5/29 22:29
@Desc: 
***************************/

#ifndef DAOGRAPH_MYVERSIONPARAM_H
#define DAOGRAPH_MYVERSIONPARAM_H

#include "DaoGraph.h"

const static char* VERSION_PARAM_KEY = "version";

struct MyVersionParam : public DaoGraph::GElementParam {
    CVoid clone(DaoGraph::GPassedParamPtr param) override {
        auto* ptr = dynamic_cast<MyVersionParam *>(param);
        if (nullptr == ptr) {
            return;    // 理论不存在的情况
        }

        this->priority_ = ptr->priority_;
        this->secondary_ = ptr->secondary_;
    }

    int priority_ = 0;    // 表示 主版本号
    int secondary_ = 0;    // 表示 辅版本号
};


#endif //DAOGRAPH_MYVERSIONPARAM_H
