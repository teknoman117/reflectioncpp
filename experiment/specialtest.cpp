#include <iostream>
#include <cinttypes>
#include "specialhash.hpp"

using namespace std;


constexpr const char *aString = "This is a string";
constexpr const char *aString2 = "123456789";
constexpr const uint64_t * dataptr = reinterpret_cast<const uint64_t *>(aString2);
constexpr const uint64_t * dataendptr = dataptr + 1;

constexpr const uint64_t m = 0xc6a4a7935bd1e995;
constexpr const int r = 47;

constexpr uint64_t rotate(uint64_t a)
{
	return a ^ (a >> r);
}

constexpr uint64_t cfinalize_work(size_t key, uint64_t h, const unsigned char *data)
{
	return (key != 0) ? cfinalize_work(key-1, (h ^ (uint64_t(data[key - 1]) << (8 * (key-1)))), data) : h * m;
}

constexpr uint64_t cfinalize(size_t len, uint64_t h, const unsigned char *data)
{
	return (len & 7) ? rotate(rotate(cfinalize_work(len & 7, h, data)) * m)
					 : rotate(rotate(h) * m);
}

constexpr uint64_t cmix(size_t len, uint64_t h, size_t depth, const uint64_t *data, const uint64_t *end)
{
	return ((data + depth) != end) ? cmix(len, (h ^ (rotate( *(data + depth) * m ) * m)) * m, depth+1, data, end)
								   : cfinalize(len, h, (unsigned char *) end);
}

constexpr const uint64_t ct = cmix(9, 21212121221, 0, dataptr, dataendptr);


uint64_t rthash(size_t len, uint64_t h, const uint64_t *data)
{
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

	const unsigned char *data2 = (const unsigned char *) end;

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
	cout << "String: " << aString << endl;


	uint64_t rt = rthash(9, 21212121221, (const uint64_t *) &aString2);
	

	cout << "Runtime: " << rt << endl;
	cout << "Compiletime: " << ct << endl;

	return 0;
}
