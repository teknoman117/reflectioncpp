#ifndef __REFLECTIONCPP_METHOD_HPP__
#define __REFLECTIONCPP_METHOD_HPP__

#include <iostream>
#include <vector>
#include <tuple>
#include <exception>
#include <stdexcept>
#include <functional>

#include "type.hpp"
#include "any.hpp"

namespace reflectioncpp
{
	// Method invoke base class
	class Method
	{
	public:
	    // Can we use templates to handle the void return case
	    virtual any Invoke(void *instance, std::vector<any>& params) = 0;
	};

	// Method invoke implementation
	template<class,class, typename Enable = void>
	class MethodImpl;

	// Specialization for return type of void, no arguments
	template<class T, class R, class... Args>
	class MethodImpl<T, R(Args...), typename std::enable_if<std::is_void<R>::value && sizeof...(Args) == 0>::type> : public Method
	{
	public:
	    typedef R (T::*PointerType)(Args...);
	    PointerType methodPointer;

	private:
	    // Invoke for zero parameters
	    void _Invoke(T *instance)
	    {
	        // Invoke the method
	        return (((T*) instance)->*methodPointer)();
	    }

	public:
	    MethodImpl(PointerType methodPointer)
	        : methodPointer(methodPointer)
	    {

	    }

	    any Invoke(void *instance, std::vector<any>& params)
	    {
	    	T *concreteInstance = reinterpret_cast<T *>(instance);
			_Invoke(concreteInstance);
			return any();
	    }
	};

	// Specialization for return type of void, but with arguments
	template<class T, class R, class... Args>
	class MethodImpl<T, R(Args...), typename std::enable_if<std::is_void<R>::value && sizeof...(Args) != 0>::type> : public Method
	{
	public:
	    typedef R (T::*PointerType)(Args...);
	    PointerType methodPointer;

	private:
	    // Completed parameter pack, executes method
	    template <typename... Ts>
	    inline typename std::enable_if<sizeof...(Args) == sizeof...(Ts), R>::type
	    _Invoke(T *instance, std::vector<any>& v, Ts&&... ts)
	    {
	        if(sizeof...(Ts) > v.size())
	            throw std::out_of_range("vector too small for function");

	        // Invoke the method
	        return (((T*) instance)->*methodPointer)(std::forward<Ts>(ts)...);
	    }

	    // Builds the parameter pack
	    template <typename... Ts>
	    inline typename std::enable_if<sizeof...(Args) != sizeof...(Ts), R>::type
	    _Invoke(T *instance, std::vector<any>& v, Ts&&... ts)
	    {
	        constexpr int index = sizeof...(Args) - sizeof...(Ts) - 1;
	        static_assert(index >= 0, "incompatible function parameters");

	        using type = typename std::tuple_element<index, std::tuple<Args...>>::type;

	        //cout << "Unpacking: " << Type<type>::Info().name << " @" << index << endl;
	        any& param = *(std::begin(v) + index);

	        return _Invoke(instance, v, any_cast<type>(param), std::forward<Ts>(ts)... );
	    }

	public:
	    MethodImpl(PointerType methodPointer)
	        : methodPointer(methodPointer)
	    {

	    }

	    any Invoke(void *instance, std::vector<any>& params)
	    {
	    	T *concreteInstance = reinterpret_cast<T *>(instance);
			_Invoke(concreteInstance, params);
			return any();
	    }
	};

	// Specialization for non-void return type, no arguments
	template<class T, class R, class... Args>
	class MethodImpl<T, R(Args...), 
					 typename std::enable_if<!std::is_void<R>::value && 
					 						 !std::is_reference<R>::value &&
					 						 sizeof...(Args) == 0
					 						 >::type
					 > : public Method
	{
	public:
	    typedef R (T::*PointerType)(Args...);
	    PointerType methodPointer;

	private:
	    R _Invoke(T *instance)
	    {
	        // Invoke the method
	        return (((T*) instance)->*methodPointer)();
	    }

	public:
	    MethodImpl(PointerType methodPointer)
	        : methodPointer(methodPointer)
	    {

	    }

	    any Invoke(void *instance, std::vector<any>& params)
	    {
	    	T *concreteInstance = reinterpret_cast<T *>(instance);
			return any(_Invoke(concreteInstance));
	    }
	};

