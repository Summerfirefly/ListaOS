#ifndef KERNEL_TYPES_H
#define KERNEL_TYPES_H

#include <stdint.h>

#ifdef OS_64BIT
typedef uint64_t mem_addr_t;
#else
typedef uint32_t mem_addr_t;
#endif

#endif
