/***************************

@File: Graphic.cpp
@Time: 2021/6/2 10:15 下午
@Desc: 
***************************/

#include "GPipeline.h"
#include "_GPerf/GPerfInclude.h"
#include "_GStroage/GStorageInclude.h"
#include "../GraphElement/_GOptimizer/GOptimizerInclude.h"
#include "../../nlohmann/json.hpp"
#include <fstream>
#include <unordered_set>

DAOGRAPH_NAMESPACE_BEGIN

namespace {
// Forward declaration — defined later in this anonymous namespace.
CStatus collectNodeChildrenFromGraphJson(
        const nlohmann::json& graphJson,
        std::unordered_map<std::string, std::vector<std::string>>& childrenMap);

CStatus buildPipelineFromGraphJson(GPipeline* pipeline, const nlohmann::json& graphJson) {
    if (!graphJson.contains("nodes") || !graphJson["nodes"].is_array()) {
        return CStatus("JSON must contain array 'nodes'");
    }

    std::unordered_map<std::string, GElementPtr> created;

    // 1) 创建全部节点实例
    for (const auto& node : graphJson["nodes"]) {
        const std::string name      = node.value("name", "");
        const std::string type      = node.value("type", "");
        const CSize loop            = node.value("loop", (CSize)1);

        if (name.empty() || type.empty()) {
            return CStatus("node missing name or type");
        }

        if (created.count(name)) {
            return CStatus("duplicate node name: " + name);
        }

        GElementPtr element = GElement::create(type);
        if (!element) {
            return CStatus("factory create failed: " + type);
        }

        created[name] = element;
        created[name]->setName(name)->setLoop(loop);
    }

    // 2) 使用 collectNodeChildrenFromGraphJson 构建 parent -> children 映射
    //    childrenMap[P] = { C | C 在其 depends 中列出了 P }
    std::unordered_map<std::string, std::vector<std::string>> childrenMap;
    CStatus childStatus = collectNodeChildrenFromGraphJson(graphJson, childrenMap);
    if (childStatus.isErr()) {
        return childStatus;
    }

    // 3) 注册到 pipeline，根据 meta_type 分别处理
    //    对所有节点，"depends" 统一表示上游 pipeline 依赖。
    //    对 Group 类节点（如 GCondition），其内部子节点由 childrenMap 推导。

    // Pre-compute the set of nodes that are children of a GCondition group, and
    // a reverse map (child name -> owning group name) so that external `depends`
    // references to branch nodes can be silently promoted to the group itself.
    // Branch nodes must NOT be registered to the pipeline independently.
    std::unordered_set<std::string> conditionChildSet;
    std::unordered_map<std::string, std::string> childToGroupMap;   // childName -> groupName
    for (const auto& node : graphJson["nodes"]) {
        const std::string meta_type = node.value("meta_type", "GNode");
        if (meta_type == "GCondition") {
            const std::string groupName = node.value("name", "");
            for (const auto& childName : childrenMap[groupName]) {
                conditionChildSet.insert(childName);
                childToGroupMap[childName] = groupName;
            }
        }
    }

    for (const auto& node : graphJson["nodes"]) {
        const std::string name      = node.value("name", "");
        const std::string meta_type = node.value("meta_type", "GNode");
        GElementPtr element = created[name];

        // Build upstream dependency set from "depends" (uniform for all meta_types).
        // If a listed dependency is a branch node inside a GCondition group, promote
        // the reference to the owning group — direct cross-group edges are illegal.
        GElementPtrSet depends;
        if (node.contains("depends")) {
            for (const auto& depName : node["depends"]) {
                std::string dep = depName.get<std::string>();
                // Promote branch-child references to their owning group
                auto groupIt = childToGroupMap.find(dep);
                if (groupIt != childToGroupMap.end()) {
                    dep = groupIt->second;
                }
                auto it = created.find(dep);
                if (it == created.end()) {
                    return CStatus("depend not found: " + dep);
                }
                depends.insert(it->second);
            }
        }

        if (meta_type == "GCondition") {
            /**
             * GCondition: child branch elements are derived from childrenMap, i.e. every node
             * that lists this group in its own "depends" array becomes a branch of the group.
             * This mirrors the C++ API: createGGroup<MyCondition>({nodeB0, nodeB1, nodeB2}).
             *
             * Reference: T06-Condition.cpp
             *   b_condition = pipeline->createGGroup<MyCondition>({nodeB0, nodeB1, nodeB2});
             *   pipeline->registerGElement<MyCondition>(&b_condition, {a}, "conditionB", 1);
             */
            auto* group = dynamic_cast<GGroupPtr>(element);
            if (!group) {
                return CStatus("meta_type GCondition but element is not a GGroup: " + name);
            }

            // Collect child branch elements from childrenMap
            GElementPtrArr children;
            for (const auto& childName : childrenMap[name]) {
                auto it = created.find(childName);
                if (it == created.end()) {
                    return CStatus("GCondition child not found in created map: " + childName);
                }
                children.push_back(it->second);
            }

            CStatus status = group->__addGElements_4py(children);
            if (status.isErr()) {
                return status;
            }

            status = pipeline->registerGElement(group, depends, name, element->getLoop());
            if (status.isErr()) {
                return status;
            }
        } else {
            // Skip nodes that are internal branches of a GCondition group —
            // they are registered inside the group, not directly to the pipeline.
            if (conditionChildSet.count(name)) {
                continue;
            }
            // Default: GNode — register with upstream dependencies
            CStatus status = pipeline->registerGElement(element, depends, name, element->getLoop());
            if (status.isErr()) {
                return status;
            }
        }
    }

    return CStatus();
}

/**
 * Parse graphJson and collect each node's direct child nodes.
 * A node C is a "child" of node P when C lists P in its "depends" array.
 *
 * @param graphJson   The parsed JSON object (must contain a "nodes" array).
 * @param childrenMap Output map: node name -> list of child node names.
 * @return CStatus
 */
CStatus collectNodeChildrenFromGraphJson(
        const nlohmann::json& graphJson,
        std::unordered_map<std::string, std::vector<std::string>>& childrenMap) {
    if (!graphJson.contains("nodes") || !graphJson["nodes"].is_array()) {
        return CStatus("JSON must contain array 'nodes'");
    }

    // Initialise an entry for every declared node (even leaf nodes with no children).
    for (const auto& node : graphJson["nodes"]) {
        const std::string name = node.value("name", "");
        if (name.empty()) {
            return CStatus("node missing name");
        }
        childrenMap.emplace(name, std::vector<std::string>{});
    }

    // For each node, register it as a child of every node it depends on.
    for (const auto& node : graphJson["nodes"]) {
        const std::string childName = node.value("name", "");
        if (!node.contains("depends")) {
            continue;
        }
        for (const auto& depEntry : node["depends"]) {
            const std::string parentName = depEntry.get<std::string>();
            auto it = childrenMap.find(parentName);
            if (it == childrenMap.end()) {
                return CStatus("depends references unknown node: " + parentName);
            }
            it->second.push_back(childName);
        }
    }

    return CStatus();
}
}  // namespace

