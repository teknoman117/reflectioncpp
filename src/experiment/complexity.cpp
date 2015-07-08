#include <iostream>
//#include <vector>
#include <initializer_list>

#include <boost/any.hpp>
#include <boost/range/any_range.hpp>

#include "../../include/reflectioncpp/common.hpp"

using namespace std;

typedef boost::any_range<boost::any, boost::forward_traversal_tag, boost::any&, std::ptrdiff_t> parameter_range;

namespace reflectioncpp
{
    class Invokable
    {
    public:
        virtual boost::any Invoke( const parameter_range& params ) = 0;

        // Empty invoke call
        boost::any Invoke()
        {
            parameter_range empty;
            return Invoke(empty);
        }

        // initializer list
        inline boost::any InvokeInitializer(const std::initializer_list<boost::any>& initializer)
        {

            return Invoke(initializer);
        }
    };

    template<class, typename Enable = void>
    class ConstructorImpl;

    // Specialization for return type of void, but with arguments
    template<class T, class... Args>
    class ConstructorImpl<T* (Args...), void> : public Invokable
    {
        static_assert(pointer_attributes<T>::indirection() == 0, "Return type must be a single pointer to a valid class");

        // Completed parameter pack, executes method
        template <typename... Ts>
        inline typename std::enable_if<sizeof...(Args) == sizeof...(Ts), T*>::type
        _Construct(parameter_range::const_iterator it, Ts&&... ts)
        {
            // Invoke the constructor

            cout << "das new T" << endl;
            return new T(std::forward<Ts>(ts)...);
        }

        // Builds the parameter pack
        template <typename... Ts>
        inline typename std::enable_if<sizeof...(Args) != sizeof...(Ts), T*>::type
        _Construct(parameter_range::const_iterator it, Ts&&... ts)
        {
            // Use tuple_element to get the type of this parameter
            using type = typename std::tuple_element<
                                      sizeof...(Ts), 
                                      std::tuple<Args...>
                                  >::type;

            // Append the parameter
            boost::any& param = *it;
            return _Construct(++it, std::forward<Ts>(ts)..., boost::any_cast<type>(param) );
        }

    public:
        boost::any Invoke(const parameter_range& params) override
        {
            const size_t length = std::distance(params.begin(), params.end());
            if(sizeof...(Args) > length)
                throw std::out_of_range("too few parameters for constructor");

            return boost::any(_Construct(params.begin()));
        }
    };
}

// Some test class A
class ComplexNumber
{
    float real;
    float imaginary;
public:
    ComplexNumber(float real, float imaginary) : real(real), imaginary(imaginary) 
    {
    }

    void Display()
    {
        cout << "=> " << real << " + " << imaginary << "i" << endl;
    }
};

using namespace reflectioncpp;

// Test some complexity shit
int main (int argc, char **argv)
{
    Invokable *factory = new ConstructorImpl<ComplexNumber* (float, float)>();

    //vector<boost::any> args{1.0f, 3.0f};

    //ComplexNumber* aNumber = boost::any_cast<ComplexNumber *>(factory->Invoke(args));

    ComplexNumber* aNumber = boost::any_cast<ComplexNumber *>(
        factory->InvokeInitializer(
        {
            -1.0f, 
            3.0f
        })
    );

    aNumber->Display();

    return 0;
}
