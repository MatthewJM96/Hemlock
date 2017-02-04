#pragma once

#include <type_traits>

namespace hemlock {
    using Sender = const void*;

    template<typename Ret, typename ...Args>
    class RDelegate {
    protected:
        using GenericFunc = void*;
        using Executor = Ret(*)(Sender, GenericFunc, Args...);
    public:
        RDelegate(Sender target, GenericFunc operation, Executor executor) {
            m_target = target;
            m_operation = operation;
            m_executor = executor;
        }
        RDelegate() : m_target(nullptr) {};
        ~RDelegate() {};

        // Execute delegate's target function.
        Ret trigger(Args... args) const {
            return m_executor(m_target, m_operation, args...);
        }
        Ret operator()(Args... args) const {
            return m_executor(m_target, m_operation, args...);
        }

        // Check if two delegates are equal or not.
        bool operator==(const RDelegate& delegate) const {
            return m_target == delegate.m_target && m_operation == delegate.m_operation;
        }
        bool operator!=(const RDelegate& delegate) const {
            return m_target != delegate.m_target || m_operation != delegate.m_operation;
        }
        
        // Static function pointers.
        static RDelegate create(Ret (*operation)(Args...)) {
            return RDelegate(nullptr, *(GenericFunc*)&operation, execute);
        }
        // Non-const member function pointers.
        template<typename T>
        static RDelegate create(T* target, Ret (T::*operation)(Args...)) {
            return RDelegate(target, *(GenericFunc*)&operation, executeObj<T>);
        }
        // Const member function pointers.
        template<typename T>
        static RDelegate create(T* target, Ret (T::*operation)(Args...) const) {
            return RDelegate(target, *(GenericFunc*)&operation, executeObj<T>);
        }
    protected:
        template<typename T>
        static Ret executeObj(Sender target, GenericFunc operation, Args... args) {
            using Func = Ret (T::*)(Args...);

            auto targ = (T*)(target);
            Func op = *(Func*)&operation;

            return (targ->*op)(args...);
        }
        static Ret execute(Sender target, GenericFunc operation, Args... args) {
            using Func = Ret (*)(Args...);

            Func op = *(Func*)&operation;

            return op(args...);
        }

        Sender m_target;
        GenericFunc m_operation;
        Executor m_executor;
    };

    template<typename ...Args>
    class Delegate : public RDelegate<void, Args...> {
    public:
        Delegate(Sender target, GenericFunc operation, Executor executor) {
            m_target = target;
            m_operation = operation;
            m_executor = executor;
        }

        // Static function pointers.
        static Delegate create(void(*operation)(Args...)) {
            return Delegate(nullptr, *(GenericFunc*)&operation, execute);
        }
        // Non-const member function pointers.
        template<typename T>
        static Delegate create(T* target, void(T::*operation)(Args...)) {
            return Delegate(target, *(GenericFunc*)&operation, executeObj<T>);
        }
        // Const member function pointers.
        template<typename T>
        static Delegate create(T* target, void(T::*operation)(Args...) const) {
            return Delegate(target, *(GenericFunc*)&operation, executeObj<T>);
        }    
    };

    // Static function pointers.
    template<typename Ret, typename ...Args>
    RDelegate<Ret, Args...> makeRDelegate(Ret (*operation)(Args...)) {
        return RDelegate<Ret, Args...>::create(operation);
    }
    // Non-const member function pointers.
    template<typename Ret, typename T, typename ...Args>
    RDelegate<Ret, Args...> makeRDelegate(T& target, Ret (T::*operation)(Args...)) {
        return RDelegate<Ret, Args...>::create<T>(&target, operation);
    }
    // Const member function pointers.
    template<typename Ret, typename T, typename ...Args>
    RDelegate<Ret, Args...> makeRDelegate(T& target, Ret (T::*operation)(Args...) const) {
        return RDelegate<Ret, Args...>::create<T>(&target, operation);
    }

