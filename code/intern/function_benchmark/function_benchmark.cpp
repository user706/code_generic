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
struct myfunc;
struct myfunc2;

struct A
{
    A(int i): a(i) {}

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





template<typename T>
class Func {
public:
    Func()
    {
    }

    Func(const T &t)
    {
        t_ = t;
    }

    static void set(const T &t) {
        t_ = t;
    }
    
    int operator()(int i) const {
        return call(i);
    }

    static inline int call(int i) {
        return t_.times_a(i);
    }

    constexpr static void * id() {
        return std::addressof(t_);
    }
private:
    static T t_;
};

template<typename T>
T Func<T>::t_;

template<>
A Func<A>::t_(3);


struct XA {
    template<typename T>
    XA(T) : id_(Func<T>::id())
        {}
    int operator()(int i) const {
        if (id_ == Func<A>::id()) {
            return Func<A>::call(i);
        }
        return 0;
    }

    const void * const id_;
    //Func<A> func_a;
};

template <typename Sig>
struct XXX;

template <typename Ret, typename ...Args>
struct XXX<Ret(Args...)>
{
    template<typename T>
    XXX(T) : id_(Func<T>::id())
        {}
    Ret operator()(Args... args) const {
        if (id_ == Func<A>::id()) {
            return Func<A>::call(std::forward<Args>(args)...);
        }
        return 0;
    }

    const void * const id_;
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

        A a{2};
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
	
        A a{2};
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
	
        A a{2};
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
	
        A a{2};
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
	
        A a{2};
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
	
        A a{2};
	embxx::util::StaticFunction<int(int), sizeof(A)+4> f{a};
    };

    template<>
    struct caller<myfunc> : test::base
    {
        caller()
        {}
        
        void benchmark()
        {
            this->val += (x)(this->val);
        }

        A a{2};
        Func<A> x{a};
        //int(*f)(int) = x.call;
    };

    template<>
    struct caller<myfunc2> : test::base
    {
        caller()
        {
            Func<A>::set(a);
        }
        
        void benchmark()
        {
            this->val += (xyx)(this->val);
        }

        A a{2};
        XXX<int(int)> xyx{A{2}};
        

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
        (Perf< myfunc >)
        (Perf< myfunc2 >)
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
