/*
 * div64.c
 *
 * The library of Lista OS kernel
 * Written by Natsuyoru (a.k.a Summerfirefly)
 *
 * Include some functions for the divide of 64-bits number
 *
 * Licenced under LGPLv3
 *
 */

#include "div64.h"


/*
 * 64-bits number divides 32-bits number
 *
 * return rem, res saves in n
 */
unsigned long long do_div64_32(unsigned long long *n, unsigned int base)
{
    unsigned long long rem = *n;
	unsigned long long res = 0;
	unsigned long long b = base;
	unsigned long long d = 1;
	unsigned int high = rem >> 32;
	
	if (rem < b)
	{
	    *n = 0;
	    return rem;
	}
	
	if (high == 0)
	{
	    *n = (unsigned long long)((unsigned int) rem / base);
	    return (unsigned long long)((unsigned int) rem % base);
	}
	
	if (high >= b)
	{
		high /= base;
		res = (unsigned long long) high << 32;
		rem -= ((unsigned long long) (high * base)) << 32;
	}

	while (b < rem)
	{
		b = b+b;
		d = d+d;
	}

	do
	{
		if (rem >= b)
		{
			rem -= b;
			res += d;
		}
		b >>= 1;
		d >>= 1;
	} while (d != 0);

	*n = res;
	return rem;
}
