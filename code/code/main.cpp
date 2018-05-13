#include <iostream>
#include "generic/memfun.hpp"
#include "generic/forwarder.hpp"

// Adder
class A 
{
public:
    A(int offset) : offset_(offset) {}

    int add(int i) const { return offset_ + i; }
    int operator()(int i) const { return add(i); }

    int add_offset_twice(int i) const { return offset_ + offset_ + i; }
private:
    const int arrpadding[100] = {}; // padding, to blow up class size
    const int offset_;
};

class RefIncrementer
{
public:
    RefIncrementer(int &ref) : ref_(ref) {}
    void increment() { ++ref_; }
private:
    int &ref_;
};



#define GEN_MAK_FOR(OBJECT, MEMFUN)                                           \
template <typename FP, FP fp, typename R, class C, typename ...A>             \
constexpr inline auto makFor(OBJECT, MEMFUN) noexcept                         \
{                                                                             \
    return gnr::forwarder<R(A...), sizeof(decltype(gnr::mem_fun<FP, fp>(object)))>(gnr::mem_fun<FP, fp>(object)); \
}

GEN_MAK_FOR(C&       object, R (C::* const)(A...) const)
GEN_MAK_FOR(C&       object, R (C::* const)(A...) const volatile)
GEN_MAK_FOR(C&       object, R (C::* const)(A...)       volatile)
GEN_MAK_FOR(C&       object, R (C::* const)(A...))

GEN_MAK_FOR(const C& object, R (C::* const)(A...) const)
GEN_MAK_FOR(const C& object, R (C::* const)(A...) const volatile)
GEN_MAK_FOR(const C& object, R (C::* const)(A...)       volatile)
GEN_MAK_FOR(const C& object, R (C::* const)(A...))

template <typename FP, FP fp, class C>
constexpr inline auto makeForwarder(C& object) noexcept
{
  return makFor<FP, fp>(object, fp);
}

template <typename FP, FP fp, class C>
constexpr inline auto makeForwarder(const C& object) noexcept
{
  return makFor<FP, fp>(object, fp);
}


int main()
{
    {
	A adder(1);
	
	auto add1 = gnr::mem_fun<MEM_FUN(A::add)>(adder);
	std::cout << add1(1) << std::endl;

	gnr::forwarder<int(int), sizeof(A)>                          f_add1(adder);                // store entire adder in forwarder (use operator()(int))
	gnr::forwarder<int(int), sizeof(decltype(std::cref(adder)))> f_add1_ref(std::cref(adder)); // don't store entire adder in forwarder, but only a cref to it
	gnr::forwarder<int(int), sizeof(add1)>                       f_add1_memfun(add1);          // mem_fun is similar to cref
	std::cout << f_add1(7)     << std::endl;
	std::cout << f_add1_ref(7) << std::endl;
	std::cout << f_add1_memfun(7) << std::endl;
	std::cout << "sizeof(f_add1): "        << sizeof(f_add1)        << std::endl;
	std::cout << "sizeof(f_add1_ref): "    << sizeof(f_add1_ref)    << std::endl;
	std::cout << "sizeof(f_add1_memfun): " << sizeof(f_add1_memfun) << std::endl;
	
	//gnr::forwarder<int(int), sizeof(decltype(gnr::mem_fun<MEM_FUN(A::add_offset_twice)>(adder)))> f_add2(gnr::mem_fun<MEM_FUN(A::add_offset_twice)>(adder));
	auto f_add2 = makeForwarder<MEM_FUN(A::add_offset_twice)>(adder);
	std::cout << f_add2(7) << std::endl;
	std::cout << "sizeof(f_add2): "     << sizeof(f_add2)     << std::endl;
    }

    std::cout << "\n\n";

    {
	int i = 1;
	RefIncrementer ri(i);
	auto refinc = gnr::mem_fun<MEM_FUN(RefIncrementer::increment)>(ri);
	refinc();
	std::cout << i << std::endl;

	//gnr::forwarder<void(), sizeof(decltype(gnr::mem_fun<MEM_FUN(RefIncrementer::increment)>(ri)))> f_refinc_memfun(gnr::mem_fun<MEM_FUN(RefIncrementer::increment)>(ri));
	auto f_refinc_memfun = makeForwarder<MEM_FUN(RefIncrementer::increment)>(ri);
	f_refinc_memfun();
	std::cout << i << std::endl;
	std::cout << "sizeof(f_refinc_memfun): " << sizeof(f_refinc_memfun)        << std::endl;
    }

    std::cout << "\n\n";

    {
	const A adder(1);
	auto  f = makeForwarder<MEM_FUN(A::add_offset_twice)>(adder);
    }
    return 0;
}
