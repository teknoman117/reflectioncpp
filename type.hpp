#ifndef __REFLECTIONCPP_TYPE_HPP__
#define __REFLECTIONCPP_TYPE_HPP__

#include <cinttypes>
#include <functional>
#include <type_traits>

#include <cassert>

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
			constexpr static inline const TypeCode Code() \
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

	struct TypeInfo
	{
		// Type information
		const TypeCode code;
		const char*    name;

		// Qualifier data
		const bool     isReference;
		const size_t   indirection;
		const bool     isConst;
		const bool     isVolatile;

		bool operator== (const TypeInfo& rhs) const
		{
			return rhs.code == code &&
				   rhs.isReference == isReference &&
				   rhs.indirection == indirection &&
				   rhs.isConst == isConst &&
				   rhs.isVolatile == isVolatile;
		}

		bool operator!= (const TypeInfo& rhs) const
		{
			return !(*this == rhs);
		}

		constexpr TypeInfo()
			: code(0), name(0), isReference(false), indirection(0), isConst(false), isVolatile(false)
		{

		}

		constexpr TypeInfo(const TypeCode code, const char *name, const bool isReference, const size_t indirection, const bool isConst, const bool isVolatile)
			: code(code), name(name), isReference(isReference), indirection(indirection), isConst(isConst), isVolatile(isVolatile)
		{

		}

	};

	namespace definition
	{
		// Type definition class (override for typeinfo'd types)
		template <class T>
		struct TypeDefinition
		{
			constexpr static inline const char* Name()
			{
				assert(0 /*No type definition for type*/);
				return NULL;
			}
			constexpr static inline const TypeCode Code()
			{
				assert(0 /*No type definition for type*/);
				return 0;
			}
		};
	}


	// Type cleanup utility
	template <class T>
	struct Type
	{
		typedef typename std::remove_reference<T>::type      noref;
		typedef typename pointer_attributes<noref>::BaseType noptr;
		typedef typename std::remove_cv<noptr>::type         RootType;

		constexpr static inline const char* Name()
		{
			return definition::TypeDefinition<RootType>::Name();
		}
		constexpr static inline TypeCode Code()
		{
			return definition::TypeDefinition<RootType>::Code();
		}
		static inline const TypeInfo & Info()
		{
			constexpr static const TypeInfo info = TypeInfo
			(
				definition::TypeDefinition<RootType>::Code(),
				definition::TypeDefinition<RootType>::Name(),

				std::is_reference<T>::value,
				pointer_attributes<noref>::indirection(),
				std::is_const<noptr>::value,
				std::is_volatile<noptr>::value
			);

			return info;
		}
	};
}

// Pull in the primitive type definitions
#include "internal/type_definitions.hpp"

#endif
