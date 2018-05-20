#include <iostream>
#include <random>
#include <string>
#include <chrono>
#include <memory>
#include <functional>

#include "StaticFunction.h"

#include "override_new.h"
#include "measure.hpp"  

#include "generic/memfun.hpp"
#include "generic/forwarder.hpp"
#define MAX_REPEAT 100000


struct direct;
struct forw_memfun;     // memfun as unnecessary wrapper
struct function_memfun; // memfun as wrapper to have no heap

struct forw;            // forwarder (without unnecessary wrapper)
struct function_ref;    // std::ref as wrapper to have no heap

struct static_func;     // using embxx::util::StaticFunction


struct A
{
    A(): a(2) {}

    int times_a(int val)
    {
        return val * a;
    }
    
    int operator()(int val)
    {
	return val * a;
    }
    int arr[8] = {};
    int a;
};

namespace cases
{
    template<class F>
    struct caller : test::base
    {
    };

    template<>
    struct caller<direct> : test::base
    {
        caller()
        {}
        
        void benchmark()
        {
            this->val += a.times_a(this->val);
        }

        A a;
    };
	
    template<>
    struct caller<forw_memfun> : test::base
    {
        caller()
        {}
        
        void benchmark()
        {
            this->val += (f)(this->val);
        }
	
        A a;
	decltype(gnr::memfun<MEMFUN(A::times_a)>(a)) binder = gnr::memfun<MEMFUN(A::times_a)>(a);
	gnr::forwarder<int(int)> f{binder};
    };

    template<>
    struct caller<function_memfun> : test::base
    {
        caller()
        {}
        
        void benchmark()
        {
            this->val += (f)(this->val);
        }
	
        A a;
	decltype(gnr::memfun<MEMFUN(A::times_a)>(a)) binder = gnr::memfun<MEMFUN(A::times_a)>(a);
	std::function<int(int)> f{binder};
    };

    template<>
    struct caller<forw> : test::base
    {
        caller()
        {}
        
        void benchmark()
        {
            this->val += (f)(this->val);
        }
	
        A a;
	gnr::forwarder<int(int), sizeof(A)> f{a};
    };

    template<>
    struct caller<function_ref> : test::base
    {
        caller()
        {}
        
        void benchmark()
        {
            this->val += (f)(this->val);
        }
	
        A a;
	std::function<int(int)> f{std::ref(a)};
    };

    template<>
    struct caller<static_func> : test::base
    {
        caller()
        {}
        
        void benchmark()
        {
            this->val += (f)(this->val);
        }
	
        A a;
	embxx::util::StaticFunction<int(int), sizeof(A)+4> f{a};
    };

}



template<template<class> class Perf>
void benchmark1(char const* name)
{
    std::cout << "[" << name << "]\n";
    BOOST_SPIRIT_TEST_BENCHMARK(
        MAX_REPEAT,
	(Perf< direct >)
        (Perf< forw_memfun >)
        (Perf< function_memfun >)
        (Perf< forw >)
        (Perf< function_ref >)
	(Perf< static_func >)
    )
    std::cout << std::endl;
}

#define BENCHMARK(i, name) benchmark##i<cases::name>(#name)

int main(int /*argc*/, char* /*argv*/[])
{
    BENCHMARK(1, caller);

    std::cout << Memuse() << std::endl;
    
    // This is ultimately responsible for preventing all the test code
    // from being optimized away.  Change this to return 0 and you
    // unplug the whole test's life support system.
    return test::live_code != 0;
}