GPipeline::GPipeline() {
    session_ = URandom<>::generateSession(DAOGRAPH_STR_PIPELINE);
    element_manager_ = DAOGRAPH_SAFE_MALLOC_COBJECT(GElementManager)
    param_manager_ = DAOGRAPH_SAFE_MALLOC_COBJECT(GParamManager)
    daemon_manager_ = DAOGRAPH_SAFE_MALLOC_COBJECT(GDaemonManager)
    event_manager_ = DAOGRAPH_SAFE_MALLOC_COBJECT(GEventManager)
    stage_manager_ = DAOGRAPH_SAFE_MALLOC_COBJECT(GStageManager)
}


GPipeline::~GPipeline() {
    // 结束的时候，会自动释放所有的element信息
    DAOGRAPH_DELETE_PTR(daemon_manager_)
    DAOGRAPH_DELETE_PTR(element_manager_)
    DAOGRAPH_DELETE_PTR(param_manager_)
    DAOGRAPH_DELETE_PTR(event_manager_)
    DAOGRAPH_DELETE_PTR(stage_manager_)
}


CStatus GPipeline::init() {
    DAOGRAPH_FUNCTION_BEGIN
    DAOGRAPH_ASSERT_INIT(false)    // 必须是非初始化的状态下，才可以初始化。反之同理
    DAOGRAPH_ASSERT_NOT_NULL(element_manager_, param_manager_, daemon_manager_, event_manager_, stage_manager_)

    status += initEnv();
    DAOGRAPH_FUNCTION_CHECK_STATUS

    status += param_manager_->init();
    status += event_manager_->init();
    status += element_manager_->init();
    status += stage_manager_->init();
    status += daemon_manager_->init();    // daemon的初始化，需要晚于所有element的初始化
    DAOGRAPH_FUNCTION_CHECK_STATUS

    is_init_ = true;
    DAOGRAPH_FUNCTION_END
}


