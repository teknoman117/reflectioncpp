#ifndef __REFLECTIONCPP_PRIMITIVE_HPP__
#define __REFLECTIONCPP_PRIMITIVE_HPP__

#include "common.h"
#include "type.h"

namespace reflectioncpp
{
	// Generic type wrapper
	class Primitive
	{
	public:
		virtual const std::string& GetName() const = 0;
		virtual const size_t       GetSize() const = 0;
		virtual const TypeCode     GetTypeCode() const = 0;
	};
}
