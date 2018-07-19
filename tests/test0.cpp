#include "../include/branchless.hpp"
#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <stdint.h>
#include <stdlib.h>

namespace branchless {

static void test_min_max()
{
    for( int i = 0; i < 10000; ++i )
    for( int k = 0; k < 10000; ++k )
    {
        if( std::min(i, k) != min(i, k) )
            throw std::runtime_error("min failed");  
        if( std::max(i, k) != max(i, k) )
            throw std::runtime_error("max failed");
    }
}

static void test_is_pow2()
{
    uint32_t cnt = 0;
    for( uint32_t i = 0; i < 0xffffffffU; ++i )
    {
        if( is_pow2(i) )
            ++cnt;
    }

    if( cnt != 32 )
        throw std::runtime_error("too many counts!");
}

static uint32_t const MAX_VALUE = 10000; // can be any value!

static void test_select()
{
    for( int i = 0; i < 100000; ++i )
    {
        int const x = rand() % MAX_VALUE;
        int const y = rand() % MAX_VALUE;
        int choose = i & 1 ? x : y;
        if( select(i & 1, x, y) != (i & 1 ? x : y) )
            throw std::runtime_error("select not equal!");
    }
}

template<int N>
static void test_bsearch_tiny_phase()
{
    uint32_t array[N];
    for( auto &v : array )
    {
        v = rand() % MAX_VALUE;
    }

    std::sort(array, array + N);

    // forward and search in self
    for( int i = 0; i < N; ++i )
    {
        uint32_t idx = bsearch_branchless_tiny<N>::op(array[i], &array[0]); 
        if( idx == N )
            throw std::runtime_error("value is not found!");
        //std::cout << "found index=" << idx << " current value=" << array[i] << " found value=" << array[idx] << std::endl;
        if( array[idx] != array[i] )
            throw std::runtime_error("invalid value found!");
    }    
}

static void test_bsearch_tiny()
{
    test_bsearch_tiny_phase<16>();
    test_bsearch_tiny_phase<32>();
    test_bsearch_tiny_phase<64>();
    test_bsearch_tiny_phase<128>();
    test_bsearch_tiny_phase<256>();
    test_bsearch_tiny_phase<512>();
    test_bsearch_tiny_phase<1024>();
} 

} // namespace branchless

#define TEST_ME(func) \
try { func(); std::cout << "test " #func " success!" << std::endl; } catch( std::exception const &ex ) \
    { std::cerr << "test " #func " failure: " << ex.what() << std::endl; } 


int main()
{
    TEST_ME(branchless::test_min_max);
    TEST_ME(branchless::test_is_pow2);
    TEST_ME(branchless::test_select);
    TEST_ME(branchless::test_bsearch_tiny);
    return 0;
}
