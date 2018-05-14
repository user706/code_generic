#include <iostream>
#include "generic/forwarder.hpp"
#include "override_new.h"

constexpr auto const default_size = 4 * sizeof(void*);

// uncomment _one_ of the two lines below
template <typename T, std::size_t N = 0> using FF = std::function<T>;
//template <typename T, std::size_t N = default_size> using FF = gnr::forwarder<T, N>;  // this only works if commenting out lines with std::bind below, or changing forwarder.hpp



// example taken from here: http://en.cppreference.com/w/cpp/utility/functional/function#Example

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

    // store a free function
    FF<void(int)> f_display = print_num;
    f_display(-9);

    // store a lambda
    FF<void()> f_display_42 = []() { print_num(42); };
    f_display_42();

    // store the result of a call to std::bind
    FF<void()> f_display_31337 = std::bind(print_num, 31337);
    f_display_31337();

    // store a call to a member function
    FF<void(const Foo&, int)> f_add_display = &Foo::print_add;
    const Foo foo(314159);
    f_add_display(foo, 1);
    f_add_display(314159, 1);

    // store a call to a data member accessor
    FF<int(Foo const&)> f_num = &Foo::num_;
    std::cout << "num_: " << f_num(foo) << '\n';

    // store a call to a member function and object
    using std::placeholders::_1;
    FF<void(int)> f_add_display2 = std::bind( &Foo::print_add, foo, _1 );
    f_add_display2(2);

    // store a call to a member function and object ptr
    FF<void(int)> f_add_display3 = std::bind( &Foo::print_add, &foo, _1 );
    f_add_display3(3);

    // store a call to a function object
    FF<void(int)> f_display_obj = PrintNum();
    f_display_obj(18);

    

    std::cout << Memuse() << "\n\n";

    return 0;
}
