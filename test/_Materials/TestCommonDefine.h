/***************************

@File: TestCommonDefine.h
@Time: 2023/12/29 22:00
@Desc: 
***************************/

#ifndef DAOGRAPH_TESTCOMMONDEFINE_H
#define DAOGRAPH_TESTCOMMONDEFINE_H

#include <atomic>

#include "DaoGraph.h"

std::atomic<unsigned int> g_test_node_cnt = {0};
static const char* g_test_message_key = "test-message-key";
static const char* g_add1_by_param_key = "add1-by-param";

#endif //DAOGRAPH_TESTCOMMONDEFINE_H
