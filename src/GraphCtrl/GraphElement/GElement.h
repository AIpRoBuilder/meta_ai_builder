/***************************

@File: GElement.h
@Time: 2021/6/1 10:13 下午
@Desc: 
***************************/

#ifndef DAOGRAPH_GELEMENT_H
#define DAOGRAPH_GELEMENT_H

#include <set>
#include <vector>
#include <string>
#include <atomic>
#include <algorithm>
#include <memory>
#include <functional>
#include <unordered_map>
#include <mutex>
#include <type_traits>
#include <typeinfo>

#include "GElementDefine.h"
#include "GElementObject.h"
#include "GElementRelation.h"

DAOGRAPH_NAMESPACE_BEGIN

enum class GMultiConditionType;
struct GPerfInfo;
class GElementRepository;

class GElement : public GElementObject,
                 public CDescInfo {
public:
    using GWaitFunc = std::function<CBool()>;

    /**
     * 实现添加切面的逻辑
     * @tparam TAspect
     * @tparam TParam
     * @param param
     * @return
     */
    template<typename TAspect, typename TParam = GAspectDefaultParam,
            c_enable_if_t<std::is_base_of<GAspect, TAspect>::value, int> = 0,
            c_enable_if_t<std::is_base_of<GAspectParam, TParam>::value, int> = 0>
    GElement* addGAspect(TParam* param = nullptr);

    /**
     * 实现添加模板切面的逻辑
     * @tparam TAspect
     * @tparam Args
     * @return
     */
    template<typename TAspect, typename ...Args,
            c_enable_if_t<std::is_base_of<GTemplateAspect<Args...>, TAspect>::value, int> = 0>
    GElement* addGAspect(Args... args);

    /**
     * 添加当前element内部参数
     * @tparam T
     * @param key
     * @param param
     * @return
     */
    template<typename T,
            c_enable_if_t<std::is_base_of<GElementParam, T>::value, int> = 0>
    GElement* addEParam(const std::string& key, T* param);

    /**
     * 添加依赖节点信息
     * @param elements
     * @return
     */
    CStatus addDependGElements(const std::set<GElement *>& elements);

    /**
     * 设置name信息
     * @param name
     * @return
     */
#ifdef _WIN32
    CVoidPtr setName(const std::string& name) override;
#else
    GElement* setName(const std::string& name) override;
#endif

    /**
     * 设置循环次数
     * @param loop
     * @return
     */
    GElement* setLoop(CSize loop);

    /**
     * 设置level信息，用于控制init和destroy方法的执行顺序
     * level值越低，函数越先执行
     * @param level
     * @return
     */
    GElement* setLevel(CLevel level);

    /**
     * 设置visible信息。当 visible = false 的时候，算子实际不执行
     * @param visible
     * @return
     */
    GElement* setVisible(CBool visible);

    /**
     * 设定绑定的线程id
     * @param index，需要绑定的 thread id 信息
     * @return
     * @notice 本接口仅保证绑定线程优先调度，但不保证最终一定在绑定线程上执行。若不了解调度机制，不建议使用本接口，否则可能导致运行时阻塞。
     */
    GElement* setBindingIndex(CIndex index);

    /**
     * 设定当前算子的超时时间
     * @param timeout 超时时间
     * @param strategy 当超时的时候，处理的策略
     * @return
     */
    GElement* setTimeout(CMSec timeout,
                         GElementTimeoutStrategy strategy = GElementTimeoutStrategy::AS_ERROR);

    /**
     * 设置为微任务
     * @param macro
     * @return
     */
    GElement* setMacro(CBool macro);

    /**
     * 设定当前element每次run前等待的函数，轮询该函数直到返回 true 才继续执行
     * @param func 传入 nullptr 表示不等待
     * @return
     * @notice 等待期间支持 cancel 和 suspend。func 抛出异常时，element 返回错误状态
     */
    GElement* setWaitFunc(const GWaitFunc& func);

    /**
     * 当前element是否是一个 group逻辑
     * @return
     */
    CBool isGGroup() const;

    /**
     * 当前element是否是一个 adaptor逻辑
     * @return
     */
    CBool isGAdaptor() const;

    /**
     * 当前element是否是一个 node逻辑
     * @return
     */
    CBool isGNode() const;

    /**
     * 获取当前节点状态信息
     * @return
     */
    GElementState getCurState() const;

    /**
     * 获取loop值信息
     * @return
     */
    CSize getLoop() const;

    /**
     * 删除一个依赖的节点信息
     * @param element
     * @return
     * @notice 删除依赖关系之后，可能会出现 dag 无法连通的情况
     */
    CStatus removeDepend(GElement* element);

    /**
     * 获取当前节点的相关关系信息，包含前驱、后继、从属关系、包含元素
     * @return
     */
    GElementRelation getRelation() const;

    /**
     * 实现连续注册的语法糖，形如：
     *  (*a)-->b&c;
     *  (*b)-->d;
     *  (*c)-->d;
     *  (*b)*2;
     * @return
     */
    GElement& operator--(int) noexcept;
    GElement& operator>(GElement* element);
    GElement& operator&(GElement* element);
    GElement& operator*(CSize loop) noexcept;

public:  /*** 全局注册相关接口 ***/
    using GElementCreator = std::function<GElement*()>;

    /**
     * 自动推导类名：优先使用子类的 staticClassName()，否则使用 typeid 名称
     */
    template<typename T>
    static std::string defaultClassName();

    /**
     * 手动注册：传入类名和自定义构造器
     */
    static bool registerClass(const std::string& name, GElementCreator creator);

    /**
     * 模板注册：默认使用 defaultClassName 推导类名
     */
    template<typename T>
    static bool registerClass(const std::string& name = defaultClassName<T>());

    /**
     * 根据类名创建实例
     */
    static GElement* create(const std::string& name);

    /**
     * 获取当前所有已注册的类名
     */
    static std::vector<std::string> registeredClasses();

    /**
     * 查询类名是否已注册
     */
    static bool isRegistered(const std::string& name);

    /**
     * 子类使用的自动注册助手（程序启动时注册）
     */
    template<typename T>
    class AutoRegistrar {
    public:
        explicit AutoRegistrar(const std::string& name = GElement::defaultClassName<T>()) {
            GElement::registerClass<T>(name);
        }
    };

protected:
    /**
     * 构造函数
     */
    explicit GElement() = default;

    /**
     * 析构函数
     */
    ~GElement() override;

    /**
     * init()后，第一次执行run之前，会执行的函数
     * @return
     * @notice 主要为了弥补init()方法，不是并发执行的缺陷。也属于单次执行的函数
     */
    virtual CStatus prepareRun();

    /**
     * run() 方法完成之后（包含所有循环次数和 isHold逻辑）的校验函数
     * @return
     * @notice 不建议在异步element中使用
     */
    virtual CStatus checkRunResult();

    /**
     * 是否持续进行
     * 默认为false，表示执行且仅执行一次
     * @return
     */
    virtual CBool isHold();

    /**
     * 用于在MultiCondition中被判定，是否可以执行。
     * @return
     * @notice 默认返回false，不执行
     */
    virtual CBool isMatch();

    /**
     * 崩溃流程处理
     * @param ex
     * @return
     */
    CStatus crashed(const CException& ex);

    /**
     * 获取当前element内部参数
     * @tparam T
     * @param key
     * @return
     */
    template<typename T,
            c_enable_if_t<std::is_base_of<GElementParam, T>::value, int> = 0>
    T* getEParam(const std::string& key);

    /**
     * 获取执行线程对应的信息
     * @return
     * @notice 启动线程返回-1（DAOGRAPH_MAIN_THREAD_ID），辅助线程返回-2（DAOGRAPH_SECONDARY_THREAD_COMMON_ID），主线程返回 线程index
     */
    CIndex getThreadIndex();

    /**
     * 判断当前是否超时
     * @return
     */
    CBool isTimeout() const;

    /**
     * 获取绑定线程id信息
     * @return
     * @notice 不同的group类型，获取 binding index 的方式不同
     */
    CIndex getBindingIndex() const;

    DAOGRAPH_NO_ALLOWED_COPY(GElement);

/********************************/
/** 使用者请勿复写private中的函数 **/
/********************************/
private:
    template<typename T>
    struct HasStaticClassName {
        template<typename X>
        static auto test(int) -> decltype(std::string(X::staticClassName()), std::true_type{});
        template<typename>
        static std::false_type test(...);
        static constexpr bool value = decltype(test<T>(0))::value;
    };

    using RegistryMap = std::unordered_map<std::string, GElementCreator>;

    static RegistryMap& registry();
    static std::mutex& registryMutex();

    /**
     * 恢复运行最初的信息
     * @return
     */
    CVoid refresh();

    /**
     * 判定当前的内容，是否需要异步执行
     * @return
     */
    CBool isAsync() const;

    /**
     * 判断当前是否可以在运行时更新图结构逻辑
     * @return
     */
    CBool isMutable() const;

    /**
     * 判断当前是否是微节点
     * @return
     */
    CBool isMacro() const;

    /**
     * 判断当前element是否已经被注册到特定pipeline中了。避免反复注册的问题
     * @return
     */
    virtual CBool isRegistered() const;

    /**
     * 执行切面逻辑
     * @param aspectType
     * @param curStatus
     * @return
     */
    CStatus doAspect(const internal::GAspectType& aspectType,
                     const CStatus& curStatus = CStatus());

    /**
     * 设置element信息
     * @param depends
     * @param name
     * @param loop
     * @return
     */
    virtual CStatus addElementInfo(const std::set<GElement *>& depends,
                                   const std::string& name, CSize loop);

    /**
     * 设置manager信息
     * @param paramManager
     * @param eventManager
     * @param stageManager
     * @return
     */
    virtual CStatus addManagers(GParamManagerPtr paramManager,
                                GEventManagerPtr eventManager,
                                GStageManagerPtr stageManager);

    /**
     * 包含切面相关功能的函数，fat取自fatjar的意思
     * @param type
     * @return
     */
    CStatus fatProcessor(const CFunctionType& type);

    /**
     * 如果配置了等待函数，则阻塞轮询，直到该函数返回 true
     * @return
     */
    CStatus waitForFunc();

    /**
     * 设置线程池信息
     * @param ptr
     * @return
     */
    GElement* setThreadPool(UThreadPoolPtr ptr);

    /**
     * 设置线程池信息，供个别有 manager 的 group使用
     * @return
     */
    virtual GElement* setThreadPoolEx(UThreadPoolPtr ptr);

    /**
     * graphviz dump 逻辑
     * @param oss
     * @return
    */
    virtual CVoid dump(std::ostream& oss);

    /**
     * graphviz dump 边逻辑
     * @param oss
     * @param src 表示开始的元素
     * @param dst 表示结束的元素
     * @param label
     * @return
    */
    CVoid dumpEdge(std::ostream& oss, GElement* src, GElement* dst,
                   const std::string& label = DAOGRAPH_EMPTY);

    /**
     * graphviz dump 点逻辑
     * @param oss
    */
    virtual CVoid dumpElement(std::ostream& oss);

    /**
     * dump 当前element的header。主要就是为了代码
     * @param oss
     * @return
     */
    CVoid dumpElementHeader(std::ostream& oss);

    /**
     * graphviz dump perf逻辑
     * @param oss
     * @return
     */
    CVoid dumpPerfInfo(std::ostream& oss);

    /**
     * 判断是否进入 suspend 状态。如果是的话，则等待恢复。未进入 suspend 状态，则继续运行
     * @return
     */
    CVoid checkSuspend();

    /**
     * 判断当前元素，是否可以线性执行。默认返回true
     * @return
     */
    virtual CBool isSerializable() const;

    /**
     * 弹出一个最后一个切面
     * @return
     */
    CStatus popLastAspect();

    /**
     * 异步执行
     * @return
     */
    CStatus asyncRun();

    /**
     * 异步获取结果信息
     * @return
     */
    CStatus getAsyncResult();

    /**
     * 判断当前节点，是否符合执行逻辑。主要用于init的早期阶段
     * @return
     */
    virtual CStatus checkSuitable();

    /**
     * 获取链路所有的 belong信息
     * @param reverse
     * @return
     */
    std::vector<GElement *> getDeepPath(CBool reverse) const;

    /**
     * 判断是否是默认绑定策略
     * @return
     */
    CBool isDefaultBinding() const;

    /**
     * 更新切面信息
     * @return
     */
    GElement* updateAspectInfo();

    /**
     * 获取其中包含的内容
     * @return
     * @notice 仅在为 group 的情况下有意义
     */
    virtual std::vector<GElement *> getChildren() const;

private:
    /** 状态相关信息 */
    CBool done_ { false };                                                    // 判定被执行结束
    CBool visible_ { true };                                                  // 判定可见的，如果被删除的话，则认为是不可见的
    CBool is_init_ { false };                                                 // 判断是否init
    GElementType element_type_ { GElementType::ELEMENT };                     // 用于区分element 内部类型
    std::atomic<GElementState> cur_state_ { GElementState::NORMAL };          // 当前执行状态
    internal::GElementShape shape_ { internal::GElementShape::NORMAL };       // 元素位置类型

    /** 配置相关信息 */
    CSize loop_ { DAOGRAPH_DEFAULT_LOOP_TIMES };                                // 元素执行次数
    CLevel level_ { DAOGRAPH_DEFAULT_ELEMENT_LEVEL };                           // 用于设定init的执行顺序(值小的，优先init，可以为负数)
    CIndex binding_index_ { DAOGRAPH_DEFAULT_BINDING_INDEX };                   // 用于设定绑定线程id
    CMSec timeout_ { DAOGRAPH_DEFAULT_ELEMENT_TIMEOUT };                        // 超时时间信息（0表示不计算超时）
    GElementTimeoutStrategy timeout_strategy_ { GElementTimeoutStrategy::AS_ERROR };    // 判定超时的情况下，是否返回错误
    CBool is_marco_ { false };                                                // 微任务
    GWaitFunc wait_func_ { nullptr };                                         // run前轮询的等待函数，返回true才继续执行

    /** 执行期间相关信息 */
    GElementParamMap local_params_;                                           // 用于记录当前element的内部参数
    GAspectManagerPtr aspect_manager_ { nullptr };                            // 整体流程的切面管理类
    UThreadPoolPtr thread_pool_ { nullptr };                                  // 用于执行的线程池信息
    GPerfInfo* perf_info_ { nullptr };                                        // 用于perf的信息
    CBool is_prepared_ { false };                                             // 判断是否已经执行过 prepareRun() 方法

    /** 图相关信息 */
    std::atomic<CSize> left_depend_ { 0 };                                    // 当 left_depend_ 值为0的时候，即可以执行该element信息
    USmallVector<GElement *> run_before_ {};                                  // 被依赖的节点（后继）
    USmallVector<GElement *> dependence_ {};                                  // 依赖的节点信息（前驱）
    GElement* belong_ { nullptr };                                            // 从属的element 信息，如为nullptr，则表示从属于 pipeline
    GElementRepository* owner_repository_ { nullptr };                       // 所属pipeline仓库（包含未注册的节点）

    /** 异步执行相关信息 */
    std::future<CStatus> async_result_;                                       // 用于记录当前节点的异步执行情况
    UCvMutex suspend_locker_;                                                 // 控制停止执行锁信息

    friend class GNode;
    friend class GGroup;
    friend class GCluster;
    friend class GRegion;
    friend class GCondition;
    friend class GMutable;
    template<CInt> friend class GSome;
    template<GMultiConditionType> friend class GMultiCondition;
    friend class GPipeline;
    friend class GElementManager;
    friend class GElementSorter;
    friend class GAdapter;
    friend class GFunction;
    friend class GFence;
    template<CInt> friend class GCoordinator;
    template<typename T> friend class GSingleton;
    friend class GEngine;
    friend class GDynamicEngine;
    friend class GTopoEngine;
    friend class GStaticEngine;
    friend class GAspectObject;
    friend class GOptimizer;
    friend class GMaxParaOptimizer;
    friend class GTrimOptimizer;
    friend class GSeparateOptimizer;
    friend class GElementRepository;
    friend class GPerf;
    friend class GStorage;
    friend struct _GElementStorage;

    DAOGRAPH_DECLARE_GPARAM_MANAGER_WRAPPER_WITH_MEMBER
    DAOGRAPH_DECLARE_GEVENT_MANAGER_WRAPPER_WITH_MEMBER
    DAOGRAPH_DECLARE_GSTAGE_MANAGER_WRAPPER_WITH_MEMBER

public:
    virtual std::string getClassName() const = 0;
    /**
     * 创建同类型的新实例（默认不支持，需子类自行覆盖）
     */
    virtual GElement* clone() const;
    GElement* __addGAspect_4py(GAspectPtr aspect);
    CStatus __enterStage_4py(const std::string& key);
    CBool __isTimeout_4py();
    std::string __str_4py();
};