	// Specialization for non-void return type, has arguments
	template<class T, class R, class... Args>
	class MethodImpl<T, R(Args...), 
					 typename std::enable_if<!std::is_void<R>::value && 
					 						 !std::is_reference<R>::value &&
					 						 sizeof...(Args) != 0
					 						 >::type
					 > : public Method
	{
	public:
	    typedef R (T::*PointerType)(Args...);
	    PointerType methodPointer;

	private:
	    // Completed parameter pack, executes method
	    template <typename... Ts>
	    inline typename std::enable_if<sizeof...(Args) == sizeof...(Ts), R>::type
	    _Invoke(T *instance, std::vector<any>& v, Ts&&... ts)
	    {
	        if(sizeof...(Ts) > v.size())
	            throw std::out_of_range("vector too small for function");

	        // Invoke the method
	        return (((T*) instance)->*methodPointer)(std::forward<Ts>(ts)...);
	    }

	    // Builds the parameter pack
	    template <typename... Ts>
	    inline typename std::enable_if<sizeof...(Args) != sizeof...(Ts), R>::type
	    _Invoke(T *instance, std::vector<any>& v, Ts&&... ts)
	    {
	        constexpr int index = sizeof...(Args) - sizeof...(Ts) - 1;
	        static_assert(index >= 0, "incompatible function parameters");

	        using type = typename std::tuple_element<index, std::tuple<Args...>>::type;

	        //cout << "Unpacking: " << Type<type>::Info().name << " @" << index << endl;
	        any& param = *(std::begin(v) + index);

	        return _Invoke(instance, v, any_cast<type>(param), std::forward<Ts>(ts)... );
	    }

	public:
	    MethodImpl(PointerType methodPointer)
	        : methodPointer(methodPointer)
	    {

	    }

	    any Invoke(void *instance, std::vector<any>& params)
	    {
	    	T *concreteInstance = reinterpret_cast<T *>(instance);
			return any(_Invoke(concreteInstance, params));
	    }
	};

	// Specialization for non-void reference return type, no arguments
	template<class T, class R, class... Args>
	class MethodImpl<T, R(Args...), 
					 typename std::enable_if<!std::is_void<R>::value && 
					 						 std::is_reference<R>::value &&
					 						 sizeof...(Args) == 0
					 						 >::type
					 > : public Method
	{
	public:
	    typedef R (T::*PointerType)(Args...);
	    typedef std::reference_wrapper<typename std::remove_reference<R>::type> ReferenceType ;
	    PointerType methodPointer;

	private:
	    R _Invoke(T *instance)
	    {
	        // Invoke the method
	        return (((T*) instance)->*methodPointer)();
	    }

	public:
	    MethodImpl(PointerType methodPointer)
	        : methodPointer(methodPointer)
	    {

	    }

	    any Invoke(void *instance, std::vector<any>& params)
	    {
	    	T *concreteInstance = reinterpret_cast<T *>(instance);
			return any(ReferenceType(_Invoke(concreteInstance)));
	    }
	};

	// Specialization for non-void reference return type, has arguments
	template<class T, class R, class... Args>
	class MethodImpl<T, R(Args...), 
					 typename std::enable_if<!std::is_void<R>::value && 
					 						 std::is_reference<R>::value &&
					 						 sizeof...(Args) != 0
					 						 >::type
					 > : public Method
	{
	public:
	    typedef R (T::*PointerType)(Args...);
	    typedef std::reference_wrapper<typename std::remove_reference<R>::type> ReferenceType ;
	    PointerType methodPointer;

	private:
	    // Completed parameter pack, executes method
	    template <typename... Ts>
	    inline typename std::enable_if<sizeof...(Args) == sizeof...(Ts), R>::type
	    _Invoke(T *instance, std::vector<any>& v, Ts&&... ts)
	    {
	        if(sizeof...(Ts) > v.size())
	            throw std::out_of_range("vector too small for function");

	        // Invoke the method
	        return (((T*) instance)->*methodPointer)(std::forward<Ts>(ts)...);
	    }

	    // Builds the parameter pack
	    template <typename... Ts>
	    inline typename std::enable_if<sizeof...(Args) != sizeof...(Ts), R>::type
	    _Invoke(T *instance, std::vector<any>& v, Ts&&... ts)
	    {
	        constexpr int index = sizeof...(Args) - sizeof...(Ts) - 1;
	        static_assert(index >= 0, "incompatible function parameters");

	        using type = typename std::tuple_element<index, std::tuple<Args...>>::type;

	        //cout << "Unpacking: " << Type<type>::Info().name << " @" << index << endl;
	        any& param = *(std::begin(v) + index);

	        return _Invoke(instance, v, any_cast<type>(param), std::forward<Ts>(ts)... );
	    }

	public:
	    MethodImpl(PointerType methodPointer)
	        : methodPointer(methodPointer)
	    {

	    }

	    any Invoke(void *instance, std::vector<any>& params)
	    {
	    	T *concreteInstance = reinterpret_cast<T *>(instance);
			return any(ReferenceType(_Invoke(concreteInstance, params)));
	    }
	};
}


#endif