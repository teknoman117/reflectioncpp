#include <iostream>
#include <vector>
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
        virtual boost::any Invoke(const parameter_range& params) const = 0;
        virtual boost::any Invoke() const = 0;

        // Invoke with initializer list
        inline boost::any InvokeWithList(const std::initializer_list<boost::any>& initializer) const
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
        _Construct(parameter_range::const_iterator it, Ts&&... ts) const
        {
            // Invoke the constructor
            return new T(std::forward<Ts>(ts)...);
        }

        // Builds the parameter pack
        template <typename... Ts>
        inline typename std::enable_if<sizeof...(Args) != sizeof...(Ts), T*>::type
        _Construct(parameter_range::const_iterator it, Ts&&... ts) const
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
        boost::any Invoke(const parameter_range& params) const override
        {
            const size_t length = std::distance(params.begin(), params.end());
            if(sizeof...(Args) > length)
                throw std::out_of_range("too few parameters for constructor");

            return boost::any(_Construct(params.begin()));
        }

        boost::any Invoke() const override
        {
            if(sizeof...(Args) != 0)
                throw std::out_of_range("too few parameters for constructor");

            return boost::any(_Construct(parameter_range::iterator()));
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

    ComplexNumber(float real, float imaginary, float *test) : real(real), imaginary(imaginary) 
    {
        test[0] = real;
        test[1] = imaginary;
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
    Invokable& factory_a = *(new ConstructorImpl<ComplexNumber* (float, float)>());
    Invokable& factory_b = *(new ConstructorImpl<ComplexNumber* (float, float, float*)>());
    float *someFloats = new float[2];

    vector<boost::any> args{5.0f, -9.0f};

    ComplexNumber* aNumber1 = boost::any_cast<ComplexNumber *>(factory_a.Invoke(args));

    ComplexNumber* aNumber2 = boost::any_cast<ComplexNumber *>(
        factory_b.InvokeWithList(
        {
            -1.0f, 
            3.0f,
            someFloats
        })
    );

    cout << "some floats = " << someFloats[0] << " " << someFloats[1] << endl;

    aNumber1->Display();
    aNumber2->Display();

    delete[] someFloats;

    return 0;
}