template<typename T>
std::string GElement::defaultClassName() {
    if constexpr (HasStaticClassName<T>::value) {
        return std::string(T::staticClassName());
    }
    return std::string(typeid(T).name());
}

inline GElement::RegistryMap& GElement::registry() {
    static RegistryMap registry;
    return registry;
}

inline std::mutex& GElement::registryMutex() {
    static std::mutex mutex;
    return mutex;
}

inline bool GElement::registerClass(const std::string& name, GElementCreator creator) {
    std::lock_guard<std::mutex> lock(registryMutex());
    registry()[name] = std::move(creator);
    return true;
}

template<typename T>
bool GElement::registerClass(const std::string& name) {
    return registerClass(name, [] { return new(std::nothrow) T(); });
}

inline GElement* GElement::create(const std::string& name) {
    std::lock_guard<std::mutex> lock(registryMutex());
    auto iter = registry().find(name);
    return iter == registry().end() ? nullptr : iter->second()->clone();
}

inline std::vector<std::string> GElement::registeredClasses() {
    std::lock_guard<std::mutex> lock(registryMutex());
    std::vector<std::string> names;
    names.reserve(registry().size());
    for (const auto& item : registry()) {
        names.emplace_back(item.first);
    }
    return names;
}

inline bool GElement::isRegistered(const std::string& name) {
    std::lock_guard<std::mutex> lock(registryMutex());
    return registry().find(name) != registry().end();
}

using GElementRef = GElement &;
using GElementCRef = const GElement &;
using GElementPtr = GElement *;
using GElementCPtr = const GElement *;
using GElementPPtr = GElementPtr *;
using GElementPtrArr = std::vector<GElementPtr>;
using GElementPtrMat2D = std::vector<GElementPtrArr>;
using GElementPtrSet = std::set<GElementPtr>;

#define DAOGRAPH_AUTO_REGISTER_GELEMENT(Derived) \
    namespace { ::DaoGraph::GElement::AutoRegistrar<Derived> _daograph_auto_registrar_##Derived; }

DAOGRAPH_NAMESPACE_END

#include "GElement.inl"

#endif //DAOGRAPH_GELEMENT_H
