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

// Export the character types
DEFINE_TYPE(Ref<char>::wrapper, char_Ref);
DEFINE_TYPE(Ref<wchar_t>::wrapper, wchar_t_Ref);
DEFINE_TYPE(Ref<char16_t>::wrapper, char16_t_Ref);
DEFINE_TYPE(Ref<char32_t>::wrapper, char32_t_Ref);

// Export the primitive data types
DEFINE_TYPE(Ref<bool>::wrapper, bool_Ref);
DEFINE_TYPE(Ref<int8_t>::wrapper, int8_t_Ref);
DEFINE_TYPE(Ref<uint8_t>::wrapper, uint8_t_Ref);
DEFINE_TYPE(Ref<int16_t>::wrapper, int16_t_Ref);
DEFINE_TYPE(Ref<uint16_t>::wrapper, uint16_t_Ref);
DEFINE_TYPE(Ref<int32_t>::wrapper, int32_t_Ref);
DEFINE_TYPE(Ref<uint32_t>::wrapper, uint32_t_Ref);
DEFINE_TYPE(Ref<int64_t>::wrapper, int64_t_Ref);
DEFINE_TYPE(Ref<uint64_t>::wrapper, uint64_t_Ref);
DEFINE_TYPE(Ref<float>::wrapper, float_Ref);
DEFINE_TYPE(Ref<double>::wrapper, double_Ref);


#endif
