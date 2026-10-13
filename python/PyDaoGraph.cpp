/***************************

@File: PyDaoGraph.cpp
@Time: 2025/1/30 21:43
@Desc:
***************************/

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/functional.h>

#include "DaoGraph.h"
#include "wrapper/PyWrapperInclude.h"

using namespace DaoGraph;
namespace py = pybind11;

namespace {
void retainOwnedJsonNodes(GPipeline& pipeline, const py::list& candidates) {
     py::list ownedNodes;
     for (const auto& node : candidates) {
          if (pipeline.__ownsGElement_4py(node.cast<GElementPtr>())) {
               ownedNodes.append(node);
          }
     }
     if (py::len(ownedNodes) != 0) {
          py::detail::keep_alive_impl(py::cast(&pipeline, py::return_value_policy::reference), ownedNodes);
     }
}

CStatus buildJsonWithNodeClasses(GPipeline& pipeline, const std::string& source,
                                         const py::dict& nodeClasses, bool fromFile) {
     GPipeline::GNodeClassMap constructors;
     py::list nodes;
     for (const auto& entry : nodeClasses) {
          const std::string name = py::cast<std::string>(entry.first);
          py::object nodeClass = py::reinterpret_borrow<py::object>(entry.second);
          if (!PyCallable_Check(nodeClass.ptr())) {
               throw py::type_error("nodeClasses['" + name + "'] must be a class or callable");
          }
          constructors.emplace(name, [nodeClass, nodes, name]() mutable -> GElementPtr {
               py::gil_scoped_acquire acquire;
               py::object node = nodeClass();
               if (!py::isinstance<GElement>(node)) {
                    throw py::type_error("nodeClasses['" + name + "'] must construct a GElement");
               }
               // Retain the actual Python instances, not just their classes, so
               // trampoline overrides survive after the constructor returns.
               nodes.append(node);
               return node.cast<GElementPtr>();
          });
     }
     CStatus result;
     try {
          py::gil_scoped_release release;
          result = fromFile ? pipeline.buildFromJson(source, constructors)
                            : pipeline.buildFromJsonStr(source, constructors);
     } catch (...) {
          retainOwnedJsonNodes(pipeline, nodes);
          throw;
     }
     // Retain accepted nodes even on a build error, but never foreign nodes
     // rejected by C++: their owner's deletion must not leave a stale wrapper.
     retainOwnedJsonNodes(pipeline, nodes);
     return result;
}
}  // namespace

