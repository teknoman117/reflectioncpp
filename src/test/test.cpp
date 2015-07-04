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
        virtual any Construct() = 0;
        virtual any Construct(std::vector<any>& params) = 0;
    };

    template<class, typename Enable = void>
    class ConstructorImpl;

    // Construct with no parameters
    template<class T, class... Args>
    class ConstructorImpl<T (Args...), typename std::enable_if<!sizeof...(Args)>::type> : public Constructor
    {
    public:
        any Construct()
        {
            return any(new T());
        }

        any Construct(std::vector<any>& params)
        {
            return Construct();
        }
    };

    // Specialization for return type of void, but with arguments
    template<class T, class... Args>
    class ConstructorImpl<T (Args...), typename std::enable_if<sizeof...(Args)>::type> : public Constructor
    {
        // Completed parameter pack, executes method
        template <typename... Ts>
        inline typename std::enable_if<sizeof...(Args) == sizeof...(Ts), T*>::type
        _Construct(std::vector<any>& v, Ts&&... ts)
        {
            // Invoke the constructor
            return new T(std::forward<Ts>(ts)...);
        }

        // Builds the parameter pack
        template <typename... Ts>
        inline typename std::enable_if<sizeof...(Args) != sizeof...(Ts), T*>::type
        _Construct(std::vector<any>& v, Ts&&... ts)
        {
            constexpr int index = sizeof...(Args) - sizeof...(Ts) - 1;
            static_assert(index >= 0, "incompatible function parameters");

            // Add the parameter to the function call 
            using type = typename std::tuple_element<index, std::tuple<Args...>>::type;
            any& param = *(std::begin(v) + index);
            return _Construct(v, any_cast<type>(param), std::forward<Ts>(ts)... );
        }

    public:
        any Construct()
        {
            throw std::out_of_range("vector too small for function");

            return any();
        }

        any Construct(std::vector<any>& params)
        {
            if(sizeof...(Args) > params.size())
                throw std::out_of_range("vector too small for function");

            return any(_Construct(params));
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
    vector<any> args =
    {
        89,
        3.14159f,
        std::string("hello, world!"),
    };

    Method *derp = new MethodImpl<A, int(int,float,string)>(&A::Derp);
    Method *getValue = new MethodImpl<A, int& ()>(&A::GetValue);

    Constructor *factoryDefault    = new ConstructorImpl<A ()>();
    Constructor *factoryInitialize = new ConstructorImpl<A (int)>();

    // Test the default constructor
    any aInstance = factoryDefault->Construct();
    cout << "type = " << aInstance.type().name << endl;

    A *test = any_cast<A *>(aInstance);
    test->value = 5;

    // Test a simple test function
    any someInt = derp->Invoke(test, args);
    cout << "some int = " << any_cast<int>(someInt) << endl;

    // Test reference wrappers
    any someIntRef = getValue->Invoke(test, args);
    int& valueRef = any_cast<Ref<int>::wrapper>(someIntRef).get();

    cout << "values before ref: " << valueRef << " " << test->value << endl;
    valueRef = 7;
    cout << "values after ref: " << valueRef << " " << test->value << endl;

    delete test;

	return 0;
}
