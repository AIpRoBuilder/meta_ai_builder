/***************************

@File: GStage.cpp
@Time: 2024/12/28 23:02
@Desc: 
***************************/

#include "GStage.h"

DAOGRAPH_NAMESPACE_BEGIN

CVoid GStage::launch(GStageParamPtr param) {
}


GStage::GStage() {
    session_ = URandom<>::generateSession(DAOGRAPH_STR_STAGE);
}


GStage::~GStage() {
    DAOGRAPH_DELETE_PTR(param_);
}


GStagePtr GStage::setThreshold(CInt threshold) {
    threshold_ = threshold;
    return this;
}


CVoid GStage::waiting() {
    {
        DAOGRAPH_LOCK_GUARD wm(waiting_mutex_);
        cur_value_++;
        if (cur_value_ >= threshold_) {
            // 如果超过了 threshold，则打开全部
            launch(param_);
            cur_value_ = 0;
            locker_.cv_.notify_all();
            return;
        }
    }

    DAOGRAPH_UNIQUE_LOCK lk(locker_.mtx_);
    locker_.cv_.wait(lk, [this] {
        return 0 == cur_value_ || cur_value_ >= threshold_;
    });
}

DAOGRAPH_NAMESPACE_END