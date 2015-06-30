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
	const char* Type<reflectioncpp::TypeUtility<TYPE>::type>::GetName() \
	{ \
		return __typeinfo_name_##NAME; \
	} \
	\
	template<> \
	const reflectioncpp::TypeCode Type<reflectioncpp::TypeUtility<TYPE>::type>::GetTypeCode() \
	{ \
		return __typeinfo_id_##NAME; \
	} \
}\

namespace reflectioncpp
{
	// The typeid object
	typedef uint64_t TypeCode;

	// Our custom internal type tracker
	template <class T>
	struct Type
	{
		static const char* GetName();
		static const TypeCode GetTypeCode();
	};

	// Type cleanup utility
	template <class T>
	struct TypeUtility
	{
		typedef typename std::remove_cv<typename utility::recursive_remove_pointer<typename std::remove_reference<T>::type>::type>::type type;

		typedef typename reflectioncpp::Type<type> Type;
	};

	// Our custom internal type tracker
	/*template <class T *>
	struct Type
	{
		static const char* GetName();
		static const TypeCode GetTypeCode();
	};*/
}

#endif
