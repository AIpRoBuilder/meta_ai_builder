/***************************

@File: GFunction.h
@Time: 2022/1/22 11:45 下午
@Desc: 
***************************/

#ifndef DAOGRAPH_GFUNCTION_H
#define DAOGRAPH_GFUNCTION_H

#include "../GAdapter.h"

DAOGRAPH_NAMESPACE_BEGIN

class GFunction : public GAdapter {
public:
    /**
     * 设置执行函数具体内容
     * @param type
     * @param func
     * @return
     */
    GFunction* setFunction(const CFunctionType& type,
                           DAOGRAPH_CSTATUS_CONST_FUNCTION_REF func);

    // 针对GFunction，是需要写成public的，否则在外部的 lambda中，无法获取
    DAOGRAPH_DECLARE_GPARAM_MANAGER_WRAPPER

    DAOGRAPH_DECLARE_GEVENT_MANAGER_WRAPPER

protected:
    explicit GFunction();

    CStatus init() final;

    CStatus run() final;

    CStatus destroy() final;

private:
    DAOGRAPH_CSTATUS_FUNCTION init_function_ = nullptr;
    DAOGRAPH_CSTATUS_FUNCTION run_function_ = nullptr;
    DAOGRAPH_CSTATUS_FUNCTION destroy_function_ = nullptr;

    friend class GPipeline;
};

using GFunctionPtr = GFunction *;
using GFunctionPPtr = GFunctionPtr *;

DAOGRAPH_NAMESPACE_END

#endif //DAOGRAPH_GFUNCTION_H
