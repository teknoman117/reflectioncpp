#ifndef __REFLECTIONCPP_METHOD_REFERENCERETURN_HPP__
#define __REFLECTIONCPP_METHOD_REFERENCERETURN_HPP__

namespace reflectioncpp
{
	// Specialization for non-void reference return type
	template<class T, class R, class... Args>
	class MethodImpl<T, R(Args...), 
					 typename std::enable_if<!std::is_void<R>::value && 
					 						 std::is_reference<R>::value
					 						 >::type
					 > : public Invokable
	{
	public:
	    typedef R (T::*PointerType)(Args...);
	    typedef std::reference_wrapper<typename std::remove_reference<R>::type> ReferenceType ;
	    PointerType methodPointer;

	private:
	    // Completed parameter pack, executes method
	    template <typename... Ts>
	    inline typename std::enable_if<sizeof...(Args) == sizeof...(Ts), R>::type
	    _Invoke(T *instance, std::vector<any>::iterator begin, Ts&&... ts)
	    {
	        // Invoke the method
	        return (instance->*methodPointer)(std::forward<Ts>(ts)...);
	    }

	    // Builds the parameter pack
	    template <typename... Ts>
	    inline typename std::enable_if<sizeof...(Args) != sizeof...(Ts), R>::type
	    _Invoke(T *instance, std::vector<any>::iterator begin, Ts&&... ts)
	    {
	        constexpr int index = sizeof...(Args) - sizeof...(Ts) - 1;
	        static_assert(index >= 0, "incompatible function parameters");

	        using type = typename std::tuple_element<index, std::tuple<Args...>>::type;

	        //cout << "Unpacking: " << Type<type>::Info().name << " @" << index << endl;
	        any& param = *(begin + index);

	        return _Invoke(instance, begin, any_cast<type>(param), std::forward<Ts>(ts)... );
	    }

	public:
	    MethodImpl(PointerType methodPointer)
	        : methodPointer(methodPointer)
	    {

	    }

	    any Invoke(std::vector<any>& params)
	    {
	    	if(params.size() < (sizeof...(Args) + 1))
	            throw std::out_of_range("too few parameters for method");

	    	T *instance = any_cast<T *>( *params.begin() );
			return any(ReferenceType(_Invoke(instance, params.begin() + 1)));
	    }
	};
}

#endif
