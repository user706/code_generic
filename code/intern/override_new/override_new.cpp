#include "override_new.h"

void* operator new(std::size_t sz)
{
    Memuse::alloc(sz);
    void* mem = malloc(sz);
    if (mem) {
	return mem;
    } else {
	throw std::bad_alloc();
    }
}

void operator delete(void *p) noexcept
{
    Memuse::dealloc();
    free(p);
}


void* operator new[](std::size_t sz)
{
    Memuse::alloc(sz);
    void* mem = malloc(sz);
    if (mem) {
	return mem;
    } else {
	throw std::bad_alloc();
    }
}

void operator delete[](void *p) noexcept
{
    Memuse::dealloc();
    free(p);
}


void operator delete (void* p, std::size_t) noexcept
{
    Memuse::dealloc();
    free(p);
}