CStatus GPipeline::run() {
    DAOGRAPH_FUNCTION_BEGIN
    DAOGRAPH_ASSERT_INIT(true)

    /**
     * 1. 将所有 GElement 的状态设定为 NORMAL
     * 2. 将所有的 GParam 设置为初始值
     * 3. 执行dag逻辑
     * 4. 将所有 GElement 的状态恢复为 NORMAL
     * 5. 等到所有异步事件执行结束
     * 6. 将所有的 GParam 复原
     */
    status += repository_.setup();
    status += param_manager_->setup();
    DAOGRAPH_FUNCTION_CHECK_STATUS

    status = element_manager_->run();
    status += repository_.reset();    // 等异步的逻辑执行，并且获取异步执行的结果信息
    status += event_manager_->reset();    // 所有的事件（特指异步事件）执行结束
    param_manager_->resetWithStatus(status);
    DAOGRAPH_FUNCTION_END
}


CStatus GPipeline::destroy() {
    DAOGRAPH_FUNCTION_BEGIN

    DAOGRAPH_ASSERT_INIT(true)
    DAOGRAPH_ASSERT_NOT_NULL(element_manager_, param_manager_, daemon_manager_, event_manager_, stage_manager_)

    /**
     * 需要先将线程池相关内容析构，然后再做destroy。
     * 否则的话，在错误status累积的情况下，在windows平台上，会产生崩溃
     */
    status += repository_.destroy();
    status += schedule_.destroy();
    DAOGRAPH_FUNCTION_CHECK_STATUS

    status += event_manager_->destroy();
    status += daemon_manager_->destroy();
    status += element_manager_->destroy();
    status += stage_manager_->destroy();
    status += param_manager_->destroy();
    DAOGRAPH_FUNCTION_CHECK_STATUS

    is_init_ = false;
    DAOGRAPH_FUNCTION_END
}


CStatus GPipeline::process(CSize runTimes) {
    DAOGRAPH_FUNCTION_BEGIN
    status = init();
    DAOGRAPH_FUNCTION_CHECK_STATUS

    while (runTimes-- > 0
           && !status.isErr()
           && !repository_.isCancelState()) {
        /**
         * 1. 执行轮数（runTimes）没有结束
         * 2. 执行结果正常
         * 3. 没有进入取消状态
         */
        status += run();
    }

    status += destroy();
    DAOGRAPH_FUNCTION_END
}


