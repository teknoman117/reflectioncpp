#include <iostream>
#include <vector>
#include <exception>
#include <stdexcept>
#include <tuple>
#include <string>
#include <functional>

#include <boost/any.hpp>

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

class B
{
    size_t i = 0;
public:
    void DoShit()
    {
        i++;
        //std::cout << "ayy lmao" << std::endl;
    }
};

class Accessible
{
public:
    virtual boost::any Get(boost::any instance) const = 0;
    virtual void Set(boost::any instance, boost::any param) const = 0;
};

template <class T, typename M, typename Enabled = void>
class Member;

template <class T, typename M>
class Member<T, M, void> : public Accessible
{
    typedef M T::*PointerType;
    const PointerType memberPointer;

public:
    constexpr explicit Member(PointerType memberPointer)
        : memberPointer(memberPointer)
    {
    }

    void Set(boost::any instance, boost::any param) const
    {
        T* instancePtr = boost::any_cast<T *>(instance);
        instancePtr->*memberPointer = boost::any_cast<M>(param);
    }

    boost::any Get(boost::any instance) const
    {
        T* instancePtr = boost::any_cast<T *>(instance);
        return any(instancePtr->*memberPointer);
    }
};

constexpr static Member<A, int> cAmValue(&A::value);

constexpr static const Accessible *metaMembers[] =
{
    &cAmValue,
};

int main (int argc, char** argv)
{
    /*Invokable *derp      = new MethodImpl<A, int (int,float,string)>(&A::Derp);
    Invokable *getValue  = new MethodImpl<A, int& ()>(&A::GetValue);
    Invokable *increment = new MethodImpl<A, void ()>(&A::Increment);

    Invokable *factoryDefault    = new ConstructorImpl<A* ()>();
    Invokable *factoryInitialize = new ConstructorImpl<A* (int)>();

    Invokable *staticfunction = new MethodImpl<void, void ()>(&A::Herp);
    Invokable *static2 = new MethodImpl<void, int& ()>(&A::meaningoflife);*/

    Invokable *thingy = new MethodImpl<B, void(void)>(&B::DoShit);

    B instanceOfB;
    vector<any> thingyArgs = 
    {
        &instanceOfB,
    };


    for(size_t i = 0; i < 1000000; i++)
        thingy->Invoke(thingyArgs);





    //Accessible *member = new Member<A, int>(&A::value);
    /*const Accessible *member = metaMembers[0];

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

    // Test a simple test function
    any someInt = derp->Invoke(args);
    cout << "some int = " << any_cast<int>(someInt) << endl;

    // Test reference wrappers
    any someIntRef = getValue->InvokeWithList({test});
    int& valueRef = any_cast<Ref<int>::wrapper>(someIntRef).get();

    cout << "values before ref: " << valueRef << " " << test->value << endl;
    increment->InvokeWithList({test});
    cout << "values after ref: " << valueRef << " " << test->value << endl;

    cout << "accessor get: " << any_cast<int>(member->Get(test)) << endl;
    member->Set(test, 43);
    cout << "accessor get: " << any_cast<int>(member->Get(test)) << endl;

    staticfunction->Invoke();

    any mol = static2->Invoke();
    cout << "type = " << mol.type().name() << endl;

    delete test;*/

	return 0;
}
