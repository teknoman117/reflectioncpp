#ifndef __REFLECTIONCPP_COMMON_HPP__
#define __REFLECTIONCPP_COMMON_HPP__

#include <cinttypes>

namespace reflectioncpp
{
	namespace utility
	{
		template <class T>
		struct recursive_remove_pointer
		{
		    typedef T type;
		};

		template <class T>
		struct recursive_remove_pointer<T*>
		{
		    typedef typename recursive_remove_pointer<T>::type type;
		};
	}
}

#endif
