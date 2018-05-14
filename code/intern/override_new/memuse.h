#ifndef MEMUSE_H
#define MEMUSE_H
#pragma once

#include <ostream>

class Memuse
{
public:
    static void alloc(std::size_t sz);
    static void dealloc();

private:
    friend std::ostream &operator<<(std::ostream &os, const Memuse &memuse);
    static std::size_t memory_last_print;
    static std::size_t memory_tot;
    static std::size_t num_alloc_tot;
    static std::size_t num_alloc_cur;
    static std::size_t num_prints;
};

std::ostream &operator<<(std::ostream &os, const Memuse &memuse);

#endif
