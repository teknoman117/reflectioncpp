#include <iostream>
#include <vector>
#include <exception>
#include <stdexcept>
#include <tuple>
#include <string>
#include <functional>

#include <boost/any.hpp>
#include <boost/range/any_range.hpp>

#include <reflectioncpp/method.hpp>
#include <reflectioncpp/constructor.hpp>

using namespace reflectioncpp;
using namespace boost;
using namespace std;

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

    static int& meaningoflife()
    {
        static int otherv = 42;
        return otherv;
    }
};

int main (int argc, char** argv)
{
    Invokable *derp      = new MethodImpl<A, int (int,float,string)>(&A::Derp);
    Invokable *getValue  = new MethodImpl<A, int& ()>(&A::GetValue);
    Invokable *increment = new MethodImpl<A, void ()>(&A::Increment);

    Invokable *factoryDefault    = new ConstructorImpl<A* ()>();
    Invokable *factoryInitialize = new ConstructorImpl<A* (int)>();

    Invokable *staticfunction = new MethodImpl<void, void ()>(&A::Herp);
    Invokable *static2 = new MethodImpl<void, int& ()>(&A::meaningoflife);

    // Test the default constructor
    any aInstance = factoryDefault->Invoke();
    cout << "type = " << aInstance.type().name() << endl;

    A *test = any_cast<A *>(aInstance);
    test->value = 5;

    vector<any> args =
    {
        // first argument is now instance
        test,

        89,
        3.14159f,
        std::string("hello, world!"),
    };

    any_range<any, boost::forward_traversal_tag, any&, std::ptrdiff_t> range = args;
    any_range<any, boost::forward_traversal_tag, any&, std::ptrdiff_t>::iterator sIt = range.begin();
    any_range<any, boost::forward_traversal_tag, any&, std::ptrdiff_t>::iterator eIt = range.end();
    cout << "length = " << distance(sIt, eIt) << endl;

    // Test a simple test function
    any someInt = derp->Invoke(args);
    cout << "some int = " << any_cast<int>(someInt) << endl;

    // Test reference wrappers
    any someIntRef = getValue->Invoke(args);
    int& valueRef = any_cast<Ref<int>::wrapper>(someIntRef).get();

    cout << "values before ref: " << valueRef << " " << test->value << endl;
    valueRef = 7;
    cout << "values after ref: " << valueRef << " " << test->value << endl;

    staticfunction->Invoke();
    (*staticfunction)();

    any mol = static2->Invoke();
    cout << "type = " << mol.type().name() << endl;

    delete test;

	return 0;
}
