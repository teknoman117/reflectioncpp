#ifndef __REFLECTIONCPP_INVOKABLE_HPP__
#define __REFLECTIONCPP_INVOKABLE_HPP__

#include <vector>

#include <reflectioncpp/any.hpp>
#include <reflectioncpp/type.hpp>

namespace reflectioncpp
{
	class Invokable
	{
	public:
		virtual any Invoke( void *instance, std::vector<any>& params ) = 0;

		// could use template arguments to generate the correct defaults...
	}
}

#endif
