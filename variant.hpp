#ifndef __REFLECTIONCPP_TYPE_HPP__
#define __REFLECTIONCPP_TYPE_HPP__

#include "common.hpp"
#include "type.hpp"

namespace reflectioncpp
{
	class Variant
	{
		// Currently stored typecode
		TypeCode currentType;

		// Storage for variant class
		union
		{
			float     storageFloat;
			double    storageDouble;

			char      storageChar;
			wchar_t   storageWChar;
			char16_t  storageChar16;
			char32_t  storageChar32;

			bool      storageBool;
			int8_t    storageInt8;
			uint8_t   storageUInt8;
			int16_t   storageInt16;
			uint16_t  storageUInt16;
			int32_t   storageInt32;
			uint32_t  storageUInt32;
			int64_t   storageInt64;
			uint64_t  storageUInt64;

			void     *storagePointer;
		};

	public:
		// how to deal with references versus pointers...

	};
}

