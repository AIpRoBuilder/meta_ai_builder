/***************************

@File: MyTemplateDaemon.h
@Time: 2022/12/24 00:14
@Desc: 
***************************/

#ifndef DAOGRAPH_MYTEMPLATEDAEMON_H
#define DAOGRAPH_MYTEMPLATEDAEMON_H

#include <string>

#include "DaoGraph.h"

template<class ...Args>
class MyTemplateDaemon : public DaoGraph::GTemplateDaemon<Args...> {
public:
    explicit MyTemplateDaemon(int index) {
        index_ = index;
    }

    void daemonTask(DaoGraph::GDaemonParamPtr param) override {
        DaoGraph::DAOGRAPH_ECHO("----> [MyTemplateDaemon] template input index is [%d]", index_);
    }

private:
    int index_ = 0;
};

#endif //DAOGRAPH_MYTEMPLATEDAEMON_H
