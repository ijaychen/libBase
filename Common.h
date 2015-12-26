#ifndef BASE_COMMON_H
#define BASE_COMMON_H

#include <stdint.h>
#define nullptr NULL
#define DISABLE_COPY(Class) \
Class(const Class&) = delete; \
Class & operator=(const Class&) = delete;

#include <cstddef>
#include <stdio.h>

#define SAFE_DELETE(ptr) do { if (ptr != nullptr) { delete ptr; ptr = nullptr; }} while(0)


#endif
