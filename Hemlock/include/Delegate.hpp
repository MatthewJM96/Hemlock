#pragma once

// Designed to be a super fast delegate system for functions and callables of all kinds. Initially it was hoped
// to be able to wrap script functions inside of the delegate system in addition to C++ functions and callables.
// It has since been realised that this is an impossibility due to the nature of script environments; the parameters
// and return types are not statically typed for script callbacks. As a result of not needing to support unforeseen
// kinds of invokables, various optimisations have been possible - note that no heap allocation is necessary.
// As an extension, the design choices for the delegate system were made such that it was a trivial extension to also
// support creating functors - delegates that capture some scope.

namespace hemlock {
    // Trick to create a generic member function pointer for any class.
    struct TypelessMember {};

    // Define a stx::basic_any type that can store delegates on the stack.
    using DelegateAny     = stx::basic_any<5>;
    // Wrap the any cast functions, it's not great to wrap things, but one-liners should inline no matter the compiler.
    template<typename Type>
    inline Type delegateAnyCast(const DelegateAny& operand) {
        return stx::any_cast<Type, 5>(operand);
    }
    template<typename Type>
    inline Type delegateAnyCast(DelegateAny& operand) {
        return stx::any_cast<Type, 5>(operand);
    }
    template<typename Type>
    inline Type delegateAnyCast(DelegateAny&& operand) {
        return stx::any_cast<Type, 5>(operand);
    }
    template<typename Type>
    inline const Type* delegateAnyCast(const DelegateAny* operand) {
        return stx::any_cast<Type, 5>(operand);
    }
    template<typename Type>
    inline Type* delegateAnyCast(DelegateAny* operand) {
        return stx::any_cast<Type, 5>(operand);
    }

    // Empty base class of delegates to allow for non-templated polymorphic containers. (TODO(Matthew): Would anyone want this for delegates?)
    class DelegateBase {
        // Empty
    };
    // Interface for delegates.
    template<typename ReturnType, typename ...Parameters>
    class Delegate : public DelegateBase {
    protected:
        // Alias a whole bunch of types to make things more readable.
        using GenericObject         = const void*;
        using GenericFunction       = void*;
        using GenericMemberFunction = void(TypelessMember::*)();
        using Executor              = ReturnType(*)(GenericObject, GenericMemberFunction, Parameters...);
        using Deletor               = void(*)(GenericObject);
        using StaticFunction        = ReturnType(*)(Parameters...);
        template<typename SpecificClass>
        using MemberFunction        = ReturnType(SpecificClass::*)(Parameters...);
        template<typename SpecificClass>
        using ConstMemberFunction   = ReturnType(SpecificClass::*)(Parameters...) const;
    public:
        // Constructors.
        Delegate() :
            m_object(nullptr), m_function(nullptr), m_executor(nullptr)
        { /* Empty */ }
        Delegate(GenericObject object, GenericFunction function, Executor executor, Deletor deletor = nullptr) :
            m_object(object), m_function(function), m_executor(executor), m_deletor(deletor)
        { /* Empty */ }
        Delegate(GenericObject object, GenericMemberFunction function, Executor executor, Deletor deletor = nullptr) :
            m_object(object), m_memberFunction(function), m_executor(executor), m_deletor(deletor)
        { /* Empty */ }
        Delegate(const Delegate& delegate) :
            m_object(delegate.m_object), m_function(delegate.m_function), m_executor(delegate.m_executor), m_deletor(delegate.m_deletor)
        { /* EMPTY */ }
        Delegate(Delegate&& delegate) NOEXCEPT :
            m_object(delegate.m_object), m_function(delegate.m_function), m_executor(delegate.m_executor), m_deletor(delegate.m_deletor) {
            delegate.m_deletor = nullptr;
            delegate.m_object  = nullptr;
        }
        // Destructor.
        ~Delegate() {
            if (m_deletor) {
                m_deletor(m_object);
            }
            m_object   = nullptr;
            m_function = nullptr;
            m_executor = nullptr;
            m_deletor  = nullptr;
        }

        // Assignment operators.
        Delegate& operator=(const Delegate& delegate) {
            this->m_object   = delegate.m_object;
            this->m_function = delegate.m_function;
            this->m_executor = delegate.m_executor;
            this->m_deletor  = delegate.m_deletor;
            return *this;
        }
        Delegate& operator=(Delegate&& delegate) {
            this->m_object   = delegate.m_object;
            this->m_function = delegate.m_function;
            this->m_executor = delegate.m_executor;
            this->m_deletor  = delegate.m_deletor;
            delegate.m_deletor = nullptr;
            delegate.m_object  = nullptr;
            return *this;
        }

        // Trigger the delegated function with the provided arguments.
        ReturnType invoke(Parameters... parameters) const {
            return m_executor(m_object, m_memberFunction, parameters...);
        }
        // operator() is an alias of invoke.
        ReturnType operator()(Parameters... parameters) const {
            return m_executor(m_object, m_memberFunction, parameters...);
        }

        // Equality and not-equality operators to compare delegates.
        bool operator==(const Delegate& delegate) const {
            return (m_object   == delegate.m_object)
                && (m_function == delegate.m_function)
                && (m_executor == delegate.m_executor);
        }
        bool operator!=(const Delegate& delegate) const {
            return !(*this == delegate);
        }

