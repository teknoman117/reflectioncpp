#ifndef __REFLECTIONCPP_INVOKABLE_HPP__
#define __REFLECTIONCPP_INVOKABLE_HPP__

#include <vector>

#include <boost/any.hpp>

namespace reflectioncpp
{
	class Invokable
	{
	public:
		virtual boost::any Invoke( std::vector<boost::any>& params ) = 0;

		// Empty invoke call
		boost::any Invoke()
		{
			std::vector<boost::any> empty;
			return Invoke(empty);
		}

		boost::any operator()()
		{
			return Invoke();
		}

		boost::any operator()(std::vector<boost::any>& params)
		{
			return Invoke(params);
		}

		// could use template arguments to generate the correct defaults...
	};
}

#endif
