/***************************

@File: SingletonProc.h
@Time: 2021/6/11 7:06 下午
@Desc: 
***************************/

#ifndef DAOGRAPH_USINGLETON_H
#define DAOGRAPH_USINGLETON_H

#include "USingletonDefine.h"
#include "../UtilsObject.h"

DAOGRAPH_NAMESPACE_BEGIN

template<typename T,
        USingletonType type = USingletonType::HUNGRY,
        CBool autoInit = false>
class USingleton : public UtilsObject {
public:
    explicit USingleton() noexcept {
        if (USingletonType::HUNGRY == type || autoInit) {
            /* 如果是饥汉模式，则直接构造 */
            create();
        }

        if (autoInit) {
            init();
        }
    }

    ~USingleton() override {
        clear();
    }

    /**
     * 获取singleton句柄信息
     * @return
     */
    T* get() {
        if (USingletonType::LAZY == type) {
            create();
        }

        T* handle = handle_;
        return handle;
    }

protected:
    CStatus init() override {
        DAOGRAPH_FUNCTION_BEGIN

        // 如果传入的是CObject类型的对象的话，则调用其init接口
        if (std::is_base_of<CObject, T>::value) {
            status = this->get()->init();
        }
        DAOGRAPH_FUNCTION_END
    }

    CStatus destroy() override {
        DAOGRAPH_FUNCTION_BEGIN
        if (std::is_base_of<CObject, T>::value) {
            status = this->get()->destroy();
        }
        DAOGRAPH_FUNCTION_END
    }

    /**
     * 单例的创建一个句柄
     * @return
     */
    CVoid create() {
        if (unlikely(nullptr == handle_)) {
            DAOGRAPH_LOCK_GUARD lock(lock_);
            if (nullptr == handle_) {
                handle_ = DAOGRAPH_SAFE_MALLOC_COBJECT(T)
            }
        }
    }

    /**
     * 销毁单例句柄
     * @return
     */
    CStatus clear() {
        DAOGRAPH_FUNCTION_BEGIN
        DAOGRAPH_LOCK_GUARD lock(lock_);
        DAOGRAPH_DELETE_PTR(handle_)
        DAOGRAPH_FUNCTION_END
    }

    DAOGRAPH_NO_ALLOWED_COPY(USingleton);

private:
    T* handle_ { nullptr };             // 对应的单例句柄信息
    std::mutex lock_;
};

DAOGRAPH_NAMESPACE_END

#endif //DAOGRAPH_USINGLETON_H
