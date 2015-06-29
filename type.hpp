#ifndef __REFLECTIONCPP_TYPE_HPP__
#define __REFLECTIONCPP_TYPE_HPP__

#include <string>
#include <cinttypes>
#include <cassert>

#include "common.hpp"

#define EXPORT_TYPEINFO(TYPE, NAME) \
namespace reflectioncpp \
{ \
	static std::string __typeinfo_name_##NAME = "" #NAME "" ; \
	static TypeCode __typeinfo_id_##NAME = internal::hash(__typeinfo_name_##NAME.c_str(), __typeinfo_name_##NAME.size()); \
	\
	template<> \
	const std::string& TypeInfo<TYPE>::GetName() \
	{ \
		return __typeinfo_name_##NAME; \
	} \
	\
	template<> \
	const reflectioncpp::TypeCode TypeInfo<TYPE>::GetHashCode() \
	{ \
		return __typeinfo_id_##NAME; \
	} \
}\

namespace reflectioncpp
{
	// The typeid object
	typedef uint64_t TypeCode;

	// Our custom internal type tracker
	template <typename T>
	struct TypeInfo
	{
		static const std::string& GetName();
		static const TypeCode GetHashCode();
	};
}

#endif
