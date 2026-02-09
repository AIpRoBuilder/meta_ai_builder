/***************************

@File: GNode.h
@Time: 2021/6/1 10:13 下午
@Desc: 
***************************/

#ifndef DAOGRAPH_GNODE_H
#define DAOGRAPH_GNODE_H

#include "GNodeDefine.h"

DAOGRAPH_NAMESPACE_BEGIN

class GNode : public GElement {
protected:
    /**
     * 构造函数
     */
    explicit GNode();

    /**
     * 设置节点类型
     * @param type
     * @return
     */
    GNode* setType(const GNodeType& type);

    /**
     * 并发执行所有的task信息
     * @param tasks
     * @param ttl
     * @return
     */
    CStatus spawn(const UTaskGroup& tasks, CMSec ttl = DAOGRAPH_MAX_BLOCK_TTL);

private:
    GNodeType node_type_ { GNodeType::BASIC };                    // 节点类型
};

using GNodePtr = GNode *;
using GNodePtrArr = std::vector<GNodePtr>;

DAOGRAPH_NAMESPACE_END

#endif //DAOGRAPH_GNODE_H