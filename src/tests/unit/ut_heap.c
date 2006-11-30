/*
 * PyMite - A flyweight Python interpreter for 8-bit microcontrollers and more.
 * Copyright 2002 Dean Hall
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */


/**
 * Heap Unit Tests
 *
 * Tests the Heap implementation.
 *
 * Log
 * ---
 *
 * 2006/11/21   First.
 */


#include "CuTest.h"
#include "pm.h"


/**
 * Tests heap_init():
 *      retval is OK
 *      avail size is greater than zero
 */
void
ut_heap_init_000(CuTest *tc)
{
    uint16_t avail;
    PmReturn_t retval;

    retval = heap_init();

    CuAssertTrue(tc, retval == PM_RET_OK);

    retval = heap_getAvail(&avail);
    CuAssertTrue(tc, avail > 0);
}


/**
 * Tests heap_getChunk():
 *      retval is OK
 *      return arg pointer is non-null
 *      return chunk is of requested size (rounded up to next multiple of four)
 */
void
ut_heap_getChunk_000(CuTest *tc)
{
    uint8_t *pchunk;
    PmReturn_t retval;
    pPmObj_t pobj;

    retval = heap_init();
    retval = heap_getChunk(8, &pchunk);
    pobj = (pPmObj_t)pchunk;

    CuAssertTrue(tc, retval == PM_RET_OK);
    CuAssertPtrNotNull(tc, pchunk);
    CuAssertTrue(tc, OBJ_GET_SIZE(*pobj) == 8);
}


/**
 * Tests heap_getAvail():
 *      retval is OK
 *      avail is reduced by amount obtained in getChunk
 */
void
ut_heap_getAvail_000(CuTest *tc)
{
    uint16_t avail1;
    uint16_t avail2;
    uint8_t *pchunk;
    PmReturn_t retval;

    retval = heap_init();
    retval = heap_getAvail(&avail1);
    CuAssertTrue(tc, retval == PM_RET_OK);

    retval = heap_getChunk(16, &pchunk);
    retval = heap_getAvail(&avail2);
    CuAssertTrue(tc, (avail1 - avail2) == 16);
}


/**
 * Tests heap_freeChunk():
 *      retval is OK
 *      avail is increased by amount obtained in freeChunk
 */
void
ut_heap_freeChunk_000(CuTest *tc)
{
    uint16_t avail1;
    uint16_t avail2;
    uint8_t *pchunk;
    PmReturn_t retval;

    retval = heap_init();
    retval = heap_getChunk(16, &pchunk);
    retval = heap_getAvail(&avail1);
    retval = heap_freeChunk((pPmObj_t)pchunk);
    CuAssertTrue(tc, retval == PM_RET_OK);

    retval = heap_getAvail(&avail2);
    CuAssertTrue(tc, (avail2 - avail1) == 16);
}


/**
 * Tests heap_sweep():
 *      retval is OK
 *      avail returns to pre-allocated value after sweep
 */
void
ut_heap_sweep_000(CuTest *tc)
{
    uint16_t avail1;
    uint16_t avail2;
    uint8_t *pchunk;
    PmReturn_t retval;

    retval = heap_init();
    retval = heap_getAvail(&avail1);
    retval = heap_getChunk(16, &pchunk);
    retval = heap_getChunk(32, &pchunk);
    retval = heap_getChunk(24, &pchunk);
    retval = heap_getAvail(&avail2);
    CuAssertTrue(tc, avail2 != avail1);

    retval = heap_collectGarbage();
    CuAssertTrue(tc, retval == PM_RET_OK);

    retval = heap_getAvail(&avail2);
    CuAssertTrue(tc, avail2 == avail1);
}


/** Make a suite from all tests in this file */
CuSuite *getSuite_testHeap(void)
{
	CuSuite* suite = CuSuiteNew();

	SUITE_ADD_TEST(suite, ut_heap_init_000);
	SUITE_ADD_TEST(suite, ut_heap_getChunk_000);
	SUITE_ADD_TEST(suite, ut_heap_getAvail_000);
	SUITE_ADD_TEST(suite, ut_heap_freeChunk_000);
	SUITE_ADD_TEST(suite, ut_heap_sweep_000);

    return suite;
}
