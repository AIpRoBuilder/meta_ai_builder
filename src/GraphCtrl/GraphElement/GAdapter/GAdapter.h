/***************************

@File: GAdapter.h
@Time: 2021/10/30 10:22 下午
@Desc:
***************************/

#ifndef DAOGRAPH_GADAPTER_H
#define DAOGRAPH_GADAPTER_H

#include "../GElement.h"

DAOGRAPH_NAMESPACE_BEGIN

class GAdapter : public GElement {
protected:
    explicit GAdapter() {
        element_type_ = GElementType::ADAPTER;
    }

    friend class GPipeline;
};

using GAdapterPtr = GAdapter *;

DAOGRAPH_NAMESPACE_END

#endif //DAOGRAPH_GADAPTER_H
