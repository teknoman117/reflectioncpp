#ifndef __REFLECTIONCPP_VARIANT_HPP__
#define __REFLECTIONCPP_VARIANT_HPP__

#include <functional>
#include <type_traits>

#include "common.hpp"
#include "type.hpp"

#include <iostream>

namespace reflectioncpp
{
	class Variant
	{
		// Current storage method
		typedef enum : uint8_t
		{
			StorageMethodNone                   = 0,
			StorageMethodPrimitive              = 1,
			StorageMethodUnownedPointer         = 2,
			StorageMethodUnownedConstPointer    = 3,
			StorageMethodOwnedPointer           = 4,
			StorageMethodOwnedNonTrivialPointer = 5,
		} StorageMethod;
		
		TypeCode      currentType;
		StorageMethod storageMethod;
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

			void       *storagePointer;
			const void *storageConstPointer;
		};

		std::function<void (void)> storagePointerDeleter;

	public:
		// Empty constructor 
		Variant()
			: currentType(0), storageMethod(StorageMethodNone), storagePointer(nullptr)
		{
		}

		~Variant()
		{
			Release();
		}

		// Simple method to check whether variant is empty
		bool IsEmpty () const
		{
			return (storageMethod == StorageMethodNone);
		}

		// Release the contents of the variant and become empty
		bool Release()
		{
			if(storageMethod == StorageMethodOwnedNonTrivialPointer)
			{
				storagePointerDeleter();
			}
			else if(storageMethod == StorageMethodOwnedPointer)
			{
				free(storagePointer);
			}

			storageMethod = StorageMethodNone;
		}

		// If we are to storing a pointer, life is easy
		template <class T>
		typename std::enable_if<std::is_pointer<T>::value &&
								!std::is_const<typename utility::recursive_remove_pointer<typename std::remove_reference<T>::type>::type>::value
								>::type
		Set(T data)
		{
			Release();
			
			storagePointer = reinterpret_cast<void *>(data);
			storageMethod = StorageMethodUnownedPointer;
			currentType = TypeUtility<T>::Type::GetTypeCode();
		}

		// If we are to storing a const pointer, life is easy
		template <class T>
		typename std::enable_if<std::is_pointer<T>::value &&
								std::is_const<typename utility::recursive_remove_pointer<typename std::remove_reference<T>::type>::type>::value
								>::type
		Set(T data)
		{
			Release();
			
			storageConstPointer = reinterpret_cast<const void *>(data);
			storageMethod = StorageMethodUnownedConstPointer;
			currentType = TypeUtility<T>::Type::GetTypeCode();
		}

		// If we are to be storing a reference
		template <class T>
		typename std::enable_if<std::is_reference<T>::value &&
								!std::is_const<typename utility::recursive_remove_pointer<typename std::remove_reference<T>::type>::type>::value
								>::type
		Set(T data)
		{
			Release();
			
			storagePointer = reinterpret_cast<void *>(&data);
			storageMethod = StorageMethodUnownedPointer;
			currentType = TypeUtility<T>::Type::GetTypeCode();
		}

		// If we are to storing a const reference, life is easy
		template <class T>
		typename std::enable_if<std::is_reference<T>::value &&
								std::is_const<typename utility::recursive_remove_pointer<typename std::remove_reference<T>::type>::type>::value
								>::type
		Set(T data)
		{
			Release();
			
			storageConstPointer = reinterpret_cast<const void *>(&data);
			storageMethod = StorageMethodUnownedConstPointer;
			currentType = TypeUtility<T>::Type::GetTypeCode();
		}

		// TODO - Write all the setter handlers for primitives
		//template <class T>
		//typename std::enable_if


		// If we are storing a trivial type (need to clone)
		template <class T>
		typename std::enable_if<!std::is_pointer<T>::value &&
								!std::is_reference<T>::value && 
								 std::is_trivially_copyable<T>::value
								>::type
		Set (T data)
		{
			Release();

			// Clean type (remove all qualifiers)
			using U = typename TypeUtility<T>::type;

			// Perform a trival copy
			storagePointer = malloc(sizeof(U));
			storageMethod = StorageMethodOwnedPointer;
			currentType = Type<U>::GetTypeCode();

			memcpy(storagePointer, &data, sizeof(U));
		}

		// If we are storing a non trival type, we have to construct it
		template <class T>
		typename std::enable_if<!std::is_pointer<T>::value &&
								!std::is_reference<T>::value && 
								!std::is_trivially_copyable<T>::value
								>::type
		Set (T data)
		{
			Release();

			// Clean type (remove all qualifiers)
			using U = typename TypeUtility<T>::type;

			// Perform a non trivial copy
			U* temporary = new U(data);
			
			storagePointer = reinterpret_cast<void *>(temporary);
			storageMethod = StorageMethodOwnedNonTrivialPointer;
			currentType = Type<U>::GetTypeCode();

			storagePointerDeleter = [this] ()
			{
				delete reinterpret_cast<U *>(storagePointer);
			};
		}

		// Assignment operator to any generic type is setting as a const reference
		template <class T>
		T operator= (T data)
		{
			Set(data);
			return data;
		}
	};
}

#endif
