#ifndef __REFLECTIONCPP_COMMON_HPP__
#define __REFLECTIONCPP_COMMON_HPP__

#include <type_traits>
#include <functional>

namespace reflectioncpp
{
	template <class T, size_t N = 0>
	struct pointer_attributes
	{
		template <class U = T, size_t M = N>
		constexpr static inline typename std::enable_if<!std::is_pointer<U>::value, size_t>::type indirection()
		{
			return M;
		}

		template <class U = T, size_t M = N>
		constexpr static inline typename std::enable_if<std::is_pointer<U>::value, size_t>::type indirection()
		{
			return pointer_attributes<typename std::remove_pointer<U>::type, M+1>::indirection();
		}

		template <class U>
		struct recursive_remove_pointer
		{
		    typedef U type;
		};

		template <class U>
		struct recursive_remove_pointer<U*>
		{
		    typedef typename recursive_remove_pointer<U>::type type;
		};

		typedef typename recursive_remove_pointer<T>::type BaseType;
	};

	template <class T>
	struct Ref
	{
		typedef std::reference_wrapper<typename std::remove_reference<T>::type> wrapper;
	};
}

#endif
