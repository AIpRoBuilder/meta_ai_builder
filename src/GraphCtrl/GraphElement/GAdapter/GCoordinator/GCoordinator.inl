/***************************

@File: GCoordinator.inl
@Time: 2023/9/8 21:12
@Desc:
***************************/

#ifndef DAOGRAPH_GCOORDINATOR_INL
#define DAOGRAPH_GCOORDINATOR_INL

#include "GCoordinator.h"

DAOGRAPH_NAMESPACE_BEGIN

template<CInt SIZE>
GCoordinator<SIZE>::GCoordinator() {
    element_type_ = GElementType::COORDINATOR;
    session_ = URandom<>::generateSession(DAOGRAPH_STR_COORDINATOR);
}


template<CInt SIZE>
CStatus GCoordinator<SIZE>::run() {
    DAOGRAPH_FUNCTION_BEGIN
    status = (SIZE >= 0)
             ? thread_pool_->createSecondaryThread(SIZE)
             : thread_pool_->releaseSecondaryThread((-1) * SIZE);

    DAOGRAPH_FUNCTION_END
}


template<CInt SIZE>
CVoid GCoordinator<SIZE>::dumpElement(std::ostream& oss) {
    dumpElementHeader(oss);
    dumpPerfInfo(oss);
    oss << "\", shape=diamond]\n";
}


template<CInt SIZE>
CStatus GCoordinator<SIZE>::checkSuitable() {
    DAOGRAPH_FUNCTION_BEGIN
    DAOGRAPH_RETURN_ERROR_STATUS_BY_CONDITION(loop_ > 1,    \
    "cannot set GCoordinator loop > 1")

    DAOGRAPH_FUNCTION_END
}

DAOGRAPH_NAMESPACE_END

#endif //DAOGRAPH_GCOORDINATOR_INL