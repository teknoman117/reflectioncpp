#include "common.hpp"
#include "type.hpp"

// Export the character types
EXPORT_TYPEINFO(char, char);
EXPORT_TYPEINFO(wchar_t, wchar_t);
EXPORT_TYPEINFO(char16_t, char16_t);
EXPORT_TYPEINFO(char32_t, char32_t);

// Export the primitive data types
EXPORT_TYPEINFO(bool, bool);
EXPORT_TYPEINFO(int8_t, int8_t);
EXPORT_TYPEINFO(uint8_t, uint8_t);
EXPORT_TYPEINFO(int16_t, int16_t);
EXPORT_TYPEINFO(uint16_t, uint16_t);
EXPORT_TYPEINFO(int32_t, int32_t);
EXPORT_TYPEINFO(uint32_t, uint32_t);
EXPORT_TYPEINFO(int64_t, int64_t);
EXPORT_TYPEINFO(uint64_t, uint64_t);
EXPORT_TYPEINFO(float, float);
EXPORT_TYPEINFO(double, double);

