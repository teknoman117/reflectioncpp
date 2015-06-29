#ifndef __REFLECTIONCPP_TYPE_HPP__
#define __REFLECTIONCPP_TYPE_HPP__

#include <string>
#include <cinttypes>
#include <cassert>

#include "common.hpp"

#define EXPORT_TYPEINFO(TYPE, NAME) \
namespace reflectioncpp \
{ \
	constexpr const char* __typeinfo_name_##NAME = "" #NAME "" ; \
	constexpr const size_t __typeinfo_namelength_##NAME = internal::const_strlen(__typeinfo_name_##NAME); \
	TypeCode __typeinfo_id_##NAME = internal::hash(__typeinfo_name_##NAME, __typeinfo_namelength_##NAME); \
	\
	template<> \
	const char* TypeInfo<TYPE>::GetName() \
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
		static const char* GetName();
		static const TypeCode GetHashCode();
	};
}

#endif
