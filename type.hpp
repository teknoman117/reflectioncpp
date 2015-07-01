#ifndef __REFLECTIONCPP_TYPE_HPP__
#define __REFLECTIONCPP_TYPE_HPP__

#include <string>
#include <cinttypes>
#include <cassert>
#include <type_traits>

#include "common.hpp"
#include "compiletime.hpp"

#define DEFINE_TYPE(TYPE, NAME) \
namespace reflectioncpp \
{ \
	namespace definition \
	{ \
		constexpr const char*    __typedefinition_name_##NAME = #NAME; \
		constexpr const TypeCode __typedefinition_id_##NAME = compiletime::hash_cstring(#NAME, compiletime::length_cstring(#NAME)); \
		\
		template <> \
		struct TypeDefinition<Type<TYPE>::RootType> \
		{ \
			constexpr static inline const char * Name() \
			{ \
				return __typedefinition_name_##NAME; \
			} \
			constexpr static inline TypeCode Code() \
			{ \
				return __typedefinition_id_##NAME; \
			} \
		}; \
	} \
} \

namespace reflectioncpp
{
	// The typeid object
	typedef uint64_t TypeCode;

	namespace definition
	{
		// Type definition class (override for typeinfo'd types)
		template <class T>
		struct TypeDefinition
		{
			constexpr static const char* Name()
			{
				return NULL;
			}
			constexpr static TypeCode Code()
			{
				return 0;
			}
		};
	}

	// Type cleanup utility
	template <class T>
	struct Type
	{
		typedef typename std::remove_cv<typename utility::recursive_remove_pointer<typename std::remove_reference<T>::type>::type>::type RootType;

		constexpr static inline const char* Name()
		{
			return definition::TypeDefinition<RootType>::Name();
		}
		constexpr static inline TypeCode Code()
		{
			return definition::TypeDefinition<RootType>::Code();
		}
	};
}

// Pull in the primitive type definitions
#include "internal/type_definitions.hpp"

#endif
