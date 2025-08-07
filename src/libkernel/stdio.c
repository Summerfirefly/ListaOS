/*
 * stdio.c
 *
 * The standard C library for Lista OS kernel
 * Written by Natsuyoru (a.k.a Summerfirefly)
 *
 * Include some functions for standard input/output
 *
 * Licenced under LGPLv3
 *
 */

#include <stdarg.h>
#include <stdbool.h>
#include "stdio.h"
#include "console.h"


/*Private functions*/
char *_ftoa(double num, int n);
char *_itoa(long long num);
char *_itoh(unsigned long long num, bool uppercase);
char *_ptoa(unsigned int num);
char *_utoa(unsigned long long num);


/*Print a character to screen*/
int putc(char ch)
{
    print_char(ch);
    
    return (int) ch;
}


/*Print a string to screen*/
int puts(const char *str)
{
    int len = 0;

    while (*str != '\0')
    {
        putc(*str++);
        ++len;
    }
    
    return len;
}


/*Print format string to screen*/
int printf(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    const int len = vprintf(format, args);
    va_end(args);
    return len;
}


int vprintf(const char *format, va_list args)
{
    int len = 0;
    while (*format != '\0')
    {
        while (*format != '%' && *format != '\0')
        {
            putc(*format++);
            ++len;
        }

        if (*format == '\0')
            break;

        switch (*(++format))
        {
            case 'c': /*%c*/
                putc((char) va_arg(args, int));
                ++len;
                break;
            case 'd': /*%d*/
                len += puts(_itoa(va_arg(args, int)));
                break;
            case 'f': /*%f*/
                len += puts(_ftoa(va_arg(args, double), 6));
                break;
            case 'l':
                ++format;
                if (*format == 'd') /*%ld*/
                {
                    len += puts(_itoa(va_arg(args, long)));
                }
                else if (*format == 'u') /*%lu*/
                {
                    len += puts(_utoa(va_arg(args, unsigned long)));
                }
                else if (*format == 'X') /*%lX*/
                {
                    len += puts(_itoh(va_arg(args, unsigned long), true));
                }
                else if (*format == 'x') /*%lx*/
                {
                    len += puts(_itoh(va_arg(args, unsigned long), false));
                }
                else
                {
                    ++format;
                    if (*format == 'd') /*%lld*/
                    {
                        len += puts(_itoa(va_arg(args, long long)));
                    }
                    else if (*format == 'u') /*%llu*/
                    {
                        len += puts(_utoa(va_arg(args, unsigned long long)));
                    }
                    else if (*format == 'X') /*%llX*/
                    {
                        len += puts(_itoh(va_arg(args, unsigned long long), true));
                    }
                    else if (*format == 'x') /*%llx*/
                    {
                        len += puts(_itoh(va_arg(args, unsigned long long), false));
                    }
                }
                break;
            case 'p': /*%p*/
                len += puts(_ptoa(va_arg(args, unsigned int)));
                break;
            case 's': /*%s*/
                len += puts(va_arg(args, char *));
                break;
            case 'u': /*%u*/
                len += puts(_utoa(va_arg(args, unsigned int)));
                break;
            case 'X': /*%X*/
                len += puts(_itoh(va_arg(args, unsigned int), true));
                break;
            case 'x': /*%x*/
                len += puts(_itoh(va_arg(args, unsigned int), false));
                break;
            case '%': /*%%*/
                putc('%');
                ++len;
                break;
            default:
                break;
        }

        ++format;
    }

    return len;
}


