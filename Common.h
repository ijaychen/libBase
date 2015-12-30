#ifndef BASE_COMMON_H
#define BASE_COMMON_H

#ifndef __GNUC__
#error "only support gcc"
#endif


#include <stdint.h>

#ifdef HAS_CXX11
    #include <cstdint>
    #define DISABLE_COPY(Class) \
    Class(const Class&) = delete; \
    Class & operator=(const Class&) = delete;
#else
    #include <stdint.h>
    #define nullptr NULL
    #define DISABLE_COPY(Class) \
    Class(const Class&); \
    Class & operator=(const Class&);
#endif

#include <cstddef>
#include <stdio.h>

#include "Object.h"

#define SAFE_DELETE(ptr) do { if (ptr != nullptr) { delete ptr; ptr = nullptr; }} while(0)


#endif