CStatus GPipeline::registerGNode(GElementPPtr nodeRef, const GElementPtrSet &depends,
                                 const std::string &name, CSize loop) {
    DAOGRAPH_FUNCTION_BEGIN
    DAOGRAPH_ASSERT_INIT(false)
    DAOGRAPH_ASSERT_NOT_NULL(*nodeRef)

    auto node = dynamic_cast<GNodePtr>(*nodeRef);
    DAOGRAPH_RETURN_ERROR_STATUS_BY_CONDITION(nullptr == node,  "[" + (*nodeRef)->getName() + "] is not based on GNode")
    DAOGRAPH_RETURN_ERROR_STATUS_BY_CONDITION(nullptr != node->belong_, "[" + node->getName() + "] can not register to pipeline for its belong to [" + node->belong_->getName() + "]")
    DAOGRAPH_RETURN_ERROR_STATUS_BY_CONDITION(node->isRegistered(), "[" + node->getName() + "] register duplicate")

    status = innerRegister(node, depends, name, loop);
    DAOGRAPH_FUNCTION_END
}


CStatus GPipeline::registerGGroup(GElementPPtr groupRef, const GElementPtrSet &depends,
                                  const std::string &name, CSize loop) {
    DAOGRAPH_FUNCTION_BEGIN
    DAOGRAPH_ASSERT_INIT(false)
    DAOGRAPH_ASSERT_NOT_NULL(*groupRef)

    auto group = dynamic_cast<GGroupPtr>(*groupRef);
    DAOGRAPH_RETURN_ERROR_STATUS_BY_CONDITION(nullptr == group, "[" + (*groupRef)->getName() + "] is not based on GGroup")
    DAOGRAPH_RETURN_ERROR_STATUS_BY_CONDITION(nullptr != group->belong_, "[" + group->getName() + "] can not register to pipeline for its belong to [" + group->belong_->getName() + "]")
    DAOGRAPH_RETURN_ERROR_STATUS_BY_CONDITION(group->isRegistered(), "[" + group->getName() + "] register duplicate")

    status = innerRegister(group, depends, name, loop);
    DAOGRAPH_FUNCTION_END
}


CStatus GPipeline::registerGElement(GElementPtr currentElement,
                                    const GElementPtrSet& dependsSet,
                                    const std::string& name,
                                    CSize loop) {
    DAOGRAPH_FUNCTION_BEGIN
    DAOGRAPH_ASSERT_INIT(false)
    DAOGRAPH_ASSERT_NOT_NULL(currentElement)

    DAOGRAPH_RETURN_ERROR_STATUS_BY_CONDITION(currentElement->isRegistered(), "element register duplicate")
    DAOGRAPH_RETURN_ERROR_STATUS_BY_CONDITION(nullptr != currentElement->belong_, "element already has belong")

    status = innerRegister(currentElement, dependsSet, name, loop);
    DAOGRAPH_FUNCTION_END
}


std::future<CStatus> GPipeline::asyncRun(std::launch policy) {
    /**
     * 1. 确认当前pipeline已经初始化完毕
     * 2. 异步的执行 run() 方法，并且返回执行结果的 future 信息
     */
    DAOGRAPH_ASSERT_INIT_THROW_ERROR(true)

    return std::async(policy, [this] {
        return run();
    });
}

CStatus GPipeline::buildFromJson(const std::string& jsonPath) {
    // Read and parse the JSON file
    std::ifstream jsonFile(jsonPath);
    if (!jsonFile.is_open()) {
        std::cerr << "Failed to open JSON file: " << jsonPath << std::endl;
        return CStatus("Failed to open JSON file");
    }

    nlohmann::json graphJson;
    try {
        jsonFile >> graphJson;
    } catch (const std::exception& e) {
        std::cerr << "Error parsing JSON: " << e.what() << std::endl;
        return CStatus("Error parsing JSON");
    }

    return buildPipelineFromGraphJson(this, graphJson);
}

CStatus GPipeline::buildFromJsonStr(const std::string& jsonStr) {
    nlohmann::json graphJson;
    try {
        graphJson = nlohmann::json::parse(jsonStr);
    } catch (const std::exception& e) {
        std::cerr << "Error parsing JSON string: " << e.what() << std::endl;
        return CStatus("Error parsing JSON string");
    }

    return buildPipelineFromGraphJson(this, graphJson);
}

