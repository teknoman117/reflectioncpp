// Fucking magic i tell you

#include <iostream>
#include <vector>
#include <exception>
#include <stdexcept>
#include <tuple>
#include <string>

#include "Variant.hpp"

using namespace std;

// Method invoke base class
/*class Method
{
public:
    // Can we use templates to handle the void return case
    virtual Variant Invoke(void *instance, vector<Variant> params) const = 0;
    virtual Variant Invoke(void *instance) const = 0;
};*/

// Method invoke implementation
template<class,class>
class MethodImpl;

template<class T, class R, class... Args>
class MethodImpl<T, R(Args...)>
{
public:
    typedef R (T::*PointerType)(Args...);
    PointerType methodPointer;
public:

    // Internal invoker methods
    R Invoke(void *instance)
    {
        // Invoke the method
        return (((T*) instance)->*methodPointer)();
    }

    // Completed parameter pack, executes method
    template <typename... Ts>
    typename enable_if<sizeof...(Args) == sizeof...(Ts), R>::type
    Invoke(void *instance, const vector<Variant>& v, Ts&&... ts)
    {
        if(sizeof...(Ts) > v.size())
            throw out_of_range("vector too small for function");

        // Invoke the method
        return (((T*) instance)->*methodPointer)(std::forward<Ts>(ts)...);
    }

    // Builds the parameter pack
    template <typename... Ts>
    typename enable_if<sizeof...(Args) != sizeof...(Ts), R>::type
    Invoke(void *instance, const vector<Variant>& v, Ts&&... ts)
    {
        const int index = sizeof...(Args) - sizeof...(Ts) - 1;
        static_assert(index >= 0, "incompatible function parameters");

        using type = typename tuple_element<index, tuple<Args...>>::type;

        return Invoke(instance, v, (std::begin(v) + index)->GetValue<type>(), std::forward(ts)... );
    }

    // Add code (using enable_if<is_void stuff) to marshall void calls to variant returns
    /*template <typename U = R>
    typename enable_if<!is_void<U>::value, Variant>::type
    InvokeInternal(void *instance, const vector<Variant>& params)
    {

    }*/

public:
    MethodImpl(PointerType methodPointer)
        : methodPointer(methodPointer)
    {

    }

    /// DO NOT USE YET (VERY BAD POINTER TO LOCAL THING) JUST HERE SO SHIT COMPILES
    /*Variant Invoke(void *instance, vector<Variant> params) const
    {
        // Invoke the method
        R result = Invoke(instance, params);
        return Variant(&result);
    }

    /// DO NOT USE YET (VERY BAD POINTER TO LOCAL THING) JUST HERE SO SHIT COMPILES
    Variant Invoke(void *instance) const
    {
        // Invoke the method
        R result = Invoke(instance);
        return Variant(&result);
    }*/
};

class A
{
public:
    int value;

public:
    A() : value(0) {}

    void Set(int a) {value = a;}
    int Get() {return value;}
    void Increment() {value++;};

    static void Herp()
    {
        std::cout << "Herp Derp" << std::endl;
    }

    int operator()()
    {
        return value;
    }
};

void Derp()
{
    std::cout << "Derp Derp" << std::endl;
}


int main ()
{
    int a = 5;
    int b = 7;

    Variant aP(&a);
    Variant bP(&b);

    MethodImpl<A, void(int)>  _set(&A::Set);
    MethodImpl<A, int()>  _get(&A::Get);
    MethodImpl<A, void()> _increment(&A::Increment);

    MethodImpl<A, int()> _function(&A::operator());

    // This is a thing?
    MethodImpl<A, void(int)>::PointerType pointer = &A::Set;
    MethodImpl<A, void()>::PointerType p2 = reinterpret_cast<MethodImpl<A, void()>::PointerType>(pointer);

    A test;

    int A::* derp = &A::value;

    int& hurr = test.*derp;

    int *herp = &(test.*derp);

    void (*durr)(void) = &A::Herp;

    cout << "value: " << _get.Invoke((void *) &test) << endl;

    const vector<Variant> args = {bP};
    _set.Invoke((void *) &test, args);

    cout << "value: " << _get.Invoke((void *) &test) << endl;
    _increment.Invoke((void *) &test);
    cout << "value: " << _function.Invoke((void *) &test) << endl;



    return 0;
}
