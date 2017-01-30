#pragma once

namespace hemlock {
    using Sender = const void*;

    template<typename ...Args>
    class Delegate {
        using GenericFunc = void*;
        using Executor = void(*)(Sender, GenericFunc, Args...);
    public:
        Delegate(Sender target, GenericFunc operation, Executor executor) :
            m_target(target),
            m_operation(operation),
            m_executor(executor) {};
        Delegate() : m_target(nullptr) {};
        ~Delegate() {};

        void trigger(Args... args) const {
            return m_executor(m_target, m_operation, args...);
        }
        void operator()(Args... args) const {
            return m_executor(m_target, m_operation, args...);
        }

        template<typename T>
        static Delegate create(T* target, void (T::*operation)(Args...)) {
            return Delegate(target, *(GenericFunc*)&operation, execute<T>);
        }
    private:
        template<typename T>
        static void execute(Sender target, GenericFunc operation, Args... args) {
            using Func = void(T::*)(Args...);

            auto targ = (T*)(target);
            Func op = *(Func*)&operation;

            (targ->*op)(args...);
        }

        Sender m_target;
        GenericFunc m_operation;
        Executor m_executor;
    };

    template<typename T, typename ...Args>
    Delegate<Args...> makeDelegate(T& target, void (T::*operation)(Args...)) {
        return Delegate<Args...>::create<T>(&target, operation);
    }
    //template<typename T, typename ...Args>
    //Delegate<Args...> makeDelegate(T& target, void (T::*operation)(Args...) const) {
    //    return Delegate<Args...>::create<T>(&target, operation);
    //}

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
