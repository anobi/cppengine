#ifndef C_ARRAY_H
#define C_ARRAY_H

template<class T, size_t S>
class Array {
public:
    Array()     {};
    ~Array()    {};

    size_t Length() const;
    size_t Size()   const;

    const   T& operator[](size_t index) const;
            T& operator[](size_t index);

private:
    T data[S];
};


// =============================================================
// Functions
// =============================================================

template<class T, size_t S>
inline size_t Array<T, S>::Length() const {
    return S;
}

template<class T, size_t S>
inline size_t Array<T, S>::Size() const {
    return S * sizeof(T);
}


// =============================================================
// Operators
// =============================================================

template<class T, size_t S>
inline const T& Array<T, S>::operator[](size_t index) const {
    return this->data[index];
}

template<class T, size_t S>
inline T& Array<T, S>::operator[](size_t index) {
    return this->data[index];
}

#endif  // C_ARRAY_H