/***************************

@File: GStageManagerWrapper.h
@Time: 2024/12/15 17:48
@Desc: 
***************************/

#ifndef DAOGRAPH_GSTAGEMANAGERWRAPPER_H
#define DAOGRAPH_GSTAGEMANAGERWRAPPER_H

#include "GStage.h"
#include "GStageManager.h"

DAOGRAPH_NAMESPACE_BEGIN

#define DAOGRAPH_DECLARE_GSTAGE_MANAGER_WRAPPER_WITH_MEMBER             \
protected:                                                           \
    /**
     * 进入特定阶段，等待同步
     * @param key
     */                                                              \
    CStatus enterStage(const std::string& key) {                     \
        DAOGRAPH_FUNCTION_BEGIN                                        \
        DAOGRAPH_ASSERT_NOT_NULL(stage_manager_);                      \
        status = stage_manager_->waitForReady(key);                  \
        DAOGRAPH_FUNCTION_END                                          \
    }                                                                \
                                                                     \
private:                                                             \
    CVoidPtr setGStageManager(GStageManagerPtr sm) {                 \
        stage_manager_ = sm;                                         \
        return this;                                                 \
    }                                                                \
                                                                     \
private:                                                             \
    GStageManagerPtr stage_manager_ { nullptr };                     \
                                                                     \

DAOGRAPH_NAMESPACE_END

#endif //DAOGRAPH_GSTAGEMANAGERWRAPPER_H