/*Write format string to memory*/
int sprintf(char *str, const char *format, ...)
{
    va_list vap;
    va_start(vap, format);

    char *current = str;
    
    while (*format != '\0')
    {
        while (*format != '%' && *format != '\0')
            *current++ = *format++;
        
        if (*format == '\0')
            break;
        
        char sign = *(++format);
        char *temp = NULL;
        switch (sign)
        {
            case 'c': /*%c*/
                *current++ = (char) va_arg(vap, int);
                break;
            case 'd': /*%d*/
                temp = _itoa(va_arg(vap, int));
                current += sprintf(current, temp);
                break;
            case 'f': /*%f*/
                temp = _ftoa(va_arg(vap, double), 6);
				current += sprintf(current, temp);
                break;
            case 'l':
                ++format;
                if (*format == 'd') /*%ld*/
                {
                    temp = _itoa(va_arg(vap, long));
                    current += sprintf(current, temp);
                }
                else if (*format == 'u') /*%lu*/
                {
                    temp = _utoa(va_arg(vap, unsigned long));
                    current += sprintf(current, temp);
                }
                else if (*format == 'X') /*%lX*/
                {
                    temp = _itoh(va_arg(vap, unsigned long), true);
                    current += sprintf(current, temp);
                }
                else if (*format == 'x') /*%lx*/
                {
                    temp = _itoh(va_arg(vap, unsigned long), false);
                    current += sprintf(current, temp);
                }
                else
                {
                    ++format;
                    if (*format == 'd') /*%lld*/
                    {
                        temp = _itoa(va_arg(vap, long long));
                        current += sprintf(current, temp);
                    }
                    else if (*format == 'u') /*%llu*/
                    {
                        temp = _itoa(va_arg(vap, unsigned long long));
                        current += sprintf(current, temp);
                    }
                    else if (*format == 'X') /*%llX*/
                    {
                        temp = _itoh(va_arg(vap, unsigned long long), true);
                        current += sprintf(current, temp);
                    }
                    else if (*format == 'x') /*%llx*/
                    {
                        temp = _itoh(va_arg(vap, unsigned long long), false);
                        current += sprintf(current, temp);
                    }
                }
                break;
            case 'p': /*%p*/
                temp = _ptoa(va_arg(vap, unsigned int));
                current += sprintf(current, temp);
                break;
            case 's': /*%s*/
                temp = va_arg(vap, char *);
                current += sprintf(current, temp);
                break;
            case 'u': /*%u*/
                temp = _itoa(va_arg(vap, unsigned int));
                current += sprintf(current, temp);
                break;
            case 'X': /*%X*/
                temp = _itoh(va_arg(vap, unsigned int), true);
                current += sprintf(current, temp);
                break;
            case 'x': /*%x*/
                temp = _itoh(va_arg(vap, unsigned int), false);
                current += sprintf(current, temp);
                break;
            case '%': /*%%*/
                *current++ = '%';
                break;
            default:;
        }
        
        ++format;
    }
    
    *current = '\0';
    va_end(vap);
    return current - str;
}



/************ Attention ************
 *                                 *
 * The under functions are private *
 *                                 *
 ***********************************/


char *_ftoa(double num, int n)
{
	int i = 0;
    static char str[256] = {'\0'};

    if (num < 0)
    {
        str[i++] = '-';
        num = -num;
    }

	if (num <= 0xffffffffffffffff)
	{
		unsigned long long intPart = (unsigned long long)num;
		double floatPart = num - intPart;
		
		char *tempStr = _utoa(intPart);
		for (int j = 0; tempStr[j] != '\0'; ++j)
		{
			str[i++] = tempStr[j];
		}
		
		str[i++] = '.';
		
		unsigned long long tempInt = 1;
		for (int j = 0; j < n; ++j)
		{
			tempInt *= 10;
		}
		
		tempStr = _utoa((unsigned long long)(floatPart * tempInt));
		for (int j = 0; tempStr[j] != '\0'; ++j)
		{
			str[i++] = tempStr[j];
		}
	}
	else
	{
	}
	
	str[i] = '\0';
	
    return str;
}


/*Convert oct integer to oct number string*/
char *_itoa(long long num)
{
    if (num == 0)
        return "0";

    int i = 0;
    int j = 0;
    static char str[30] = {'\0'};
    
    if (num < 0)
    {
        str[i++] = '-';
        num = -num;
    }
    
    char temp[30] = {'\0'};
    while (num != 0)
    {
        temp[j++] = (char)(num % 10 + '0');
        num /= 10;
    }
    
    while (j > 0)
    {
        str[i++] = temp[--j];
    }
    
    str[i] = '\0';
    
    return str;
}


/*Convert oct integer to hex number string*/
char *_itoh(unsigned long long num, bool uppercase)
{
    static char hex[17] = {'\0'};
    char temp[16] = {'\0'};

    int i = 0;
    int j = 0;
    for (i = 0; i < 16; ++i)
    {
        unsigned long long t = 0xf & num;

        if (t < 10) temp[i] = t + '0';
        else temp[i] = t - 10 + (uppercase ? 'A' : 'a');

        num >>= 4;
    }
    
    while (temp[i - 1] == '0' && i > 1)
    {
        temp[--i] = '\0';
    }

    while (i > 0)
    {
        hex[j++] = temp[--i];
    }

    hex[j] = '\0';

    return hex;
}


/*Convert pointer to hex number string*/
char *_ptoa(unsigned int num)
{
    static char hex[11] = {'0', 'x'};
    char temp[8] = {'\0'};

    int i = 0;
    int j = 2;
    for (i = 0; i < 8; ++i)
    {
        int t = 0xf & num;

        if (t < 10) temp[i] = t + '0';
        else temp[i] = t - 10 + 'a';

        num >>= 4;
    }

    while (i > 0)
    {
        hex[j++] = temp[--i];
    }

    hex[j] = '\0';

    return hex;
}


/*Convert unsigned oct integer to oct number string*/
char *_utoa(unsigned long long num)
{
    if (num == 0)
        return "0";

    int i = 0;
    int j = 0;
    static char str[30] = {'\0'};
    
    char temp[30] = {'\0'};
    while (num != 0)
    {
        temp[j++] = (char)(num % 10 + '0');
        num /= 10;
    }

    while (j > 0)
    {
        str[i++] = temp[--j];
    }
    
    str[i] = '\0';
    return str;
}
