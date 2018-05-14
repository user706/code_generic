#ifndef OVERRIDE_NEW_H
#define OVERRIDE_NEW_H
#pragma once

#include <cstddef>
//#include <new>
#include "memuse.h"

/////////////////////////////////////////////////////
// overridden global operator new and delete

/*
void* operator new(std::size_t sz);
void operator delete(void *p) noexcept;

void* operator new[](std::size_t sz);
void operator delete[](void *p) noexcept;

void operator delete (void* p, std::size_t) noexcept;
*/


#endif // OVERRIDE_NEW_H
