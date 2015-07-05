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
		virtual any Invoke( std::vector<any>& params ) = 0;

		// Empty invoke call
		any Invoke()
		{
			std::vector<any> empty;
			return Invoke(empty);
		}

		any operator()()
		{
			return Invoke();
		}

		any operator()(std::vector<any>& params)
		{
			return Invoke(params);
		}

		// could use template arguments to generate the correct defaults...
	};
}

#endif
