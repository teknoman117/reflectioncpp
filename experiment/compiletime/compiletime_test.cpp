#include <iostream>
#include <cinttypes>
#include <iomanip>

#include "compiletime.hpp"

using namespace std;

// Compile time strlen function (awww yeah)
constexpr size_t const_strlen(const char *str, const int idx = 0)
{
	return (str[idx] == '\0') ? idx : const_strlen(str, idx+1);
}

constexpr const char    *aString       = "does a set of all sets contains itself?";
constexpr const size_t   aStringLength = const_strlen(aString);
constexpr const uint64_t aStringHash   = compiletime::hash_cstring(aString, aStringLength);

// This is the runtime version of the hash function
uint64_t MurmurHash2_64 ( const void * key, int len, size_t seed = 0xc70f6907UL )
{
	const uint64_t m = 0xc6a4a7935bd1e995;
	const int r = 47;

	uint64_t h = seed ^ (len * m);

	const uint64_t * data = (const uint64_t *)key;
	const uint64_t * end = data + (len/8);

	while(data != end)
	{
		uint64_t k = *data++;

		k *= m; 
		k ^= k >> r; 
		k *= m; 
		
		h ^= k;
		h *= m; 
	}

	const unsigned char * data2 = (const unsigned char*)data;

	switch(len & 7)
	{
	case 7: h ^= uint64_t(data2[6]) << 48;
	case 6: h ^= uint64_t(data2[5]) << 40;
	case 5: h ^= uint64_t(data2[4]) << 32;
	case 4: h ^= uint64_t(data2[3]) << 24;
	case 3: h ^= uint64_t(data2[2]) << 16;
	case 2: h ^= uint64_t(data2[1]) << 8;
	case 1: h ^= uint64_t(data2[0]);
	        h *= m;
	};
 
	h ^= h >> r;
	h *= m;
	h ^= h >> r;

	return h;
} 

int main ()
{
	cout << "String: " << aString << "\tlen=" << aStringLength << endl;

	uint64_t aStringHashRuntime = MurmurHash2_64((const void *) aString, aStringLength);

	cout << "Runtime: " << aStringHashRuntime << endl;
	cout << "Compile: " << aStringHash << endl;

	return 0;
}
