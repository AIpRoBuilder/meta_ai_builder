/***************************

@File: GMessageUtils.h
@Time: 2022/11/1 22:48
@Desc:
***************************/

#ifndef DAOGRAPH_GMESSAGEUTILS_H
#define DAOGRAPH_GMESSAGEUTILS_H

#include "GMessageManagerSingleton.h"

DAOGRAPH_NAMESPACE_BEGIN

/*******************************
 * 以下是 send recv 的功能对应的接口
 *******************************/
/* 创建一个 topic */
#define DAOGRAPH_CREATE_MESSAGE_TOPIC(Type, topic, size)                                                \
    DaoGraph::GMessageManagerSingleton::get()->template createTopic<Type>(topic, size);                 \

/* 删除一个 topic */
#define DAOGRAPH_REMOVE_MESSAGE_TOPIC(topic)                                                            \
    DaoGraph::GMessageManagerSingleton::get()->removeTopic(topic);                                      \

/* 发送一个 message param */
#define DAOGRAPH_SEND_MPARAM(Type, topic, value, strategy)                                              \
    DaoGraph::GMessageManagerSingleton::get()->template sendTopicValue<Type>(topic, value, strategy);   \

/* 接收一个 message param */
#define DAOGRAPH_RECV_MPARAM(Type, topic, value)                                                        \
    DaoGraph::GMessageManagerSingleton::get()->template recvTopicValue<Type>(topic, value);             \

/* 有等待时间的接收一个 message param */
#define DAOGRAPH_RECV_MPARAM_WITH_TIMEOUT(Type, topic, value, timeout)                                  \
    DaoGraph::GMessageManagerSingleton::get()->template recvTopicValue<Type>(topic, value, timeout);    \

/*******************************
 * 以下是 pub sub 的功能对应的接口
 *******************************/
/* 绑定一个topic，并且返回 conn id 信息 */
#define DAOGRAPH_BIND_MESSAGE_TOPIC(Type, topic, size)                                                  \
    DaoGraph::GMessageManagerSingleton::get()->template bindTopic<Type>(topic, size);                   \

/* 发布一个消息 */
#define DAOGRAPH_PUB_MPARAM(Type, topic, value, strategy)                                               \
    DaoGraph::GMessageManagerSingleton::get()->template pubTopicValue<Type>(topic, value, strategy);    \

/* 订阅一个消息 */
#define DAOGRAPH_SUB_MPARAM(Type, connId, value)                                                        \
    DaoGraph::GMessageManagerSingleton::get()->template subTopicValue<Type>(connId, value);             \

/* 有等待时间的订阅一个消息 */
#define DAOGRAPH_SUB_MPARAM_WITH_TIMEOUT(Type, connId, value, timeout)                                  \
    DaoGraph::GMessageManagerSingleton::get()->template subTopicValue<Type>(connId, value, timeout);    \

/* 删除消息 */
#define DAOGRAPH_DROP_MESSAGE_TOPIC(topic)                                                              \
    DaoGraph::GMessageManagerSingleton::get()->dropTopic(topic);                                        \


/****************************
 * 以下针对所有功能
 ****************************/
/* 清空所有的topic */
#define DAOGRAPH_CLEAR_MESSAGES()                                                                       \
    DaoGraph::GMessageManagerSingleton::get()->clear();                                                 \

DAOGRAPH_NAMESPACE_END

#endif //DAOGRAPH_GMESSAGEUTILS_H
