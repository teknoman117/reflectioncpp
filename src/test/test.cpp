#include <iostream>
#include <vector>
#include <exception>
#include <stdexcept>
#include <tuple>
#include <string>
#include <functional>

#include <reflectioncpp/any.hpp>
#include <reflectioncpp/type.hpp>
#include <reflectioncpp/method.hpp>

using namespace reflectioncpp;
using namespace std;

namespace reflectioncpp
{
    class Constructor
    {
    public:
        virtual any Construct(std::vector<any>& params) = 0;
    };

    template<class, class, typename Enable = void>
    class ConstructorImpl;

    // Construct with no parameters
    template<class T, class R, class... Args>
    class ConstructorImpl<T, R(Args...), void> : public Constructor
    {
        T* _Construct()
        {
            return new T();
        }

    public:
        any Construct(std::vector<any>& params)
        {
            return any(_Construct());
        }
    };

    // Specialization for return type of void, but with arguments
    /*template<class T, class... Args>
    class ConstructorImpl<T, Args...> : public Constructor
    {
        // Completed parameter pack, executes method
        template <typename... Ts>
        inline typename std::enable_if<sizeof...(Args) == sizeof...(Ts), R>::type
        _Construct(std::vector<any>& v, Ts&&... ts)
        {
            if(sizeof...(Ts) > v.size())
                throw std::out_of_range("vector too small for function");

            // Invoke the method
            return (((T*) instance)->*methodPointer)(std::forward<Ts>(ts)...);
        }

        // Builds the parameter pack
        template <typename... Ts>
        inline typename std::enable_if<sizeof...(Args) != sizeof...(Ts), R>::type
        _Invoke(T *instance, std::vector<any>& v, Ts&&... ts)
        {
            constexpr int index = sizeof...(Args) - sizeof...(Ts) - 1;
            static_assert(index >= 0, "incompatible function parameters");

            using type = typename std::tuple_element<index, std::tuple<Args...>>::type;

            //cout << "Unpacking: " << Type<type>::Info().name << " @" << index << endl;
            any& param = *(std::begin(v) + index);

            return _Invoke(instance, v, any_cast<type>(param), std::forward<Ts>(ts)... );
        }

    public:
        MethodImpl(PointerType methodPointer)
            : methodPointer(methodPointer)
        {

        }

        any Invoke(void *instance, std::vector<any>& params)
        {
            T *concreteInstance = reinterpret_cast<T *>(instance);
            _Invoke(concreteInstance, params);
            return any();
        }
    };*/
}

class A
{
public:
    int value;

public:
    A() : value(0) {}

    //void Set(int a) {value = a;}
    /*int Get() {return value;}
    void Increment() {value++;};

    static void Herp()
    {
        std::cout << "Herp Derp" << std::endl;
    }*/
    int Derp(int a, float b, string c)
    {
    	cout << a << " " << b << " " << c << endl;
    	return a;
    }

    int& GetValue()
    {
    	return value;
    }

    /*int operator()()
    {
        return value;
    }*/
};

DEFINE_TYPE(A, A);
DEFINE_TYPE(string, string);

int main (int argc, char** argv)
{
    //MethodImpl<A, void(int,float,string)> *derp = new MethodImpl<A, void(int,float,string)> (&A::Derp);
    /*MethodImpl<A, void(int)>  _set(&A::Set);
    MethodImpl<A, int()>  _get(&A::Get);
    MethodImpl<A, void()> _increment(&A::Increment);
    MethodImpl<A, int()> _function(&A::operator());*/

    Method *derp = new MethodImpl<A, int(int,float,string)>(&A::Derp);
    Method *getValue = new MethodImpl<A, int& ()>(&A::GetValue);

    Constructor *factory = new ConstructorImpl<A, A* ()>();

    std::vector<any> cparm;
    A *test = any_cast<A *>(factory->Construct(cparm));
    test->value = 5;

    vector<any> args =
    {
    	89,
    	3.14159f,
    	std::string("hello, world!"),
    };

    //cout << "value: " << _get.Invoke((void *) &test) << endl;
    /*_set.Invoke((void *) &test, args);

    _derp.Invoke((void *) &test, args);

    cout << "value: " << _get.Invoke((void *) &test) << endl;
    _increment.Invoke((void *) &test);
    cout << "value: " << _function.Invoke((void *) &test) << endl;*/

    any someInt = derp->Invoke(&test, args);
    cout << "some int = " << any_cast<int>(someInt) << endl;
    int& valueRef = any_cast<Ref<int>::wrapper>(getValue->Invoke(test, args)).get();

    cout << "values: " << valueRef << " " << test->value << endl;
    valueRef = 7;
    cout << "values: " << valueRef << " " << test->value << endl;

	return 0;
}
