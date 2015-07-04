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
}

// Include the specializations of the method
#include "internal/method_noreturn.hpp"
#include "internal/method_normalreturn.hpp"
#include "internal/method_referencereturn.hpp"

#endif
