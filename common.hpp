#ifndef __REFLECTIONCPP_COMMON_HPP__
#define __REFLECTIONCPP_COMMON_HPP__

#include <string>
#include <cinttypes>

namespace reflectioncpp
{
	namespace internal
	{
		// Constant strlen
		

		// MurmurHash2 64 bit function, used for string hashing
		uint64_t hash ( const void * key, int len, size_t seed = static_cast<size_t>(0xc70f6907UL));
	}

	// A named object
	class Named
	{
		const std::string name;
	public:
		Named(const std::string name);
		const std::string& GetName() const;
	};
}

#endif
