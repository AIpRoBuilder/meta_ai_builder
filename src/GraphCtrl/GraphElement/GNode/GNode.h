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
public:
    /**
     * 开启/关闭run前等待输入
     * @param enable
     * @return
     */
    GNode* setWaitForInput(CBool enable = true);

    /**
     * 设置等待输入提示信息
     * @param prompt
     * @return
     */
    GNode* setInputPrompt(const std::string& prompt);

    /**
     * 设置输入处理回调
     * @param handler
     * @return
     */
    GNode* setInputHandler(const GInputHandler& handler);

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

    /**
     * 子类可重载，定义收到输入后的处理逻辑
     * @param input
     * @return
     */
    CStatus onInput(const std::string& input) override;

private:
    GNodeType node_type_ { GNodeType::BASIC };                    // 节点类型
};

using GNodePtr = GNode *;
using GNodePtrArr = std::vector<GNodePtr>;

DAOGRAPH_NAMESPACE_END

#endif //DAOGRAPH_GNODE_H