    // Static function pointers.
    template<typename ...Args>
    Delegate<Args...> makeDelegate(void (*operation)(Args...)) {
        return Delegate<Args...>::create(operation);
    }
    // Non-const member function pointers.
    template<typename T, typename ...Args>
    Delegate<Args...> makeDelegate(T& target, void (T::*operation)(Args...)) {
        return Delegate<Args...>::create<T>(&target, operation);
    }
    // Const member function pointers.
    template<typename T, typename ...Args>
    Delegate<Args...> makeDelegate(T& target, void (T::*operation)(Args...) const) {
        return Delegate<Args...>::create<T>(&target, operation);
    }
    
    template<typename DataRet, typename ...Args>
    class RPriorityEvent {
    public:
        using StructRet = struct {
            bool shouldContinue;
            std::enable_if_t<!std::is_void<DataRet>::value, DataRet> data;
        };
        using Return = typename std::conditional<std::is_void<DataRet>::value, bool, StructRet>::type;
    private:
        using Callback = RDelegate<Return, Sender, Args...>;
    public:
        using PriorityCallback = std::pair<ui32, Callback>;

        RPriorityEvent(Sender s = nullptr) {
            m_sender = s;
        };
        ~RPriorityEvent() {};

        void setSender(Sender s) {
            m_sender = s;
        }

        Return trigger(Args... a) {
            Return ret;
            for (auto& callback : m_callbacks) {
                ret = callback.second(m_sender, a...);
                if (!ret.shouldContinue) break;
            }
            return ret;
        }
        Return operator()(Args... a) {
            return trigger(a...);
        }

        void add(PriorityCallback pc) {
            m_callbacks.insert(pc);
        }
        void add(Callback c, ui32 p = 0) {
            add(PriorityCallback(p, c));
        }
        void operator+=(PriorityCallback pc) {
            add(pc);
        }

        // TODO(Matthew): Surely there's a better way of doing this?
        void removeAll(Callback c) {
            for (auto& it = m_callbacks.begin(); it != m_callbacks.end(); ++it) {
                if ((*it).second == c) {
                    m_callbacks.erase(it);
                    removeAll(c);
                    return;
                }
            }
        }
        void operator-=(Callback c) {
            removeAll(c);
        }
        void remove(PriorityCallback pc) {
            for (auto& it = m_callbacks.lower_bound(pc); it != m_callbacks.upper_bound(pc); ++it) {
                if ((*it).second == pc.second) {
                    m_callbacks.erase(it);
                    return;
                }
            }
        }
        void operator-=(PriorityCallback pc) {
            remove(pc);
        }

        static bool compare(const PriorityCallback& a, const PriorityCallback& b) {
            return a.first < b.first;
        };
    protected:
        Sender m_sender;

        using PriorityCallbackList = std::multiset<std::pair<ui32, Callback>, decltype(&compare)>;
        PriorityCallbackList m_callbacks = PriorityCallbackList(&RPriorityEvent::compare);
    };
    template <typename ...Args>
    class PriorityEvent : public RPriorityEvent<void, Args...> {
    public:
        PriorityEvent(Sender s = nullptr) {
            m_sender = s;
        };
        ~PriorityEvent() {};

        void trigger(Args... a) {
            for (auto& callback : m_callbacks) {
                bool ret = callback.second(m_sender, a...);
                if (!ret) break;
            }
        }
        void operator()(Args... a) {
            trigger(a...);
        }
    };

    template<typename ...Args>
    class Event {
        using Callback = Delegate<Sender, Args...>;
    public:
        Event(Sender s = nullptr) {
            m_sender = s;
        };
        ~Event() {};

        void setSender(Sender s) {
            m_sender = s;
        }

        void trigger(Args... a) {
            for (auto& callback : m_callbacks) {
                callback(m_sender, a...);
            }
        }
        void operator()(Args... a) {
            trigger(a...);
        }

        void add(Callback c) {
            m_callbacks.push_back(c);
        }
        void operator+=(Callback c) {
            add(c);
        }

        void remove(Callback c) {
            for (auto& it = m_callbacks.begin(); it != m_callbacks.end(); ++it) {
                if (*it == c) {
                    m_callbacks.erase(it);
                    return;
                }
            }
        }
        void operator-=(Callback c) {
            remove(c);
        }
    private:
        Sender m_sender;
        std::vector<Callback> m_callbacks;
    };
}
namespace h = hemlock;
