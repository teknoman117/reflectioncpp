#include <iostream>
#include <vector>
#include <exception>
#include <stdexcept>
#include <tuple>
#include <string>
#include <functional>

#include "any.hpp"
#include "type.hpp"
#include "method.hpp"

using namespace reflectioncpp;
using namespace std;



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

    A test;
    test.value = 5;

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
    int& valueRef = any_cast<Ref<int>::wrapper>(getValue->Invoke(&test, args)).get();

    cout << "values: " << valueRef << " " << test.value << endl;
    valueRef = 7;
    cout << "values: " << valueRef << " " << test.value << endl;

	return 0;
}
