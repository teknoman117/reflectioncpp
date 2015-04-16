#ifndef __VARIANT_HPP__
#define __VARIANT_HPP__

// Variant class to encapsulate some data
class Variant
{
    void *data;
public:
    template <typename T>
    Variant(T* data_)
    {
        data = (void *) data_;
    }

    Variant()
    {
        data = NULL;
    }

    template<typename T>
    T& GetValue() const
    {
        return *((T*) data);
    }

    template<typename T>
    T* Get()
    {
        return ((T*) data);
    }
};

#endif
