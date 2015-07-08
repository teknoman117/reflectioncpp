#ifndef __REFLECTIONCPP_METHOD_HPP__
#define __REFLECTIONCPP_METHOD_HPP__

#include <iostream>
#include <vector>
#include <tuple>
#include <exception>
#include <stdexcept>
#include <functional>

#include <boost/any.hpp>

#include "invokable.hpp"

namespace reflectioncpp
{
	// Method invoke implementation
	template<class,class, typename Enable = void>
	class MethodImpl;
}

// Include the specializations of the method
#include "internal/method_noreturn.hpp"
#include "internal/method_normalreturn.hpp"
#include "internal/method_referencereturn.hpp"

#endif
