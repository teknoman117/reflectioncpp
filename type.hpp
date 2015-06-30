#ifndef __REFLECTIONCPP_TYPE_HPP__
#define __REFLECTIONCPP_TYPE_HPP__

#include <string>
#include <cinttypes>
#include <cassert>
#include <type_traits>

#include "common.hpp"

#define EXPORT_TYPEINFO(TYPE, NAME) \
namespace reflectioncpp \
{ \
	constexpr const char* __typeinfo_name_##NAME = "" #NAME "" ; \
	constexpr const size_t __typeinfo_namelength_##NAME = utility::const_strlen(__typeinfo_name_##NAME); \
	TypeCode __typeinfo_id_##NAME = utility::hash(__typeinfo_name_##NAME, __typeinfo_namelength_##NAME); \
	\
	template<> \
	const char* Type<TYPE>::GetName() \
	{ \
		return __typeinfo_name_##NAME; \
	} \
	\
	template<> \
	const reflectioncpp::TypeCode Type<TYPE>::GetTypeCode() \
	{ \
		return __typeinfo_id_##NAME; \
	} \
}\

namespace reflectioncpp
{
	// The typeid object
	typedef uint64_t TypeCode;

	// Type cleanup utility
	template <typename T>
	struct TypeCleaner
	{
		typedef std::remove_reference<std::remove_cv<T> > type;
	};

	// Our custom internal type tracker
	template <typename T>
	struct Type
	{
		static const char* GetName();
		static const TypeCode GetTypeCode();
	};
}

#endif
