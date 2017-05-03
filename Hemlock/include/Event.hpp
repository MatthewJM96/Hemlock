#pragma once

#include <type_traits>

#include "Delegate.hpp"

// The event systems provide a class that stores a collection of subscribers and that may be invoked.
// Subscribers are stored as delegates, meaning any variety of different kinds of function pointers and
// callables can be subscribed to the same event.
// On triggering a standard event, each subscriber is invoked in order of subscription. For prioritised
// events, subscribers are invoked in order of priority (lower integer priority value -> called earlier).
// In the case of prioritised events, a mechanism is provided to allow the last called event to return
// a value.
// While typically there are better options than a blind callback system when data must be passed around
// and callbacks can order themselves and also cause the sequence of callback invocations to break early,
// it has a use case in such scenarios as probing to see if an action is permissible by all stakeholders.
//   An example of this is in the case of determining if a game may quit: while the original game may only
//   have one stakeholder in whether the quit is permissible (the system that also would initiate the quit
//   process), it is imaginable that mods may become independent stakeholders and so need to be able to
//   prevent such an event occurring.
//   Using a priority event, it is trivially implemented by having two events: CanQuit and OnQuit.
//     CanQuit would be a priority event, which all stakeholders in the quit event could subscribe to
//     in order to allow or deny the actual quit process, while OnQuit would be a standard event which
//     would inform subscribers of the quit process occurring, allowing them to perform necessary actions
//     such as saving script state.

namespace hemlock {
    // Sender is the object responsible for handling the event, and is stored simply as a void pointer; making things
    // easier for us and harder for users to do stupid things.
    using Sender = const void*;

    // Subscribers must provide an executor and comparator, for delegates we provide a default
    // implementation of both at the end of this file.
    template<typename ReturnType, typename ...Parameters>
    ReturnType executeDelegate(DelegateAny callback, Sender sender, Parameters... parameters);
    template<typename ReturnType, typename ...Parameters>
    bool compareDelegate(DelegateAny callback1, DelegateAny callback2);

    // A useful enumeration of the different event base classes.
    enum class EventType {
        EVENT,
        PRIORITY_EVENT
    };
    // Event interface, common to both standard and prioritised events.
    class IEvent {
    public:
        // Constructors
        IEvent(Sender sender = nullptr) :
            m_sender(sender)
        { /* Empty */ }

        // Allows tranferring "ownership" of an event. Use with caution!
        void setSender(Sender sender) {
            m_sender = sender;
        }

    // Older MSVC toolsets complain about converting function pointers to void*.
    #if defined(_MSC_FULL_VER) && (_MSC_FULL_VER < 190000000)
        using GenericExecutor = void(*)();
    #else
        using GenericExecutor = void*;
    #endif
        using GenericComparator = bool(*)(DelegateAny, DelegateAny);
    protected:
        Sender m_sender;
    };
    // Event base class, provides all aspects that don't depend on templated parameters.
    class EventBase : public IEvent {
    protected:
        struct Subscriber {
            GenericExecutor   executor;
            GenericComparator comparator;
            DelegateAny       callback;

            bool operator==(const Subscriber& rhs) {
                return (this->executor   == rhs.executor)
                    && (this->comparator == rhs.comparator)
                    && this->comparator(this->callback, rhs.callback);
            }
            bool operator!=(const Subscriber& rhs) {
                return !(*this == rhs);
            }
        };
        using Subscribers = std::vector<Subscriber>;
    public:
        // Constructors
        EventBase(Sender sender = nullptr) :
            IEvent(sender)
        { /* Empty */ }

        // Adds a subscriebr to the event.
        void add(Subscriber subscriber) {
            m_subscribers.push_back(subscriber);
        }
        // operator+= is an alias of add.
        void operator+=(Subscriber subscriber) {
            add(subscriber);
        }

