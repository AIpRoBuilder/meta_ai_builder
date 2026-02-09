/***************************

@File: GTopoEngine.cpp
@Time: 2024/1/20 21:56
@Desc: 
***************************/

#include "GTopoEngine.h"

DAOGRAPH_NAMESPACE_BEGIN

CStatus GTopoEngine::setup(const GSortedGElementPtrSet& elements) {
    DAOGRAPH_FUNCTION_BEGIN

    topo_elements_ = GEngine::getTopo(elements);
    DAOGRAPH_RETURN_ERROR_STATUS_BY_CONDITION(topo_elements_.size() != elements.size(),     \
                                            "topo engine parse size is not right")

    DAOGRAPH_FUNCTION_END
}


CStatus GTopoEngine::run() {
    DAOGRAPH_FUNCTION_BEGIN
    for (auto* element : topo_elements_) {
        status += element->fatProcessor(CFunctionType::RUN);
        DAOGRAPH_FUNCTION_CHECK_STATUS
    }
    DAOGRAPH_FUNCTION_END
}

DAOGRAPH_NAMESPACE_END
