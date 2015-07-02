#ifndef __REFLECTIONCPP_ANY_HPP__
#define __REFLECTIONCPP_ANY_HPP__

#include <type_traits>
#include <algorithm>

#include "type.hpp"

namespace reflectioncpp
{
	/*
	 * any container for reflectioncpp developed from Boost.Any
	 */

	class any
	{
	public:
		any() noexcept
			: content(0)
		{
		}

		template <typename ValueType>
		any(const ValueType & value)
			: content(new holder<typename std::decay<const ValueType>::type>(value))
		{
		}

		any(const any & other)
			: content(other.content ? other.content->clone() : 0)
		{
		}

		// Move constructor
		any(any&& other) noexcept
			: content(other.content)
		{
			other.content = 0;
		}

		// Perfect forwarding of valuetype
		template <typename ValueType>
		any(ValueType&& value
			, typename std::enable_if<!std::is_same<any &, ValueType>::value >::type* = 0
			, typename std::enable_if<!std::is_const<ValueType>::value >::type* = 0)
			: content(new holder< typename std::decay<ValueType>::type >(static_cast<ValueType&&>(value)))
		{
		}

		~any() noexcept
		{
			delete content;
		} 

	public:
		any & swap(any & rhs) noexcept
		{
			std::swap(content, rhs.content);
			return *this;
		}

		any & operator= (const any& rhs)
		{
			any(rhs).swap(*this);
			return *this;
		}

		any & operator= (any&& rhs) noexcept
		{
			rhs.swap(*this);
			any().swap(rhs);
			return *this;
		}

		template <class ValueType>
		any & operator= (ValueType&& rhs)
		{
			any(static_cast<ValueType&&>(rhs)).swap(*this);
			return *this;
		}

	public:
		bool empty() const noexcept
		{
			return !content;
		}
		
		void clear() noexcept
		{
			any().swap(*this);
		}

		const TypeInfo type() const noexcept
		{
			return content ? content->type() : Type<void>::Info();
		}

	private:
		class placeholder 
		{
		public:
			virtual ~placeholder()
			{
			}

		public:
			virtual const TypeInfo type() const noexcept = 0;
			virtual placeholder * clone() const = 0;
		};

		template <typename ValueType>
		class holder : public placeholder
		{
		public:
			holder (const ValueType & value)
				: held(value)
			{
			}

			holder (ValueType&& value)
				: held(static_cast<ValueType&&>(value))
			{
			}

		public:
			virtual const TypeInfo type() const noexcept
			{
				return Type<ValueType>::Info();
			}
			virtual placeholder * clone() const
			{
				return new holder(held);
			}

		public:
			ValueType held;

		private:
			holder & operator=(const holder &);
		};

	private:
		template<typename ValueType>
		friend ValueType * any_cast(any *) noexcept;

		template<typename ValueType>
		friend ValueType * unsafe_any_cast(any *) noexcept;

		placeholder * content;

	};

	inline void swap(any & lhs, any & rhs) noexcept
	{
		lhs.swap(rhs);
	}

	class bad_any_cast : public std::bad_cast
	{
	public:
		virtual const char * what() const noexcept
		{
			return "reflectioncpp::bad_any_cast: "
				   "failed conversion using reflectioncpp::any_cast";
		}
	};

	template<typename ValueType>
	ValueType * any_cast(any * operand) noexcept
	{
		return (operand && operand->type() == Type<ValueType>::Info()) ? &static_cast<any::holder<ValueType> *>(operand->content)->held : 0;
	}

	template<typename ValueType>
    inline const ValueType * any_cast(const any * operand) noexcept
    {
        return any_cast<ValueType>(const_cast<any *>(operand));
    }

    template<typename ValueType>
    ValueType any_cast(any & operand)
    {
        typedef typename std::remove_reference<ValueType>::type nonref;

        nonref * result = any_cast<nonref>(&operand);
        if(!result)
            throw bad_any_cast();

        // Attempt to avoid construction of a temporary object in cases when 
        // `ValueType` is not a reference. Example:
        // `static_cast<std::string>(*result);` 
        // which is equal to `std::string(*result);`

        typedef typename std::conditional<
        	std::is_reference<ValueType>::value,
        	ValueType,
        	ValueType&
        >::type ref_type;

        return static_cast<ref_type>(*result);
    }

    template<typename ValueType>
    inline ValueType any_cast(const any & operand)
    {
        typedef typename std::remove_reference<ValueType>::type nonref;

        return any_cast<const nonref &>(const_cast<any &>(operand));
    }

    template<typename ValueType>
    inline ValueType&& any_cast(any&& operand)
    {
        static_assert(
            std::is_rvalue_reference<ValueType&&>::value 
            || std::is_const< typename std::remove_reference<ValueType>::type >::value,
            "reflection::any_cast shall not be used for getting nonconst references to temporary objects" 
        );
        return any_cast<ValueType&&>(operand);
    }

    // Note: The "unsafe" versions of any_cast are not part of the
    // public interface and may be removed at any time. They are
    // required where we know what type is stored in the any and can't
    // use typeid() comparison, e.g., when our types may travel across
    // different shared libraries.
    template<typename ValueType>
    inline ValueType * unsafe_any_cast(any * operand) noexcept
    {
        return &static_cast<any::holder<ValueType> *>(operand->content)->held;
    }

    template<typename ValueType>
    inline const ValueType * unsafe_any_cast(const any * operand) noexcept
    {
        return unsafe_any_cast<ValueType>(const_cast<any *>(operand));
    }
}

#endif