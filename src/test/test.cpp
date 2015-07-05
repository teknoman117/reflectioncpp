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
    template<class, typename Enable = void>
    class ConstructorImpl;

    // Construct with no parameters
    template<class T, class... Args>
    class ConstructorImpl<T (Args...), typename std::enable_if<!sizeof...(Args)>::type> : public Invokable
    {
    public:
        any Invoke(std::vector<any>& params)
        {
            return any(new T());
        }
    };

    // Specialization for return type of void, but with arguments
    template<class T, class... Args>
    class ConstructorImpl<T (Args...), typename std::enable_if<sizeof...(Args)>::type> : public Invokable
    {
        // Completed parameter pack, executes method
        template <typename... Ts>
        inline typename std::enable_if<sizeof...(Args) == sizeof...(Ts), T*>::type
        _Construct(std::vector<any>::iterator begin, Ts&&... ts)
        {
            // Invoke the constructor
            return new T(std::forward<Ts>(ts)...);
        }

        // Builds the parameter pack
        template <typename... Ts>
        inline typename std::enable_if<sizeof...(Args) != sizeof...(Ts), T*>::type
        _Construct(std::vector<any>::iterator begin, Ts&&... ts)
        {
            constexpr int index = sizeof...(Args) - sizeof...(Ts) - 1;
            static_assert(index >= 0, "incompatible function parameters");

            // Add the parameter to the function call 
            using type = typename std::tuple_element<index, std::tuple<Args...>>::type;
            any& param = *(begin + index);
            return _Construct(begin, any_cast<type>(param), std::forward<Ts>(ts)... );
        }

    public:
        any Invoke(std::vector<any>& params)
        {
            if(sizeof...(Args) > params.size())
                throw std::out_of_range("too few parameters for constructor");

            return any(_Construct(params.begin()));
        }
    };
}

class A
{
public:
    int value;

public:
    A() : value(0) {}
    A(int i) : value(i) {}

    //void Set(int a) {value = a;}
    int Get() {return value;}
    void Increment() {value++;};

    static void Herp()
    {
        std::cout << "Herp Derp" << std::endl;
    }

    int Derp(int a, float b, string c)
    {
    	cout << a << " " << b << " " << c << endl;
    	return a;
    }

    int& GetValue()
    {
    	return value;
    }

    int operator()()
    {
        return value;
    }
};

DEFINE_TYPE(A, A);
DEFINE_TYPE(string, string);

int main (int argc, char** argv)
{
    Invokable *derp      = new MethodImpl<A, int (int,float,string)>(&A::Derp);
    Invokable *getValue  = new MethodImpl<A, int& ()>(&A::GetValue);
    Invokable *increment = new MethodImpl<A, void ()>(&A::Increment);

    Invokable *factoryDefault    = new ConstructorImpl<A ()>();
    Invokable *factoryInitialize = new ConstructorImpl<A (int)>();

    // Test the default constructor
    any aInstance = factoryDefault->Invoke();
    cout << "type = " << aInstance.type().name << endl;

    A *test = any_cast<A *>(aInstance);
    test->value = 5;

    vector<any> args =
    {
        test,
        89,
        3.14159f,
        std::string("hello, world!"),
    };

    // Test a simple test function
    any someInt = derp->Invoke(args);
    cout << "some int = " << any_cast<int>(someInt) << endl;

    // Test reference wrappers
    any someIntRef = getValue->Invoke(args);
    int& valueRef = any_cast<Ref<int>::wrapper>(someIntRef).get();

    cout << "values before ref: " << valueRef << " " << test->value << endl;
    valueRef = 7;
    cout << "values after ref: " << valueRef << " " << test->value << endl;

    delete test;

	return 0;
}