        // Removes a subscriber from the event (only the first instance found is removed).
        void remove(Subscriber subscriber) {
            const auto& it = std::find(m_subscribers.begin(), m_subscribers.end(), subscriber);
            if (it != m_subscribers.end()) {
                m_subscribers.erase(it);
            }
        }
        // operator-= is an alias of remove.
        void operator-=(Subscriber subscriber) {
            remove(subscriber);
        }
    protected:
        Subscribers m_subscribers;
    };
    // The standard event class, provides the most trivial event system in which subscribers are invoked in order of
    // subscription and all subscribers get processed.
    template <typename ...Parameters>
    class Event : public EventBase {
        using Executor   = void(*)(DelegateAny, Sender, Parameters...);
        using MyDelegate = Delegate<void, Sender, Parameters...>;
    public:
        // Constructors
        Event(Sender sender = nullptr) :
            EventBase(sender)
        { /* Empty */ }

        // Triggers the event, invoking all subscribers.
        void trigger(Parameters... parameters) {
            for (auto& subscriber : m_subscribers) {
                Executor executor = *(Executor*)&subscriber.executor;
                executor(subscriber.callback, m_sender, parameters...);
            }
        }
        // operator() is an alias of trigger.
        void operator()(Parameters... parameters) {
            trigger(parameters...);
        }

        // A convenience function that adds a delegate subscriber to the event.
        void add(MyDelegate delegate) {
            Subscriber subscriber = { (GenericExecutor)&executeDelegate<void, Parameters...>, &compareDelegate<void, Parameters...>, DelegateAny(delegate) };
            EventBase::add(subscriber);
        }
        // operator+= is an alias of add.
        void operator+=(MyDelegate delegate) {
            add(delegate);
        }

        // A convenience function that removes a delegate subscriber from the event (only the first instance found is removed).
        void remove(MyDelegate delegate) {
            Subscriber subscriber = { (GenericExecutor)&executeDelegate<void, Parameters...>, &compareDelegate<void, Parameters...>, DelegateAny(delegate) };
            EventBase::remove(subscriber);
        }
        // operator-= is an alias of remove.
        void operator-=(MyDelegate delegate) {
            remove(delegate);
        }
    };

    // Prioritised event base class, provides all aspects that don't depend on templated parameters.
    class PriorityEventBase : public IEvent {
    protected:
        struct Subscriber {
            GenericExecutor   executor;
            GenericComparator comparator;
            ui32              priority;
            DelegateAny       callback;

            bool operator==(const Subscriber& rhs) {
                return (this->executor   == rhs.executor)
                    && (this->comparator == rhs.comparator)
                    && (this->priority   == rhs.priority)
                    && this->comparator(this->callback, rhs.callback);
            }
            bool operator!=(const Subscriber& rhs) {
                return !(*this == rhs);
            }
        };
    public:
        // Define a comparison function for use in the multiset that stores subscribers.
        static bool _compare(const Subscriber& a, const Subscriber& b) {
            return a.priority < b.priority;
        };
    protected:
        using Subscribers = std::multiset<Subscriber, decltype(&_compare)>;
    public:
        // Constructors
        PriorityEventBase(Sender sender = nullptr) :
            IEvent(sender)
        { /* Empty */ }

        // Adds a subscriebr to the event.
        void add(Subscriber subscriber) {
            m_subscribers.insert(subscriber);
        }
        // operator+= is an alias of add.
        void operator+=(Subscriber subscriber) {
            add(subscriber);
        }

        // Removes a subscriber from the event (only the first instance found is removed).
        void remove(Subscriber subscriber) {
            const auto& it = m_subscribers.find(subscriber);
            if (it != m_subscribers.end()) {
                m_subscribers.erase(it);
            }
        }
        // operator-= is an alias of remove.
        void operator-=(Subscriber subscriber) {
            remove(subscriber);
        }
    protected:
        Subscribers m_subscribers = Subscribers(&PriorityEventBase::_compare);
    };
    // The prioritised event implementation, in particular in the case where some form of data may be returned
    // by the last invoked subscriber.
    template<typename DataReturnType, typename ...Parameters>
    class RPriorityEvent : public PriorityEventBase {
    protected:
        // Define the return type in the case that DataReturnType is NOT void.
        using _ReturnType = struct {
            bool shouldContinue;
            // Use std::conditional & std::is_void to avoid trying to have a void type field (disallowed).
            // Would prefer to use enable_if rather than conditional, but MSVC++ v120 and Clang 3.8 don't support SFINAE completely.
            typename std::conditional<std::is_void<DataReturnType>::value, void*, DataReturnType>::type data;
        };
    public:
        // If DataReturnType is void, define the ReturnType to be bool, else define it to be equal to _ReturnType.
        using ReturnType         = typename std::conditional<std::is_void<DataReturnType>::value, bool, _ReturnType>::type;
        using MyDelegate         = Delegate<ReturnType, Sender, Parameters...>;
        using MyPriorityDelegate = std::pair<ui32, MyDelegate>;
        using Executor           = ReturnType(*)(DelegateAny, Sender, Parameters...);