CStatus GPipeline::collectNodeChildren(
        const std::string& jsonPath,
        std::unordered_map<std::string, std::vector<std::string>>& childrenMap) {
    std::ifstream jsonFile(jsonPath);
    if (!jsonFile.is_open()) {
        std::cerr << "Failed to open JSON file: " << jsonPath << std::endl;
        return CStatus("Failed to open JSON file");
    }

    nlohmann::json graphJson;
    try {
        jsonFile >> graphJson;
    } catch (const std::exception& e) {
        std::cerr << "Error parsing JSON: " << e.what() << std::endl;
        return CStatus("Error parsing JSON");
    }

    return collectNodeChildrenFromGraphJson(graphJson, childrenMap);
}

CStatus GPipeline::collectNodeChildrenFromStr(
        const std::string& jsonStr,
        std::unordered_map<std::string, std::vector<std::string>>& childrenMap) {
    nlohmann::json graphJson;
    try {
        graphJson = nlohmann::json::parse(jsonStr);
    } catch (const std::exception& e) {
        std::cerr << "Error parsing JSON string: " << e.what() << std::endl;
        return CStatus("Error parsing JSON string");
    }

    return collectNodeChildrenFromGraphJson(graphJson, childrenMap);
}

std::future<CStatus> GPipeline::asyncProcess(CSize runTimes, std::launch policy) {
    DAOGRAPH_ASSERT_INIT_THROW_ERROR(false)

    return std::async(policy, [runTimes, this] {
        return process(runTimes);
    });
}


CStatus GPipeline::cancel() {
    DAOGRAPH_ASSERT_INIT(true)
    return repository_.pushAllState(GElementState::CANCEL);
}


CStatus GPipeline::suspend() {
    DAOGRAPH_ASSERT_INIT(true)
    return repository_.pushAllState(GElementState::SUSPEND);
}


CStatus GPipeline::disrupt() {
    return suspend();
}


CStatus GPipeline::resume() {
    // 直接恢复正常状态好了
    DAOGRAPH_ASSERT_INIT(true)
    return repository_.pushAllState(GElementState::NORMAL);
}


CStatus GPipeline::proceed() {
    return resume();
}


CStatus GPipeline::dump(std::ostream& oss) {
    DAOGRAPH_FUNCTION_BEGIN
    DAOGRAPH_ASSERT_NOT_NULL(element_manager_)
    oss << std::fixed << std::setprecision(2);    // 小数点最多展示2位数字

    oss << "\ndigraph DaoGraph {\n";
    oss << "compound=true;\n";

    for (const auto& element : element_manager_->manager_elements_) {
        DAOGRAPH_ASSERT_NOT_NULL(element)
        element->dump(oss);
    }

    oss << "}\n\n";
    std::cout.unsetf(std::ios::fixed);
    DAOGRAPH_FUNCTION_END
}


CStatus GPipeline::perf(std::ostream& oss) {
    DAOGRAPH_FUNCTION_BEGIN
    DAOGRAPH_ASSERT_INIT_THROW_ERROR(false)

    status = GPerf::perf(this, oss);
    DAOGRAPH_FUNCTION_END
}



GPipelinePtr GPipeline::setGEngineType(GEngineType type) {
    DAOGRAPH_ASSERT_INIT_THROW_ERROR(false)
    DAOGRAPH_ASSERT_NOT_NULL_THROW_ERROR(element_manager_)

    element_manager_->setEngineType(type);
    return this;
}


GPipelinePtr GPipeline::setUniqueThreadPoolConfig(const UThreadPoolConfig& config) {
    DAOGRAPH_FUNCTION_BEGIN
    DAOGRAPH_ASSERT_INIT_THROW_ERROR(false)
    DAOGRAPH_THROW_EXCEPTION_BY_STATUS(config.check())

    /**
     * 实际是将信息传递给 schedule中，如果是unique的话，就使用这个参数
     * 如果是 shared的话，其实配置是无效的
     */
    schedule_.config_ = config;
    return this;
}


