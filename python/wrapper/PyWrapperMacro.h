/***************************

@File: PyWrapperDefine.h
@Time: 2025/4/6 14:48
@Desc: 
***************************/

#ifndef DAOGRAPH_PYWRAPPERDEFINE_H
#define DAOGRAPH_PYWRAPPERDEFINE_H

#include <pybind11/pybind11.h>
#include <typeinfo>

#include "DaoGraph.h"

#define PYDAOGRAPH_DECLARE_GELEMENT_COMMON_FUNCTIONS(PCGE)    \
    CBool isHold() override {                               \
        PYBIND11_OVERLOAD(CBool, PCGE, isHold);             \
    }                                                       \
                                                            \
    CBool isMatch() override {                              \
        PYBIND11_OVERLOAD(CBool, PCGE, isMatch);            \
    }                                                       \
                                                            \
    CStatus prepareRun() override {                         \
        PYBIND11_OVERLOAD(CStatus, PCGE, prepareRun);       \
    }                                                       \
                                                            \
    CStatus checkRunResult() override {                     \
        PYBIND11_OVERLOAD(CStatus, PCGE, checkRunResult);   \
    }                                                       \
                                                            \
    CStatus onInput(const std::string& input) override {    \
        PYBIND11_OVERLOAD(CStatus, PCGE, onInput, input);   \
    }                                                       \
                                                            \
    CStatus onInputs(const std::vector<std::string>& inputs) override {   \
        PYBIND11_OVERLOAD(CStatus, PCGE, onInputs, inputs); \
    }                                                       \
                                                            \
    std::string getClassName() const override {             \
        const auto& n = this->getName();                    \
        if (!n.empty()) {                                   \
            return n;                                       \
        }                                                   \
        return std::string(typeid(*this).name());           \
    }                                                       \
                                                            \
    DaoGraph::GElement* clone() const override {              \
        PYBIND11_OVERLOAD(DaoGraph::GElement*, PCGE, clone);  \
    }                                                       \
                                                            \


#define PYDAOGRAPH_DECLARE_GGROUP_INTERFACE_CLASS(PCGG, PVFunc)                             \
class PCGG##Interface : public PCGG {                                                     \
public:                                                                                   \
    explicit PCGG##Interface(const DaoGraph::GElementPtrArr& elements = DaoGraph::GElementPtrArr{}) : PCGG(elements) {}; \
    ~PCGG##Interface() override = default;                                                \
protected:                                                                                \
    PYDAOGRAPH_DECLARE_GELEMENT_COMMON_FUNCTIONS(PCGG##Interface)                           \
    PVFunc                                                                                \
};                                                                                        \
                                                                                          \


#define PYDAOGRAPH_DECLARE_GGROUP_PYBIND11_FUNCTIONS(PCGG)                                  \
    py::class_<Pyw##PCGG, GElement, std::unique_ptr<Pyw##PCGG, py::nodelete> >(cg, #PCGG) \
        .def(py::init<const GElementPtrArr&>(),                                           \
            py::arg("elements") = GElementPtrArr{},                                       \
            py::keep_alive<1, 2>())                                                       \
        .def("addGElements", &Pyw##PCGG::__addGElements_4py,                              \
            py::arg("elements"),                                                          \
            py::keep_alive<1, 2>())                                                       \
                                                                                          \


#define PYDAOGRAPH_DEF_GPARAM_PYBIND11_FUNCTIONS(PCGO)                                      \
    def("createGParam", &PCGO::__createGParam_4py,                                        \
         py::arg("param"),                                                                \
         py::arg("key"),                                                                  \
         py::keep_alive<1, 2>(),                                                          \
         py::call_guard<py::gil_scoped_release>())                                        \
    .def("getGParam", &PCGO::__getGParam_4py,                                             \
         py::arg("key"))                                                                  \
    .def("getGParamWithNoEmpty", &PCGO::__getGParamWithNoEmpty_4py,                       \
         py::arg("key"))                                                                  \
    .def("removeGParam", &PCGO::__removeGParam_4py,                                       \
         py::arg("key"),                                                                  \
         py::call_guard<py::gil_scoped_release>())                                        \
    .def("hasGParam", &PCGO::__hasGParam_4py,                                             \
         py::arg("key"),                                                                  \
         py::call_guard<py::gil_scoped_release>())                                        \
                                                                                          \


#define PYDAOGRAPH_DEF_GEVENT_PYBIND11_FUNCTIONS(PCGO)                                      \
    def("notify", &PCGO::__notify_4py,                                                    \
        py::arg("key"),                                                                   \
        py::arg("type"),                                                                  \
        py::arg("strategy") = GEventAsyncStrategy::PIPELINE_RUN_FINISH,                   \
        py::call_guard<py::gil_scoped_release>())                                         \
    .def("asyncNotify", &PCGO::__asyncNotify_4py,                                         \
        py::arg("key"),                                                                   \
        py::arg("strategy") = GEventAsyncStrategy::PIPELINE_RUN_FINISH,                   \
        py::call_guard<py::gil_scoped_release>())                                         \
                                                                                          \

#endif //DAOGRAPH_PYWRAPPERDEFINE_H
