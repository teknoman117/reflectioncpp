#ifndef __REFLECTIONCPP_CONSTRUCTOR_HPP__
#define __REFLECTIONCPP_CONSTRUCTOR_HPP__

#include <vector>
#include <boost/any.hpp>

#include "common.hpp"
#include "invokable.hpp"

namespace reflectioncpp
{
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

#endif
