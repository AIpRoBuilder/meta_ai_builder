/***************************

@File: ULruNode.h
@Time: 2021/9/19 12:17 上午
@Desc: 
***************************/

#ifndef DAOGRAPH_ULRUNODE_H
#define DAOGRAPH_ULRUNODE_H

#include "../UtilsObject.h"

DAOGRAPH_NAMESPACE_BEGIN

template<typename K, typename V>
struct ULruNode : public UtilsObject {
    K key_;
    V value_;

    ULruNode(const K& key, const V& value) {
        this->key_ = key;
        this->value_ = value;
    }
};

DAOGRAPH_NAMESPACE_END

#endif //DAOGRAPH_ULRUNODE_H
