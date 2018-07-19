#pragma once

#include <type_traits>
#include <cstddef>

namespace branchless {

template<typename T>
inline T min( T x, T y )
{
    static_assert(std::is_integral<T>::value, "Expected integral type here!");
    return y ^ ((x ^ y) & -(x < y));
}

template<typename T>
inline T max( T x, T y )
{
    static_assert(std::is_integral<T>::value, "Expected integral type here!");
    return x ^ ((x ^ y) & -(x < y));
}

template<typename T>
inline bool is_pow2( T x )
{
    static_assert(std::is_integral<T>::value, "Expected integral type here!");
    return x && !(x & (x - 1));
}

template<typename T>
inline T set_or_clear_bit( T x, T mask, bool set )
{
    static_assert(std::is_integral<T>::value, "Expected integral type here!");
    return (x & ~mask) | (-set & mask);
}

template<typename T>
inline T select(bool condition, T x, T y)
{
    static_assert(std::is_integral<T>::value, "Expected integral type here!");
    T const mask = condition ? static_cast<T>(-1) : 0;
    return ((x ^ y) & mask) ^ y;
}

// this class unroll loop into linear code
// very usefull on small arrays, thats fits into L1 cache

template<int N>
struct bsearch_branchless_tiny
{
    static_assert( (N & (N - 1)) == 0, "array size must be power of two!" );
    template<typename T>
    static inline size_t op( T n, T const *__restrict__ array )
    {
        static_assert(std::is_integral<T>::value, "Expected integral type here!");
        auto const K = N >> 1;
        // some compilers, like gcc failed to remove branches 
        // with ternary operator inside comosite template code
        // use select() function to help compiler to eliminate branch!
        //auto const I = (array[K] <= n) ? K : 0;
        auto const I = select(array[K] <= n, K, 0);
        return I + bsearch_branchless_tiny<K>::op(n, array + I);
    }
};

// terminator for bsearch_branchless_tiny class unroller
template<>
struct bsearch_branchless_tiny<0> { 
    template<typename T>
    static std::size_t op( T, T const *__restrict__ ) { return 0; } 
};

} // namespace branchless
