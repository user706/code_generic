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
    void x() const {
	std::cout << "here" << std::endl;
    }
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

class MathOperand
{
public:
    MathOperand(int operand) : operand_(operand)
	{}
    int add(int i) const { return i + operand_; }
    int mul(int i) const { return i * operand_; }
private:
    int const operand_;
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

struct Foo {
    Foo(int num) : num_(num) {}
    void print_add(int i) const { std::cout << num_+i << '\n'; }
    int num_;
};

void print_num(int i)
{
    std::cout << i << '\n';
}

struct PrintNum {
    void operator()(int i) const
	{
	    std::cout << i << '\n';
	}
};

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

    {
	A adder(1);
	gnr::forwarder<void(A &)> f1(&A::x);
	gnr::forwarder<int(A &, int)> f2(&A::add_offset_twice);
	std::cout << f2(adder, 4) << std::endl;

    }
    std::cout << "----------------" << std::endl;
    {
	// store a free function
	std::function<void(int)> f_display = print_num;
	f_display(-9);

	// store a lambda
	std::function<void()> f_display_42 = []() { print_num(42); };
	f_display_42();

	// store the result of a call to std::bind
	std::function<void()> f_display_31337 = std::bind(print_num, 31337);
	f_display_31337();

	// store a call to a member function
	std::function<void(const Foo&, int)> f_add_display = &Foo::print_add;
	const Foo foo(314159);
	f_add_display(foo, 1);
	f_add_display(314159, 1);

	// store a call to a data member accessor
	std::function<int(Foo const&)> f_num = &Foo::num_;
	std::cout << "num_: " << f_num(foo) << '\n';

	// store a call to a member function and object
	using std::placeholders::_1;
	std::function<void(int)> f_add_display2 = std::bind( &Foo::print_add, foo, _1 );
	f_add_display2(2);

	// store a call to a member function and object ptr
	std::function<void(int)> f_add_display3 = std::bind( &Foo::print_add, &foo, _1 );
	f_add_display3(3);

	// store a call to a function object
	std::function<void(int)> f_display_obj = PrintNum();
	f_display_obj(18);
    }
    std::cout << "----------------" << std::endl;
    // {
    // 	// store a free function
    // 	gnr::forwarder<void(int)> f_display = print_num;
    // 	f_display(-9);

    // 	// store a lambda
    // 	gnr::forwarder<void()> f_display_42 = []() { print_num(42); };
    // 	f_display_42();

    // 	// store the result of a call to std::bind
    // 	gnr::forwarder<void()> f_display_31337 = std::bind(print_num, 31337);
    // 	f_display_31337();

    // 	// store a call to a member function
    // 	gnr::forwarder<void(const Foo&, int)> f_add_display = &Foo::print_add;
    // 	const Foo foo(314159);
    // 	f_add_display(foo, 1);
    // 	f_add_display(314159, 1);

    // 	// store a call to a data member accessor
    // 	gnr::forwarder<int(Foo const&)> f_num = &Foo::num_;
    // 	std::cout << "num_: " << f_num(foo) << '\n';

    // 	// store a call to a member function and object
    // 	using std::placeholders::_1;
    // 	gnr::forwarder<void(int)> f_add_display2 = std::bind( &Foo::print_add, foo, _1 );
    // 	f_add_display2(2);

    // 	// store a call to a member function and object ptr
    // 	gnr::forwarder<void(int)> f_add_display3 = std::bind( &Foo::print_add, &foo, _1 );
    // 	f_add_display3(3);

    // 	// store a call to a function object
    // 	gnr::forwarder<void(int)> f_display_obj = PrintNum();
    // 	f_display_obj(18);
    // }
    {

	//template <typename T>
	    //using funfor = std::function<T>;
	
	MathOperand const mo(10);
	gnr::forwarder<int(MathOperand const &, int)> f_add(&MathOperand::add);
	gnr::forwarder<int(MathOperand const &, int)> f_mul(&MathOperand::mul);

	std::cout << "add: " << f_add(mo, 2) << std::endl;
	std::cout << "mul: " << f_mul(mo, 2) << std::endl;

	gnr::forwarder<int(MathOperand const *, int)> fp_add(&MathOperand::add);
	gnr::forwarder<int(MathOperand const *, int)> fp_mul(&MathOperand::mul);

	std::cout << "add: " << fp_add(&mo, 2) << std::endl;
	std::cout << "mul: " << fp_mul(&mo, 2) << std::endl;

    }
    return 0;
}