GPipelinePtr GPipeline::setSharedThreadPool(UThreadPoolPtr ptr) {
    DAOGRAPH_FUNCTION_BEGIN
    DAOGRAPH_ASSERT_INIT_THROW_ERROR(false)
    DAOGRAPH_THROW_EXCEPTION_BY_STATUS(schedule_.makeType(ptr))
    return this;
}


CSize GPipeline::getMaxPara() {
    DAOGRAPH_ASSERT_INIT_THROW_ERROR(false)
    DAOGRAPH_ASSERT_NOT_NULL_THROW_ERROR(element_manager_)
    return element_manager_->calcMaxParaSize();
}


CSize GPipeline::trim() {
    DAOGRAPH_ASSERT_INIT_THROW_ERROR(false)
    DAOGRAPH_ASSERT_NOT_NULL_THROW_ERROR(element_manager_)
    return element_manager_->trim();
}


CStatus GPipeline::makeSerial() {
    DAOGRAPH_FUNCTION_BEGIN
    DAOGRAPH_ASSERT_INIT(false)
    DAOGRAPH_ASSERT_NOT_NULL(element_manager_)

    DAOGRAPH_RETURN_ERROR_STATUS_BY_CONDITION((schedule_.type_ != internal::GScheduleType::UNIQUE),    \
                                            "cannot set serial config without UNIQUE schedule")

    DAOGRAPH_RETURN_ERROR_STATUS_BY_CONDITION(!element_manager_->checkSerializable(),    \
                                       "cannot set serial config for this pipeline")

    UThreadPoolConfig config;
    config.default_thread_size_ = 0;    // 设置之后，不再开辟线程池，直接通过主线程执行pipeline的逻辑
    config.secondary_thread_size_ = 0;
    config.max_thread_size_ = 0;
    config.monitor_enable_ = false;
    config.batch_task_enable_ = false;
    schedule_.config_ = config;
    DAOGRAPH_FUNCTION_END
}


GPipelineState GPipeline::getCurState() const {
    return repository_.cur_state_;
}


CBool GPipeline::checkSeparate(GElementPtr fst, GElementPtr snd) const {
    CBool result = false;
    if ((fst == snd) || (!repository_.find(fst)) || (!repository_.find(snd))) {
        return result;
    }

    GElementPtr fstPatch, sndPatch = nullptr;
    GElementPtr ancestor = GSeparateOptimizer::getNearestAncestor(fst, snd, &fstPatch, &sndPatch);
    if (ancestor == fst || ancestor == snd) {
        // 其中一个是另外一个的祖先，则直接认为是分离的
        result = true;
    } else if (ancestor) {
        // 两个同属于一个 group 的情况，则根据 group 的属性来决定
        DAOGRAPH_THROW_EXCEPTION_BY_CONDITION(!ancestor->isGGroup(), "calculate ancestor failed, not a group.")
        result = (dynamic_cast<GGroupPtr>(ancestor))->isSeparate(fstPatch, sndPatch);
    } else {
        // ancestor == nullptr，则认定fst 和 snd 的 ancestor 是pipeline
        result = GSeparateOptimizer::checkSeparate(element_manager_->manager_elements_, fstPatch, sndPatch);
    }

    return result;
}


CStatus GPipeline::save(const std::string& path) {
    DAOGRAPH_FUNCTION_BEGIN
    DAOGRAPH_ASSERT_INIT(false)

    // 不允许有同名的 element 被存储
    for (const auto* element : repository_.elements_) {
        DAOGRAPH_ASSERT_NOT_NULL(element)
        DAOGRAPH_RETURN_ERROR_STATUS_BY_CONDITION(element->isGAdaptor(),
                                                element->name_ + " is GAdaptor, cannot be saved.")
    }

#if __cplusplus >= 201703L
    status = GStorage::save(this, path);
#else
    status = CStatus("save function support cpp17+ only");
#endif
    DAOGRAPH_FUNCTION_END
}


CStatus GPipeline::load(const std::string& path) {
    DAOGRAPH_FUNCTION_BEGIN
    DAOGRAPH_ASSERT_INIT(false)

#if __cplusplus >= 201703L
    status = GStorage::load(this, path);
#else
    status = CStatus("load function support cpp17+ only");
#endif

    DAOGRAPH_FUNCTION_END
}


