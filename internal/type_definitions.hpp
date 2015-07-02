#ifndef __REFLECTIONCPP_TYPE_DEFINITIONS__
#define __REFLECTIONCPP_TYPE_DEFINITIONS__

// Good ol' void
DEFINE_TYPE(void, void);

// Export the character types
DEFINE_TYPE(char, char);
DEFINE_TYPE(wchar_t, wchar_t);
DEFINE_TYPE(char16_t, char16_t);
DEFINE_TYPE(char32_t, char32_t);

// Export the primitive data types
DEFINE_TYPE(bool, bool);
DEFINE_TYPE(int8_t, int8_t);
DEFINE_TYPE(uint8_t, uint8_t);
DEFINE_TYPE(int16_t, int16_t);
DEFINE_TYPE(uint16_t, uint16_t);
DEFINE_TYPE(int32_t, int32_t);
DEFINE_TYPE(uint32_t, uint32_t);
DEFINE_TYPE(int64_t, int64_t);
DEFINE_TYPE(uint64_t, uint64_t);
DEFINE_TYPE(float, float);
DEFINE_TYPE(double, double);

#endif
