/***************************

@File: UQueueDefine.h
@Time: 2023/9/15 21:31
@Desc: 
***************************/

#ifndef DAOGRAPH_UQUEUEDEFINE_H
#define DAOGRAPH_UQUEUEDEFINE_H

DAOGRAPH_NAMESPACE_BEGIN

/** 当环形队列满的时候，写入信息时候的策略 */
enum class URingBufferPushStrategy {
    WAIT = 1,                 // 等待有数据被消费后，再写入
    REPLACE = 2,              // 替换未被消费的最早进入的内容
    DROP = 3,                 // 丢弃当前信息
};

DAOGRAPH_NAMESPACE_END

#endif //DAOGRAPH_UQUEUEDEFINE_H
