#include <iostream>
#include <cinttypes>
#include "specialhash.hpp"

using namespace std;


constexpr const char *aString = "This is a string";
constexpr const unsigned char data[8] = {5, 23, 237, 178, 93, 34, 23, 54};

constexpr const uint64_t m = 0xc6a4a7935bd1e995;
constexpr const int r = 47;

constexpr uint64_t cfinalize_work(size_t key, uint64_t h, const unsigned char *data)
{
	return (key != 0) ? cfinalize_work(key-1, (h ^ (uint64_t(data[key - 1]) << (8 * (key-1)))), data) : h * m;
}
constexpr uint64_t cfinalize(size_t key, uint64_t h, const unsigned char *data)
{
	return (key != 0) ? cfinalize_work(key, h, data) : h;
}

constexpr const uint64_t ct = cfinalize(1, 21212121221, &data[0]);


uint64_t mix(size_t key, uint64_t h, const unsigned char *data2)
{
	switch(key)
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
	return h;
}

int main ()
{
	cout << "String: " << aString << endl;


	uint64_t rt = mix(1, 21212121221, &data[0]);
	

	cout << "Runtime: " << rt << endl;
	cout << "Compiletime: " << ct << endl;

	return 0;
}
