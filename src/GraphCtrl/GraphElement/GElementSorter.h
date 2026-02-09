/***************************

@File: GElementSorter.h
@Time: 2022/5/1 11:55
@Desc: 
***************************/

#ifndef DAOGRAPH_GELEMENTSORTER_H
#define DAOGRAPH_GELEMENTSORTER_H

#include <set>

#include "GElement.h"

DAOGRAPH_NAMESPACE_BEGIN

class GElementSorter : public CStruct {
public:
    /**
     * 当level不同的时候，level值小的element优先被init和destroy
     * 默认情况下， init和destroy顺序随机
     * @param a
     * @param b
     * @return
     */
    CBool operator()(GElementPtr a, GElementPtr b) const {
        return (a->level_ == b->level_) ? (a < b) : (a->level_ < b->level_);
    }
};

using GSortedGElementPtrSet = std::set<GElementPtr, GElementSorter>;

DAOGRAPH_NAMESPACE_END

#endif //DAOGRAPH_GELEMENTSORTER_H