CStatus GPipeline::initEnv() {
    DAOGRAPH_FUNCTION_BEGIN
    DAOGRAPH_ASSERT_NOT_NULL(event_manager_, element_manager_, stage_manager_)

    status = schedule_.init();
    DAOGRAPH_FUNCTION_CHECK_STATUS

    auto tp = schedule_.getThreadPool();
    event_manager_->setThreadPool(tp);
    // 这里设置线程池，是为了将tp信息，传递给内部的engine类
    element_manager_->setThreadPool(tp);

    // 设置所有的element 中的thread_pool
    repository_.fetchAll(element_manager_);
    repository_.setThreadPool(tp);
    status += repository_.init();
    DAOGRAPH_FUNCTION_END
}


CStatus GPipeline::innerRegister(GElementPtr element, const GElementPtrSet &depends,
                                 const std::string &name, CSize loop) {
    DAOGRAPH_FUNCTION_BEGIN
    DAOGRAPH_ASSERT_NOT_NULL(element)
    DAOGRAPH_ASSERT_INIT(false)

    const std::string& curName = name.empty() ? element->getName() : name;
    status = element->addElementInfo(depends, curName, loop);
    DAOGRAPH_FUNCTION_CHECK_STATUS

    status = element->addManagers(param_manager_, event_manager_, stage_manager_);
    DAOGRAPH_FUNCTION_CHECK_STATUS
    status = element_manager_->add(element);
    DAOGRAPH_FUNCTION_CHECK_STATUS
    repository_.insert(element);

    DAOGRAPH_FUNCTION_END
}


CStatus GPipeline::__registerGElement_4py(DaoGraph::GElementPtr element, const DaoGraph::GElementPtrSet &depends,
                                         const std::string &name, CSize loop) {
    return innerRegister(element, depends, name, loop);
}


GPipelinePtr GPipeline::__addGEvent_4py(GEventPtr event, const std::string& key) {
    DAOGRAPH_FUNCTION_BEGIN
    DAOGRAPH_ASSERT_NOT_NULL_THROW_ERROR(event, event_manager_, param_manager_)
    event_manager_->param_manager_ = this->param_manager_;
    status = event_manager_->__create_4py(event, key);
    DAOGRAPH_THROW_EXCEPTION_BY_STATUS(status)

    return this;
}


GPipelinePtr GPipeline::__addGDaemon_4py(GDaemonPtr daemon, CMSec ms) {
    DAOGRAPH_FUNCTION_BEGIN
    DAOGRAPH_ASSERT_NOT_NULL_THROW_ERROR(daemon, param_manager_, event_manager_, daemon_manager_)
    daemon->setGParamManager(this->param_manager_);
    daemon->setGEventManager(this->event_manager_);
    daemon->setInterval(ms);
    status = daemon_manager_->add(daemon);
    DAOGRAPH_THROW_EXCEPTION_BY_STATUS(status)
    return this;
}


GPipelinePtr GPipeline::__addGStage_4py(GStagePtr stage, const std::string& key, CInt threshold) {
    DAOGRAPH_FUNCTION_BEGIN
    DAOGRAPH_ASSERT_NOT_NULL_THROW_ERROR(stage, param_manager_, stage_manager_)
    stage_manager_->setGParamManager(param_manager_);
    stage_manager_->__create_4py(stage, key, threshold);
    return this;
}


std::string GPipeline::__dump_4py() {
    std::ostringstream oss;
    auto status = this->dump(oss);
    DAOGRAPH_THROW_EXCEPTION_BY_STATUS(status);
    return oss.str();
}


std::string GPipeline::__perf_4py() {
    std::ostringstream oss;
    auto status = this->perf(oss);
    DAOGRAPH_THROW_EXCEPTION_BY_STATUS(status);
    return oss.str();
}

DAOGRAPH_NAMESPACE_END
