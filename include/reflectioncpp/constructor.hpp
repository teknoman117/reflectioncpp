#ifndef __REFLECTIONCPP_CONSTRUCTOR_HPP__
#define __REFLECTIONCPP_CONSTRUCTOR_HPP__

#include <vector>

#include "common.hpp"
#include "any.hpp"
#include "type.hpp"
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
        any Invoke(std::vector<any>& params) override
        {
            if(sizeof...(Args) > params.size())
                throw std::out_of_range("too few parameters for constructor");

            return any(_Construct(params.begin()));
        }
    };
}

#endif
