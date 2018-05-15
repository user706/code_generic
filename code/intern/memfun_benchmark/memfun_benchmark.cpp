#include <iostream>
#include <random>
#include <string>
#include <chrono>
#include <memory>

#include "measure.hpp"  

#include "memfun.hpp"

#define MAX_REPEAT 100000


struct direct;
struct gnr_memfun;
struct mfp_direct;
struct binder;



struct A
{
    A(): a(2) {}

    int times_a(int val)
    {
        return val * a;
    }
    
    int a;
};

namespace cases
{
    template<class F>
    struct mfp_caller : test::base
    {
    };

    template<>
    struct mfp_caller<direct> : test::base
    {
        mfp_caller()
        {}
        
        void benchmark()
        {
            this->val += a.times_a(this->val);
        }

        A a;
    };
	
    template<>
    struct mfp_caller<gnr_memfun> : test::base
    {
        mfp_caller()
        {}
        
        void benchmark()
        {
            this->val += f(this->val);
        }

	A a;
        decltype(gnr::memfun<MEMFUN(A::times_a)>(a)) f = gnr::memfun<MEMFUN(A::times_a)>(a);
    };


    template<>
    struct mfp_caller<mfp_direct> : test::base
    {
        mfp_caller()
        {}
        
        void benchmark()
        {
            this->val += (a.*t2)(this->val);
        }

        A a;
	int(A::*t2)(int) = &A::times_a;
    };

    template<>
    struct mfp_caller<binder> : test::base
    {
        mfp_caller()
        {}
        
        void benchmark()
        {
            this->val += f(this->val);
        }
        A a;
	decltype(std::bind(&A::times_a, &a, std::placeholders::_1)) f = std::bind(&A::times_a, &a, std::placeholders::_1);
    };

}



template<template<class> class Perf>
void benchmark1(char const* name)
{
    std::cout << "[" << name << "]\n";
    BOOST_SPIRIT_TEST_BENCHMARK(
        MAX_REPEAT,
	(Perf< direct >)
        (Perf< gnr_memfun >)
        (Perf< mfp_direct >)
        (Perf< binder >)
    )
    std::cout << std::endl;
}

#define BENCHMARK(i, name) benchmark##i<cases::name>(#name)

int main(int /*argc*/, char* /*argv*/[])
{
    BENCHMARK(1, mfp_caller);

    // This is ultimately responsible for preventing all the test code
    // from being optimized away.  Change this to return 0 and you
    // unplug the whole test's life support system.
    return test::live_code != 0;
}