        // Creation functions for wrapping various types of functions.
        // Static functions.
        static Delegate create(StaticFunction function) {
            return Delegate(nullptr, (GenericFunction)function, &execute);
        }
        /// Enable create functions only if SpecificClass is a class.
        // Non-const member functions.
        template<typename SpecificClass, typename = typename std::enable_if<std::is_class<SpecificClass>::value>::type>
        static Delegate create(SpecificClass* object, MemberFunction<SpecificClass> function) {
            return Delegate(object, (GenericMemberFunction)function, &executeWithObject<SpecificClass>);
        }
        template<typename SpecificClass, typename = typename std::enable_if<std::is_class<SpecificClass>::value>::type>
        static Delegate createCopy(SpecificClass& object, MemberFunction<SpecificClass> function) {
            return Delegate(new SpecificClass(object), (GenericMemberFunction)function, &executeWithObject<SpecificClass>, &destroy<SpecificClass>);
        }
        // Const member functions.
        template<typename SpecificClass, typename = typename std::enable_if<std::is_class<SpecificClass>::value>::type>
        static Delegate create(SpecificClass const* object, ConstMemberFunction<SpecificClass> function) {
            return Delegate(object, (GenericMemberFunction)function, &executeWithObject<SpecificClass>);
        }
        template<typename SpecificClass, typename = typename std::enable_if<std::is_class<SpecificClass>::value>::type>
        static Delegate createCopy(SpecificClass const& object, ConstMemberFunction<SpecificClass> function) {
            return Delegate(new SpecificClass(const_cast<SpecificClass>(object)), (GenericMemberFunction)function, &executeWithObject<SpecificClass>, &destroy<SpecificClass>);
        }
    protected:
        // Execution routine for static functions.
        static ReturnType execute(GenericObject object, GenericMemberFunction function, Parameters... parameters) {
            // Use type punning to get function pointer kind desired.
            // Warning: Technically this can fail dependent on compiler, compilation options etc., but on all tested compilers this works as expected.
            //          We are initially storing the static function pointer that we are casting out of the member function pointer in the static function
            //          pointer union field, so it is very likely this will hold true for most, if not all, scenarios.
            StaticFunction operation = *(StaticFunction*)&function;

            return operation(parameters...);
        }
        // Execution routine for member functions (const or not).
        template<typename SpecificClass, typename = typename std::enable_if<std::is_class<SpecificClass>::value>::type>
        static ReturnType executeWithObject(GenericObject object, GenericMemberFunction function, Parameters... parameters) {
            // Case void (function) pointer to member function pointer & void (object) pointer to object pointer, then invoke.
            MemberFunction<SpecificClass> operation = (MemberFunction<SpecificClass>)function;
            SpecificClass*                   target = (SpecificClass*)object;

            return (target->*operation)(parameters...);
        }
        // Deletion routine for functors.
        template<typename SpecificClass>
        static void destroy(GenericObject object) {
            delete (SpecificClass*)object;
        }

        GenericObject m_object;
        union {
            GenericFunction       m_function;
            GenericMemberFunction m_memberFunction;
        };
        Executor m_executor;
        Deletor  m_deletor;
    };



    //// makeDelegate routines.
    // Static functions.
    template<typename ReturnType, typename ...Parameters>
    Delegate<ReturnType, Parameters...> makeDelegate(ReturnType(*function)(Parameters...)) {
        return Delegate<ReturnType, Parameters...>::create(function);
    }
    /// Enable make functions only if SpecificClass is a class.
    // Non-const member functions.
    template<typename SpecificClass, typename ReturnType, typename ...Parameters, typename = typename std::enable_if<std::is_class<SpecificClass>::value>::type>
    Delegate<ReturnType, Parameters...> makeDelegate(SpecificClass* object, ReturnType(SpecificClass::*function)(Parameters...)) {
        return Delegate<ReturnType, Parameters...>::template create<SpecificClass>(object, function);
    }
    // Const member functions.
    template<typename SpecificClass, typename ReturnType, typename ...Parameters, typename = typename std::enable_if<std::is_class<SpecificClass>::value>::type>
    Delegate<ReturnType, Parameters...> makeDelegate(SpecificClass const* object, ReturnType(SpecificClass::*function)(Parameters...) const) {
        return Delegate<ReturnType, Parameters...>::template create<SpecificClass>(object, function);
    }
    // // Invokable classes.
    // template<typename SpecificClass, typename ReturnType, typename ...Parameters, typename = typename std::enable_if<std::is_class<SpecificClass>::value>::type>
    // Delegate<ReturnType, Parameters...> makeDelegate(SpecificClass& object) {
    //     using Function    = ReturnType(SpecificClass::*)(Parameters...) const;
    //     Function function = &SpecificClass::operator();
    //     return Delegate<ReturnType, Parameters...>::template create<SpecificClass>(&object, function);
    // }

    // makeFunctor routines.
    // Non-const member functions.
    template<typename SpecificClass, typename ReturnType, typename ...Parameters, typename = typename std::enable_if<std::is_class<SpecificClass>::value>::type>
    Delegate<ReturnType, Parameters...> makeFunctor(SpecificClass& object, ReturnType(SpecificClass::*function)(Parameters...)) {
        return Delegate<ReturnType, Parameters...>::template createCopy<SpecificClass>(object, function);
    }
    // Const member functions.
    template<typename SpecificClass, typename ReturnType, typename ...Parameters, typename = typename std::enable_if<std::is_class<SpecificClass>::value>::type>
    Delegate<ReturnType, Parameters...> makeFunctor(SpecificClass const& object, ReturnType(SpecificClass::*function)(Parameters...) const) {
        return Delegate<ReturnType, Parameters...>::template createCopy<SpecificClass>(object, function);
    }
}
namespace h = hemlock;
