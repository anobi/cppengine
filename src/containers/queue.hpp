#ifndef __CONTAINERS__QUEUE_H__
#define __CONTAINERS__QUEUE_H__

#include <assert.h>
#include <cstdlib>
#include <string.h>

#include "array.hpp"

template <class T, size_t S>
class Queue {
public:
    Queue();
    ~Queue();

    void Add(const T item);
    T Pop();

    void Clear();
    void Zero();

    size_t      Length()    const;
    size_t      Size()      const;
    const T&    Front()     const;
    const T&    Back()      const;


private:
    const size_t _offset = sizeof(T);
    unsigned int _front = 0;
    unsigned int _back = 0;
    T* _data;
};


// =============================================================
// Constructors & destructors
// =============================================================

template<class T, size_t S>
inline Queue<T, S>::Queue()
{
    this->_data = (T*) malloc(S * sizeof(T));
    memset(this->_data, 0, S * sizeof(T));
}

template<class T, size_t S>
inline Queue<T, S>::~Queue()
{
    free(this->_data);
}


// =============================================================
// Functions
// =============================================================

template<class T, size_t S>
inline void Queue<T, S>::Add(const T item)
{
    //assert(((&this->_front - &this->_back) + this->_offset) < this->Size());
    //this->_front = &this->_back[_queue_top];
    //*this->_front = item;
    //this->_queue_top++;
}

//template<class T, size_t S>
//inline T Queue<T, S>::Pop()
//{
//    T item = this->_data[this->_queue_top];
//
//    this->_queue_top -= 1;
//
//    return item;
//}
//
//template<class T, size_t S>
//inline void Queue<T, S>::Clear()
//{
//    this->_queue_top = 0;
//    this->_front = 0;
//    this->_data = 0;
//}
//
//template<class T, size_t S>
//inline void Queue<T, S>::Zero()
//{
//    this->Clear();
//    // this->_data.Clear();
//}

template<class T, size_t S>
inline const T& Queue<T, S>::Front() const
{
    return this->_data[this->_front];
}

template<class T, size_t S>
inline const T& Queue<T, S>::Back() const
{
    return this->_data[this->_back];
}

template<class T, size_t S>
inline size_t Queue<T, S>::Length() const {
    return S;
}

template<class T, size_t S>
inline size_t Queue<T, S>::Size() const
{
    return S * sizeof(T);
}

#endif  // __CONTAINERS__QUEUE_H__