PYBIND11_MODULE(_core, cg) {
    cg.doc() = "DaoGraph with python api";
    py::class_<UThreadPoolConfig>(cg, "UThreadPoolConfig")
        .def(py::init<>())
        .def_readwrite("default_thread_size", &UThreadPoolConfig::default_thread_size_)
        .def_readwrite("secondary_thread_size", &UThreadPoolConfig::secondary_thread_size_)
        .def_readwrite("max_thread_size", &UThreadPoolConfig::max_thread_size_)
        .def_readwrite("max_task_steal_range", &UThreadPoolConfig::max_task_steal_range_)
        .def_readwrite("max_local_batch_size", &UThreadPoolConfig::max_local_batch_size_)
        .def_readwrite("max_pool_batch_size", &UThreadPoolConfig::max_pool_batch_size_)
        .def_readwrite("max_steal_batch_size", &UThreadPoolConfig::max_steal_batch_size_)
        .def_readwrite("primary_thread_busy_epoch", &UThreadPoolConfig::primary_thread_busy_epoch_)
        .def_readwrite("primary_thread_empty_interval", &UThreadPoolConfig::primary_thread_empty_interval_)
        .def_readwrite("secondary_thread_ttl", &UThreadPoolConfig::secondary_thread_ttl_)
        .def_readwrite("monitor_span", &UThreadPoolConfig::monitor_span_)
        .def_readwrite("queue_empty_interval", &UThreadPoolConfig::queue_emtpy_interval_)
        .def_readwrite("primary_thread_policy", &UThreadPoolConfig::primary_thread_policy_)
        .def_readwrite("secondary_thread_policy", &UThreadPoolConfig::secondary_thread_policy_)
        .def_readwrite("primary_thread_priority", &UThreadPoolConfig::primary_thread_priority_)
        .def_readwrite("secondary_thread_priority", &UThreadPoolConfig::secondary_thread_priority_)
        .def_readwrite("bind_cpu_enable", &UThreadPoolConfig::bind_cpu_enable_)
        .def_readwrite("batch_task_enable", &UThreadPoolConfig::batch_task_enable_)
        .def_readwrite("monitor_enable", &UThreadPoolConfig::monitor_enable_);

    py::class_<GElementRelation>(cg, "GElementRelation")
        .def(py::init<>())
        .def_readonly("predecessors", &GElementRelation::predecessors_)
        .def_readonly("successors", &GElementRelation::successors_)
        .def_readonly("children", &GElementRelation::children_)
        .def_readonly("belong", &GElementRelation::belong_);

    py::class_<UThreadPool>(cg, "UThreadPool")
        .def(py::init<CBool, const UThreadPoolConfig&>(),
             py::arg("autoInit") = true,
             py::arg("config") = UThreadPoolConfig{})
        .def("setConfig", &UThreadPool::setConfig,
              py::arg("config"),
              py::keep_alive<1, 2>())
        .def("getConfig", &UThreadPool::getConfig)
        .def("init", &UThreadPool::init)
        .def("destroy", &UThreadPool::destroy)
        .def("isInit", &UThreadPool::isInit);

    py::register_exception<CException>(cg, "CException");

    py::enum_<GEngineType>(cg, "GEngineType")
        .value("DYNAMIC", GEngineType::DYNAMIC)
        .value("TOPO", GEngineType::TOPO)
        .value("STATIC", GEngineType::STATIC)
        .export_values();

    py::enum_<GElementTimeoutStrategy>(cg, "GElementTimeoutStrategy")
        .value("AS_ERROR", GElementTimeoutStrategy::AS_ERROR)
        .value("HOLD_BY_PIPELINE", GElementTimeoutStrategy::HOLD_BY_PIPELINE)
        .value("NO_HOLD", GElementTimeoutStrategy::NO_HOLD)
        .export_values();

    py::enum_<GMultiConditionType>(cg, "GMultiConditionType")
        .value("SERIAL", GMultiConditionType::SERIAL)
        .value("PARALLEL", GMultiConditionType::PARALLEL)
        .export_values();

    py::enum_<GEventType>(cg, "GEventType")
        .value("SYNC", GEventType::SYNC)
        .value("ASYNC", GEventType::ASYNC)
        .export_values();

    py::enum_<GEventAsyncStrategy>(cg, "GEventAsyncStrategy")
        .value("PIPELINE_RUN_FINISH", GEventAsyncStrategy::PIPELINE_RUN_FINISH)
        .value("PIPELINE_DESTROY", GEventAsyncStrategy::PIPELINE_DESTROY)
        .value("NO_WAIT", GEventAsyncStrategy::NO_WAIT)
        .export_values();

    py::enum_<CFunctionType>(cg, "CFunctionType")
        .value("INIT", CFunctionType::INIT)
        .value("RUN", CFunctionType::RUN)
        .value("DESTROY", CFunctionType::DESTROY)
        .export_values();

    py::enum_<GElementState>(cg, "GElementState")
        .value("NORMAL", GElementState::NORMAL)
        .value("CANCEL", GElementState::CANCEL)
        .value("SUSPEND", GElementState::SUSPEND)
        .value("TIMEOUT", GElementState::TIMEOUT)
        .export_values();
    cg.attr("GPipelineState") = cg.attr("GElementState");

    py::enum_<std::launch>(cg, "StdLaunchPolicy")
        .value("ASYNC", std::launch::async)
        .value("DEFERRED", std::launch::deferred)
        .export_values();

    py::class_<std::shared_future<void> >(cg, "StdSharedFutureVoid")
        .def("wait", [] (std::shared_future<void>& fut) {
            fut.wait();
        }, py::call_guard<py::gil_scoped_release>());

    py::class_<std::future<CStatus> >(cg, "StdFutureCStatus")
        .def("get", [] (std::future<CStatus>& fut) {
            return fut.get();
        }, py::call_guard<py::gil_scoped_release>())
        .def("wait", [] (std::future<CStatus>& fut) {
            fut.wait();
        }, py::call_guard<py::gil_scoped_release>());

    py::class_<CStatus>(cg, "CStatus")
        .def(py::init<>())
        .def(py::init<int, const std::string&>(),
             py::arg("errorCode"),
             py::arg("errorInfo"))
        .def("__iadd__", &CStatus::operator+=,
             py::return_value_policy::reference)
        .def("getCode", &CStatus::getCode)
        .def("getInfo", &CStatus::getInfo)
        .def("reset", &CStatus::reset)
        .def("isOK", &CStatus::isOK)
        .def("isErr", &CStatus::isErr)
        .def("isCrash", &CStatus::isCrash);

    py::class_<GAspect, PywGAspect, std::unique_ptr<GAspect, py::nodelete> >(cg, "GAspect")
        .def(py::init<>())
        .def("getName", &GAspect::__getName_4py)
        .PYDAOGRAPH_DEF_GPARAM_PYBIND11_FUNCTIONS(GAspect)
        .PYDAOGRAPH_DEF_GEVENT_PYBIND11_FUNCTIONS(GAspect);

    py::class_<GDaemon, PywGDaemon, std::unique_ptr<GDaemon, py::nodelete> >(cg, "GDaemon")
        .def(py::init<>())
        .def("getInterval", &GDaemon::__getInterval_4py)
        .PYDAOGRAPH_DEF_GPARAM_PYBIND11_FUNCTIONS(GDaemon)
        .PYDAOGRAPH_DEF_GEVENT_PYBIND11_FUNCTIONS(GDaemon);

    py::class_<GEvent, PywGEvent, std::unique_ptr<GEvent, py::nodelete> >(cg, "GEvent")
        .def(py::init<>())
        .PYDAOGRAPH_DEF_GPARAM_PYBIND11_FUNCTIONS(GEvent);

    py::class_<GStage, PywGStage, std::unique_ptr<GStage, py::nodelete> >(cg, "GStage")
        .def(py::init<>())
        .PYDAOGRAPH_DEF_GPARAM_PYBIND11_FUNCTIONS(GStage);

    py::class_<GParam, PywGParam, std::unique_ptr<GParam, py::nodelete> >(cg, "GParam")
        .def(py::init<>())
        .def("lock", &GParam::lock,
             py::call_guard<py::gil_scoped_release>())
        .def("unlock", &GParam::unlock,
             py::call_guard<py::gil_scoped_release>())
        .def("tryLock", &GParam::tryLock,
             py::call_guard<py::gil_scoped_release>());

    py::class_<GPassedParam, PywGPassedParam, std::unique_ptr<GPassedParam, py::nodelete> >(cg, "GPassedParam")
        .def(py::init<>());
    cg.attr("GElementParam") = cg.attr("GPassedParam");
    cg.attr("GDaemonParam") = cg.attr("GPassedParam");
    cg.attr("GStageParam") = cg.attr("GPassedParam");
    cg.attr("GEventParam") = cg.attr("GPassedParam");

    py::class_<GPipeline, std::unique_ptr<GPipeline, PywGPipelineDeleter> >(cg, "GPipeline")
        .def(py::init<>([]() { return GPipelineFactory::create(); }))
        .def("init", &GPipeline::init)
        .PYDAOGRAPH_DEF_GPARAM_PYBIND11_FUNCTIONS(GPipeline)
        .def("setUniqueThreadPoolConfig", &GPipeline::setUniqueThreadPoolConfig,
             py::arg("config"))
        .def("setSharedThreadPool", &GPipeline::setSharedThreadPool,
             py::arg("ptr"),
             py::call_guard<py::gil_scoped_release>(),
             py::keep_alive<1, 2>())
     .def("buildFromJson", static_cast<CStatus (GPipeline::*)(const std::string&)>(&GPipeline::buildFromJson),
             py::arg("jsonPath"),
             py::call_guard<py::gil_scoped_release>())
        .def("buildFromJson", [](GPipeline& self, const std::string& jsonPath,
                                 const py::dict& nodeClasses) {
                 return buildJsonWithNodeClasses(self, jsonPath, nodeClasses, true);
             }, py::arg("jsonPath"), py::arg("nodeClasses"),
             "Build from a JSON file and a dict mapping node names to classes or zero-argument factories.")
     .def("buildFromJsonStr", static_cast<CStatus (GPipeline::*)(const std::string&)>(&GPipeline::buildFromJsonStr),
             py::arg("jsonStr"),
             py::call_guard<py::gil_scoped_release>())
        .def("buildFromJsonStr", [](GPipeline& self, const std::string& jsonStr,
                                    const py::dict& nodeClasses) {
                 return buildJsonWithNodeClasses(self, jsonStr, nodeClasses, false);
             }, py::arg("jsonStr"), py::arg("nodeClasses"),
             "Build from JSON text and a dict mapping node names to classes or zero-argument factories.")
        .def("collectNodeChildren",
             [](GPipeline& self, const std::string& jsonPath)
             -> std::pair<CStatus, std::unordered_map<std::string, std::vector<std::string>>> {
                 std::unordered_map<std::string, std::vector<std::string>> childrenMap;
                 CStatus s = self.collectNodeChildren(jsonPath, childrenMap);
                 return {s, childrenMap};
             },
             py::arg("jsonPath"),
             py::call_guard<py::gil_scoped_release>(),
             "Parse a graph JSON file, return (CStatus, dict[str, list[str]]) "
             "mapping each node name to its direct child node names.")
        .def("collectNodeChildrenFromStr",
             [](GPipeline& self, const std::string& jsonStr)
             -> std::pair<CStatus, std::unordered_map<std::string, std::vector<std::string>>> {
                 std::unordered_map<std::string, std::vector<std::string>> childrenMap;
                 CStatus s = self.collectNodeChildrenFromStr(jsonStr, childrenMap);
                 return {s, childrenMap};
             },
             py::arg("jsonStr"),
             py::call_guard<py::gil_scoped_release>(),
             "Parse a graph JSON string, return (CStatus, dict[str, list[str]]) "
             "mapping each node name to its direct child node names.")
        .def("setGEngineType", &GPipeline::setGEngineType,
             py::arg("type"))
        .def("run", &GPipeline::run,
             py::call_guard<py::gil_scoped_release>())
        .def("process", &GPipeline::process,
             py::arg("runTimes") = 1,
             py::call_guard<py::gil_scoped_release>())
        .def("destroy", &GPipeline::destroy)
        .def("addGEvent", &GPipeline::__addGEvent_4py,
             py::arg("event"),
             py::arg("key"),
             py::keep_alive<1, 2>())
        .def("addGDaemon", &GPipeline::__addGDaemon_4py,
             py::arg("daemon"),
             py::arg("ms"),
             py::keep_alive<1, 2>())
        .def("addGStage", &GPipeline::__addGStage_4py,
             py::arg("stage"),
             py::arg("key"),
             py::arg("threshold"),
             py::keep_alive<1, 2>())
        .def("asyncRun", &GPipeline::asyncRun,
             py::arg("policy") = std::launch::async,
             py::call_guard<py::gil_scoped_release>())
        .def("asyncProcess", &GPipeline::asyncProcess,
             py::arg("runTimes") = DAOGRAPH_DEFAULT_LOOP_TIMES,
             py::arg("policy") = std::launch::async,
             py::call_guard<py::gil_scoped_release>())
        .def("cancel", &GPipeline::cancel,
            py::call_guard<py::gil_scoped_release>())
        .def("suspend", &GPipeline::suspend,
             py::call_guard<py::gil_scoped_release>())
        .def("disrupt", &GPipeline::disrupt,
             py::call_guard<py::gil_scoped_release>())
        .def("resume", &GPipeline::resume,
             py::call_guard<py::gil_scoped_release>())
        .def("proceed", &GPipeline::proceed,
             py::call_guard<py::gil_scoped_release>())
        .def("perf", &GPipeline::__perf_4py,
             py::call_guard<py::gil_scoped_release>())
        .def("dump", &GPipeline::__dump_4py)
        .def("trim", &GPipeline::trim)
        .def("makeSerial", &GPipeline::makeSerial)
        .def("getMaxPara", &GPipeline::getMaxPara)
        .def("getCurState", &GPipeline::getCurState)
        .def("checkSeparate", &GPipeline::checkSeparate,
             py::arg("fst"),
             py::arg("snd"))
        .def("registerGElement", &GPipeline::__registerGElement_4py,
             py::arg("element"),
             py::arg("depends") = GElementPtrSet{},
             py::arg("name") = DAOGRAPH_EMPTY,
             py::arg("loop") = DAOGRAPH_DEFAULT_LOOP_TIMES,
             py::keep_alive<1, 2>());

    py::class_<GPipelineManager>(cg, "GPipelineManager")
        .def(py::init<>())
        .def("init", &GPipelineManager::init)
        .def("run", &GPipelineManager::run,
             py::call_guard<py::gil_scoped_release>())
        .def("destroy", &GPipelineManager::destroy)
        .def("add", &GPipelineManager::add,
             py::arg("ptr"),
             py::keep_alive<1, 2>())
        .def("clear", &GPipelineManager::clear)
        .def("find", &GPipelineManager::find,
             py::arg("ptr"))
        .def("remove", &GPipelineManager::remove,
             py::arg("ptr"))
        .def("getSize", &GPipelineManager::getSize)
        .def("fetch", &GPipelineManager::fetch,
             py::call_guard<py::gil_scoped_release>())
        .def("release", &GPipelineManager::release,
             py::arg("ptr"),
             py::call_guard<py::gil_scoped_release>());

    py::class_<GElement, PywGElement, std::unique_ptr<GElement, py::nodelete> > gElementPy(cg, "GElement");
    gElementPy
        .def(py::init<>())
        .def("run", &GElement::run,
             py::call_guard<py::gil_scoped_release>())
        .def("__str__", &GElement::__str_4py)
        .def_static("registerClass", [](const std::string& name, py::function creator) {
            // Store the creator function, prevent GC
            creator.inc_ref();
            return GElement::registerClass(name, [creator]() -> GElement* {
                py::gil_scoped_acquire acquire;
                py::object obj = creator();
                GElement* ptr = obj.cast<GElement*>();
                // prevent python garbage collection of the returned object
                obj.inc_ref();
                return ptr;
            });
        }, py::arg("name"), py::arg("creator"))
        .def_static("create", &GElement::create,
             py::arg("name"),
             py::return_value_policy::reference)
        .def_static("registeredClasses", &GElement::registeredClasses)
        .PYDAOGRAPH_DEF_GPARAM_PYBIND11_FUNCTIONS(GElement)
        .PYDAOGRAPH_DEF_GEVENT_PYBIND11_FUNCTIONS(GElement)
        .def("enterStage", &GElement::__enterStage_4py,
             py::arg("key"),
             py::call_guard<py::gil_scoped_release>())
        .def("getName", &GElement::getName)
        .def("getSession", &GElement::getSession)
        .def("getRelation", &GElement::getRelation)
        .def("getLoop", &GElement::getLoop)
        .def("getCurState", &GElement::getCurState)
        .def("setLoop", &GElement::setLoop,
             py::arg("loop"))
        .def("setName", &GElement::setName,
             py::arg("name"))
        .def("setLevel", &GElement::setLevel,
             py::arg("level"))
        .def("setVisible", &GElement::setVisible,
             py::arg("visible"))
        .def("setMacro", &GElement::setMacro,
             py::arg("macro"))
        .def("setWaitFunc", &GElement::setWaitFunc,
             py::arg("func"))
        .def("setTimeout", &GElement::setTimeout,
             py::arg("timeout"),
             py::arg("strategy") = GElementTimeoutStrategy::AS_ERROR)
        .def("isTimeout", &GElement::__isTimeout_4py)
        .def("isGGroup", &GElement::isGGroup)
        .def("isGAdaptor", &GElement::isGAdaptor)
        .def("isGNode", &GElement::isGNode)
        .def("addGAspect", &GElement::__addGAspect_4py,
             py::arg("aspect"),
             py::keep_alive<1, 2>())
        .def("addDependGElements", &GElement::addDependGElements,
             py::arg("elements"))
        .def("removeDepend", &GElement::removeDepend,
             py::arg("element"))
        .def("clone", &GElement::clone,
             py::return_value_policy::take_ownership);

    py::class_<GNode, PywGNode, GElement, std::unique_ptr<GNode, py::nodelete> >(cg, "GNode")
        .def(py::init<const std::string&, int>(),
             py::arg("name"),
             py::arg("loop") = DAOGRAPH_DEFAULT_LOOP_TIMES)
        .def(py::init<const GElementPtrSet&, const std::string&, int>(),
             py::arg("depends") = GElementPtrSet{},
             py::arg("name") = DAOGRAPH_EMPTY,
             py::arg("loop") = DAOGRAPH_DEFAULT_LOOP_TIMES)
        .def("run", &GNode::run,
             py::call_guard<py::gil_scoped_release>());

    py::class_<GFence, PywGFence, GElement, std::unique_ptr<GFence, py::nodelete> >(cg, "GFence")
        .def(py::init<>())
        .PYDAOGRAPH_DEF_GPARAM_PYBIND11_FUNCTIONS(GFence)
        .def("waitGElement", &GFence::waitGElement,
             py::arg("element"))
        .def("waitGElements", &GFence::waitGElements,
             py::arg("elements"))
        .def("clear", &GFence::clear);

    py::class_<GFunction, PywGFunction, GElement, std::unique_ptr<GFunction, py::nodelete> >(cg, "GFunction")
        .def(py::init<>())
        .PYDAOGRAPH_DEF_GPARAM_PYBIND11_FUNCTIONS(GFunction)
        .PYDAOGRAPH_DEF_GEVENT_PYBIND11_FUNCTIONS(GFunction)
        .def("setFunction", &GFunction::setFunction,
             py::arg("type"),
             py::arg("func"));

    PYDAOGRAPH_DECLARE_GGROUP_PYBIND11_FUNCTIONS(GCluster);
    PYDAOGRAPH_DECLARE_GGROUP_PYBIND11_FUNCTIONS(GClusterInterface);

    PYDAOGRAPH_DECLARE_GGROUP_PYBIND11_FUNCTIONS(GRegion);
    PYDAOGRAPH_DECLARE_GGROUP_PYBIND11_FUNCTIONS(GRegionInterface);

    py::class_<GCondition, PywGCondition, GElement, std::unique_ptr<GCondition, py::nodelete> >(cg, "GCondition")
        .def(py::init<const GElementPtrArr&>(),
             py::arg("elements") = GElementPtrArr{},
             py::keep_alive<1, 2>())
        .def("addGElements", &PywGCondition::__addGElements_4py,
             py::arg("elements"),
             py::keep_alive<1, 2>())
        .def("choose", [](GCondition& self) {
             return static_cast<PywGCondition&>(self).choose();
         });
    PYDAOGRAPH_DECLARE_GGROUP_PYBIND11_FUNCTIONS(GConditionInterface);

    PYDAOGRAPH_DECLARE_GGROUP_PYBIND11_FUNCTIONS(GSerialMultiCondition);
    PYDAOGRAPH_DECLARE_GGROUP_PYBIND11_FUNCTIONS(GSerialMultiConditionInterface);

    PYDAOGRAPH_DECLARE_GGROUP_PYBIND11_FUNCTIONS(GParallelMultiCondition);
    PYDAOGRAPH_DECLARE_GGROUP_PYBIND11_FUNCTIONS(GParallelMultiConditionInterface);

    PYDAOGRAPH_DECLARE_GGROUP_PYBIND11_FUNCTIONS(GMutable);
    PYDAOGRAPH_DECLARE_GGROUP_PYBIND11_FUNCTIONS(GMutableInterface);
}
