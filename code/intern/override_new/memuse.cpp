#include "memuse.h"

std::size_t Memuse::memory_last_print = 0;
std::size_t Memuse::memory_tot        = 0;
std::size_t Memuse::num_alloc_tot     = 0;
std::size_t Memuse::num_alloc_cur     = 0;
std::size_t Memuse::num_prints        = 0;

void Memuse::alloc(std::size_t sz) {
    memory_tot += sz;
    ++num_alloc_cur;
    ++num_alloc_tot;
}

void Memuse::dealloc() {
    --num_alloc_cur;
}

std::ostream &operator<<(std::ostream &os, const Memuse &memuse)
{
    os << R"(\\\\\\\)" " memuse printout (#" << memuse.num_prints++ << ") -----------\n";
    os << "memory_tot                           = " << memuse.memory_tot    << '\n';
    os << "memory_accumulation_since_last_print = " << memuse.memory_tot - memuse.memory_last_print << '\n';
    os << "num_alloc_tot                        = " << memuse.num_alloc_tot << '\n';
    os << "num_alloc_cur                        = " << memuse.num_alloc_cur;
    memuse.memory_last_print = memuse.memory_tot;
    return os;
}
