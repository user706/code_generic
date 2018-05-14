#include <iostream>
#include "generic/forwarder.hpp"
#include "override_new.h"

constexpr auto const default_size = 4 * sizeof(void*);

// uncomment _one_ of the two lines below
template <typename T, std::size_t N = 0> using FF = std::function<T>;
//template <typename T, std::size_t N = default_size> using FF = gnr::forwarder<T, N>;



class MathOperand
{
public:
    MathOperand(int operand) : operand_(operand)  {    }
    int add(int i)        const { return i + operand_; }
    int mul(int i)        const { return i * operand_; }
    int operator()(int i) const { return add(i);       }
private:
    int const operand_;
};


class Accumulator
{
public:
    long double add(long double v)        { sum_  += v; return sum_; }
    long double mul(long double v)        { prod_ *= v; return sum_; }
    long double operator()(long double v) { return add(v); }
private:
    int arr_[100]; // padding
    long double sum_  = 0;
    long double prod_ = 1;
};



int main()
{
    MathOperand const mo(10);
    {
	FF<int(MathOperand const &, int)> f_add(&MathOperand::add);
	FF<int(MathOperand const &, int)> f_mul(&MathOperand::mul);
	
	std::cout << "add: " << f_add(mo, 2) << std::endl;
	std::cout << "mul: " << f_mul(mo, 2) << std::endl;
    }
    {
	FF<int(MathOperand const *, int)> fp_add(&MathOperand::add);
	FF<int(MathOperand const *, int)> fp_mul(&MathOperand::mul);
	
	std::cout << "add: " << fp_add(&mo, 2) << std::endl;
	std::cout << "mul: " << fp_mul(&mo, 2) << std::endl;
    }

    std::cout << Memuse() << "\n\n";

    {
	Accumulator acc;
	std::cout << "sizeof(Accumulator) = " << sizeof(Accumulator) << std::endl;
	FF<long double(long double), sizeof(Accumulator)> f_bound(acc);
	std::cout << "sizeof(f_bound) = " << sizeof(f_bound) << std::endl;
	std::cout << "operator()(long double) = add = " << f_bound(2.2) << std::endl;
    }
    std::cout << Memuse() << std::endl;
    return 0;
}
