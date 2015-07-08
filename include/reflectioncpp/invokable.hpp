#ifndef __REFLECTIONCPP_INVOKABLE_HPP__
#define __REFLECTIONCPP_INVOKABLE_HPP__

#include <vector>

#include <boost/any.hpp>
#include <boost/range/any_range.hpp>

namespace reflectioncpp
{	
	// Wrapper around any container 
	typedef boost::any_range<boost::any,
							 boost::forward_traversal_tag,
							 boost::any&,
							 std::ptrdiff_t
							> parameter_range;

	class Invokable
	{
	public:
		virtual boost::any Invoke(const parameter_range& params) const = 0;
        virtual boost::any Invoke() const = 0;

        // Invoke with initializer list
        inline boost::any InvokeWithList(const std::initializer_list<boost::any>& initializer) const
        {
            return Invoke(initializer);
        }
	};
}

#endif
