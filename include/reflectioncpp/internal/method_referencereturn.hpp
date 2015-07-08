#ifndef __REFLECTIONCPP_METHOD_REFERENCERETURN_HPP__
#define __REFLECTIONCPP_METHOD_REFERENCERETURN_HPP__

namespace reflectioncpp
{
	// Specialization for non-void reference return type
	template<class T, class R, class... Args>
	class MethodImpl<T, R(Args...), 
					 typename std::enable_if<!std::is_void<R>::value && 
					 						 !std::is_void<T>::value &&
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
	    _Invoke(T *instance, parameter_range::const_iterator it, Ts&&... ts) const
	    {
	        // Invoke the method
	        return (instance->*methodPointer)(std::forward<Ts>(ts)...);
	    }

	    // Builds the parameter pack
	    template <typename... Ts>
	    inline typename std::enable_if<sizeof...(Args) != sizeof...(Ts), R>::type
	    _Invoke(T *instance, parameter_range::const_iterator it, Ts&&... ts) const
	    {
	        using type = typename std::tuple_element<sizeof...(Ts), std::tuple<Args...>>::type;
	        boost::any& param = *(it++);
	        return _Invoke(instance, it, std::forward<Ts>(ts)..., boost::any_cast<type>(param));
	    }

	public:
	    MethodImpl(PointerType methodPointer)
	        : methodPointer(methodPointer)
	    {
	    }

	    boost::any Invoke() const override
	    {
            throw std::out_of_range("too few parameters for constructor");
			return boost::any();
	    }

	    boost::any Invoke(const parameter_range& params) const override
	    {
            const size_t length = std::distance(params.begin(), params.end());
            if((sizeof...(Args) + 1) > length)
                throw std::out_of_range("too few parameters for constructor");

            parameter_range::iterator it = params.begin();
	    	T *instance = boost::any_cast<T *>( *(it++) );
			return boost::any(ReferenceType(_Invoke(instance, it)));
	    }
	};

		// Specialization for non-void reference return type
	template<class T, class R, class... Args>
	class MethodImpl<T, R(Args...), 
					 typename std::enable_if<!std::is_void<R>::value && 
					 						 std::is_void<T>::value &&
					 						 std::is_reference<R>::value
					 						 >::type
					 > : public Invokable
	{
	public:
	    typedef R (*PointerType)(Args...);
	    typedef std::reference_wrapper<typename std::remove_reference<R>::type> ReferenceType ;
	    PointerType methodPointer;

	private:
	    // Completed parameter pack, executes method
	    template <typename... Ts>
	    inline typename std::enable_if<sizeof...(Args) == sizeof...(Ts), R>::type
	    _Invoke(parameter_range::const_iterator it, Ts&&... ts) const
	    {
	        // Invoke the method
	        return (*methodPointer)(std::forward<Ts>(ts)...);
	    }

	    // Builds the parameter pack
	    template <typename... Ts>
	    inline typename std::enable_if<sizeof...(Args) != sizeof...(Ts), R>::type
	    _Invoke(parameter_range::const_iterator it, Ts&&... ts) const
	    {
	        using type = typename std::tuple_element<sizeof...(Ts), std::tuple<Args...>>::type;
	        boost::any& param = *(it++);
	        return _Invoke(it, std::forward<Ts>(ts)..., boost::any_cast<type>(param));
	    }

	public:
	    MethodImpl(PointerType methodPointer)
	        : methodPointer(methodPointer)
	    {
	    }


	    boost::any Invoke() const override
	    {
	    	if(sizeof...(Args))
	    		throw std::out_of_range("too few parameters for method");

			return boost::any(ReferenceType(_Invoke(parameter_range::iterator())));
	    }

	    boost::any Invoke(const parameter_range& params) const override
	    {
            const size_t length = std::distance(params.begin(), params.end());
            if(sizeof...(Args) > length)
                throw std::out_of_range("too few parameters for constructor");

			return boost::any(ReferenceType(_Invoke(params.begin())));
	    }
	};
}

#endif
