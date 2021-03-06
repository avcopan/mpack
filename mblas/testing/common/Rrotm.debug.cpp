/*
 * Copyright (c) 2008-2010
 *	Nakata, Maho
 * 	All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 */
#include <mblas.h>
#include <blas.h>
#include <mpack_debug.h>

#if defined VERBOSE_TEST
#include <iostream>
#endif

#define MAX_INCX  10
#define MIN_INCX -10
#define MAX_INCY  10
#define MIN_INCY -10
#define MAX_N     10
#define MAX_ITER  1

REAL_REF maxdiff = 0.0;

void Rrotm_test2(INTEGER dflag)
{
    int errorflag = FALSE;
    for (int incx = MAX_INCX; incx >= MIN_INCX; incx--) {
	for (int incy = MAX_INCY; incy >= MIN_INCY; incy--) {
	    for (int n = 0; n < MAX_N; n++) {
#if defined VERBOSE_TEST
		printf("# n:%d incx:%d, incy:%d \n", n, incx, incy);
#endif
		REAL_REF *x_ref = new REAL_REF[veclen(n, incx)];
		REAL_REF *y_ref = new REAL_REF[veclen(n, incy)];
		REAL_REF *dparam_ref = new REAL_REF[veclen(5, 1)];
		REAL_REF diff;
		REAL *x = new REAL[veclen(n, incx)];
		REAL *y = new REAL[veclen(n, incy)];
		REAL *dparam = new REAL[veclen(5, 1)];

		int j = 0;

		while (j < MAX_ITER) {
		    set_random_vector(x_ref, x, veclen(n, incx));
		    set_random_vector(y_ref, y, veclen(n, incy));
		    set_random_vector(dparam_ref, dparam, veclen(5, 1));
		    dparam_ref[0] = dflag;
		    dparam[0] = dflag;

#if defined ___MPACK_BUILD_WITH_MPFR___
		    drotm_f77(&n, x_ref, &incx, y_ref, &incy, dparam_ref);
#else
		    Rrotm(n, x_ref, incx, y_ref, incy, dparam_ref);
#endif
		    Rrotm(n, x, incx, y, incy, dparam);

		    diff = infnorm(x_ref, x, veclen(n, incx), 1);
		    if (diff > EPSILON) {
			printf("error: x"); printnum(diff); printf("\n");
			errorflag = TRUE;
		    }
		    if (maxdiff < diff)
			maxdiff = diff;
		    printf("max error: "); printnum(maxdiff); printf("\n");
		    diff = infnorm(y_ref, y, veclen(n, incy), 1);
		    if (diff > EPSILON) {
			printf("error: y"); printnum(diff); printf("\n");
			errorflag = TRUE;
		    }
		    if (maxdiff < diff)
			maxdiff = diff;
		    printf("max error: "); printnum(maxdiff); printf("\n");
		    diff = infnorm(dparam_ref, dparam, 5, 1);
		    if (diff > EPSILON) {
			printf("dparam: y"); printnum(diff); printf("\n");
			errorflag = TRUE;
		    }
		    j++;
		}
		delete[]dparam;
		delete[]dparam_ref;
		delete[]x;
		delete[]y;
		delete[]x_ref;
		delete[]y_ref;
	    }

	    if (errorflag == TRUE) {
		printf("Rrotm test failed...\n");
		exit(1);
	    }
	    printf("max error: "); printnum(maxdiff); printf("\n");
	}
    }
}

void Rrotm_test()
{
    INTEGER dflag = 0;
    dflag = -1;
    Rrotm_test2(dflag);
    dflag = -2;
    Rrotm_test2(dflag);
    dflag = 0;
    Rrotm_test2(dflag);
    dflag = 1;
    Rrotm_test2(dflag);
}

int main(int argc, char *argv[])
{
    Rrotm_test();
    printf("Rrotm test passed...\n");
    return (0);
}
