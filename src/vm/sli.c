/*
# This file is Copyright 2003, 2006, 2007, 2009, 2010 Dean Hall.
#
# This file is part of the PyMite VM.
# The PyMite VM is free software: you can redistribute it and/or modify
# it under the terms of the GNU GENERAL PUBLIC LICENSE Version 2.
#
# The PyMite VM is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# A copy of the GNU GENERAL PUBLIC LICENSE Version 2
# is seen in the file COPYING in this directory.
*/


#undef __FILE_ID__
#define __FILE_ID__ 0x11


/**
 * \file
 * \brief Standard Library Interface
 *
 * PyMite requires a few functions from a few different
 * standard C libraries (memory, string, etc).
 */


#include "pm.h"


/** use Duff's Device or simple for-loop for memcpy. */
#define USE_DUFFS_DEVICE    0


#if !HAVE_STRING_H

void *
sli_memcpy(unsigned char *to, unsigned char const *from, unsigned int n)
{
    unsigned char *tobak;

    /* Store init value of to */
    tobak = to;

#if USE_DUFFS_DEVICE
    if (n > 0)
    {
        switch (n & 0x7)
            do
            {
            case 0:
                *to++ = *from++;
            case 7:
                *to++ = *from++;
            case 6:
                *to++ = *from++;
            case 5:
                *to++ = *from++;
            case 4:
                *to++ = *from++;
            case 3:
                *to++ = *from++;
            case 2:
                *to++ = *from++;
            case 1:
                *to++ = *from++;
            }
            while ((n -= 8) > 0);
    }
#else
    for (; n > 0; n--)
    {
        *to = *from;
        from++;
        to++;
    }
#endif /* USE_DUFFS_DEVICE */
    return tobak;
}


int
sli_strlen(char const *s)
{
    char const *si = s;
    int len = 0;

    while (*si++)
    {
        len++;
    }
    return len;
}


int
sli_strcmp(char const *s1, char const *s2)
{
    /* While not at either strings' end and they're same */
    while ((*s1 != C_NULL) && (*s2 != C_NULL) && (*s1 == *s2))
    {
        s1++;
        s2++;
    }

    /* Return string difference */
    return *s1 - *s2;
}


int
sli_strncmp(char const *s1, char const *s2, unsigned int n)
{
    unsigned int i = 0;

    if (n == 0)
    {
        return 0;
    }

    /* Scan n bytes in string */
    for (i = 0; i < n; i++)
    {
        /* If bytes differ, return difference */
        if (s1[i] != s2[i])
        {
            return s1[i] - s2[i];
        }
    }
    return 0;
}

#endif /* HAVE_STRING_H */


/*
 * This function is moved outside of HAVE_STRING_H because the one in string.h
 * will not accept a null value for the second arg
 */
void
sli_memset(unsigned char *dest, char const val, unsigned int n)
{
    unsigned int i;

    for (i = 0; i < n; i++)
    {
        *dest = (unsigned char)val;
        dest++;
    }
}

void sli_puts(uint8_t * s)
{
    uint8_t *ps = s;
    uint8_t c;

    c = *ps;
    ps++;
    while (c != '\0')
    {
        plat_putByte(c);
        c = *ps;
        ps++;
    }
}


uint8_t * sli_ltoa10(int32_t value, uint8_t *buf)
{
    int32_t const decimal_places[] = { 1000000000, 100000000, 10000000, 1000000,
                                       100000, 10000, 1000, 100, 10, 1 };
    int32_t decimal_place;
    int32_t number;
    uint8_t c;
    uint8_t printed_one = 0;
    uint8_t i;
    uint8_t j;

    number = value;
    if (number == 0)
    {
        buf[0] = '0';
        buf[1] = '\0';
        return buf;
    }

    j = 0;
    if (number < 0)
    {
        buf[0] = '-';
        j++;
        number = -number;
    }

    for (i = 0; i < 10; i++)
    {
        decimal_place = decimal_places[i];
        c = '0';
        while (number >= decimal_place)
        {
            number -= decimal_place;
            c++;
        }
        if ((c != '0') || printed_one)
        {
            buf[j++] = c;
            printed_one = 1;
        }
    }
    buf[j] = '\0';

    return buf;
}