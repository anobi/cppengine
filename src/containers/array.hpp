#ifndef __CONTAINERS_ARRAY_H__
#define __CONTAINERS_ARRAY_H__

#include <assert.h>
#include <string.h>
#include <cstdlib>

template<class T, size_t S>
class Array {
public:
    Array();
    ~Array();

    const   size_t  Length() const;
    const   size_t  Size()   const;

    void    Clear();
    void    Destroy();

    const   T& operator[](size_t index) const;
            T& operator[](size_t index);

            void operator=(Array<T, S> other);

private:
    const size_t _size;
    const size_t _length;
    T* _data;
};


// =============================================================
// Constructors & destructors
// =============================================================

template<class T, size_t S>
inline Array<T, S>::Array() : _size(S * sizeof(T)), _length(S), _data(nullptr)
{
    this->_data = (T*)malloc(this->_size);
    memset(this->_data, 0x0, this->_size);
};

template<class T, size_t S>
inline Array<T, S>::~Array()
{
    this->Destroy();
}


// =============================================================
// Functions
// =============================================================

template<class T, size_t S>
inline const size_t Array<T, S>::Length() const
{
    return this->_length;
}

template<class T, size_t S>
inline const size_t Array<T, S>::Size() const
{
    return this->_size;
}

template<class T, size_t S>
inline void Array<T, S>::Clear()
{
    // assert(this->_data);
    memset(this->_data, 0, this->Size());
}

template<class T, size_t S>
inline void Array<T, S>::Destroy()
{
    this->_data = nullptr;
    free(this->_data);
}

// =============================================================
// Operators
// =============================================================

template<class T, size_t S>
inline const T& Array<T, S>::operator[](size_t index) const
{
    // assert(index >= 0 && index < this->Length());
    // assert(this->_data);
    return this->_data[index];
}

template<class T, size_t S>
inline T& Array<T, S>::operator[](size_t index)
{
    // assert(index >= 0 && index < this->Length());
    // assert(this->_data);
    return this->_data[index];
}

template<class T, size_t S>
inline void Array<T, S>::operator=(Array<T, S> other)
{
    // assert(this->_data && other._data);
    memcpy(this->_data, other._data, this->Size());
}

#endif  // __CONTAINERS_ARRAY_H__