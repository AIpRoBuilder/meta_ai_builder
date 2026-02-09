/***************************

@File: MyTemplateAspect.h
@Time: 2022/12/23 22:44
@Desc: 
***************************/

#ifndef DAOGRAPH_MYTEMPLATEASPECT_H
#define DAOGRAPH_MYTEMPLATEASPECT_H

#include "DaoGraph.h"

template<class ...Args>
class MyTemplateAspect : public DaoGraph::GTemplateAspect<Args...> {
public:
    explicit MyTemplateAspect(int age, double score) {
        age_ = age;
        score_ = score;
    }

    CStatus beginInit() override {
        DaoGraph::DAOGRAPH_ECHO("----> [MyTemplateAspect] [%s] input age is [%d], score is [%lf]",
                            this->getName().c_str(), age_, score_);
        return CStatus();
    }

private:
    int age_;
    double score_;
};

#endif //DAOGRAPH_MYTEMPLATEASPECT_H