        // Constructors.
        RPriorityEvent(Sender sender = nullptr) :
            PriorityEventBase(sender)
        { /* EMPTY */ }

        // Triggers the event, invoking all subscribers.
        DataReturnType trigger(Parameters... parameters) {
            ReturnType ret = {};
            for (auto& subscriber : m_subscribers) {
                Executor executor = *(Executor*)&subscriber.executor;
                ret = executor(subscriber.callback, m_sender, parameters...);
                if (!ret.shouldContinue) break;
            }
            return ret.data;
        }
        // operator() is an alias of trigger.
        DataReturnType operator()(Parameters... parameters) {
            return trigger(parameters...);
        }

        // Adds a delegate subscriber to the event.
        void add(MyPriorityDelegate delegate) {
            Subscriber subscriber = { (GenericExecutor)&executeDelegate<ReturnType, Parameters...>, &compareDelegate<ReturnType, Parameters...>, delegate.first, DelegateAny(delegate.second) };
            PriorityEventBase::add(subscriber);
        }
        // operator+= is an alias of add.
        void operator+=(MyPriorityDelegate delegate) {
            add(delegate);
        }

        // Removes a delegate subscriber from the event (only the first instance found is removed).
        void remove(MyPriorityDelegate delegate) {
            Subscriber subscriber = { (GenericExecutor)&executeDelegate<ReturnType, Parameters...>, &compareDelegate<ReturnType, Parameters...>, delegate.first, DelegateAny(delegate.second) };
            PriorityEventBase::remove(subscriber);
        }
        // operator-= is an alias of remove.
        void operator-=(MyPriorityDelegate delegate) {
            remove(delegate);
        }
    };
    // Specialisation of prioritised event to handle void data return type.
    template<typename ...Parameters>
    class PriorityEvent : public RPriorityEvent<void, Parameters...> {
    public:
        // Constructors.
        PriorityEvent(Sender sender = nullptr) :
            RPriorityEvent<void, Parameters...>(sender)
        { /* EMPTY */ }

        using Executor = bool(*)(DelegateAny, Sender, Parameters...);
        // Triggers the event, invoking all subscribers.
        void trigger(Parameters... parameters) {
            for (auto& subscriber : RPriorityEvent<void, Parameters...>::m_subscribers) {
                Executor executor = *(Executor*)&subscriber.executor;
                if (!executor(subscriber.callback, RPriorityEvent<void, Parameters...>::m_sender, parameters...)) {
                    break;
                }
            }
        }
        // operator() is an alias of trigger.
        void operator()(Parameters... parameters) {
            trigger(parameters...);
        }
    };

    template<typename ReturnType, typename ...Parameters>
    ReturnType executeDelegate(DelegateAny callback, Sender sender, Parameters... parameters) {
        using MyDelegate = Delegate<ReturnType, Sender, Parameters...>;
        MyDelegate delegate = delegateAnyCast<MyDelegate>(callback);

        return delegate(sender, parameters...);
    }
    template<typename ReturnType, typename ...Parameters>
    bool compareDelegate(DelegateAny callback1, DelegateAny callback2) {
        using MyDelegate = Delegate<ReturnType, Sender, Parameters...>;
        MyDelegate delegate1 = delegateAnyCast<MyDelegate>(callback1);
        MyDelegate delegate2 = delegateAnyCast<MyDelegate>(callback2);

        return delegate1 == delegate2;
    }
}
namespace h = hemlock;
