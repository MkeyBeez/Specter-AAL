/******************************************************************************/
/*                                   Specter                                  */
/*                   <<Abstract Arithmetics Library Header>>                  */
/*                              George Delaportas                             */
/*                            Copyright © 2010-2025                           */
/******************************************************************************/



/* Libraries */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>
#include <string.h>

/* AAL Header Tag */
#ifndef __AAL_H__
#define __AAL_H__ 		1

/* Fixate Numbers Length - Result */
typedef struct fixlenres
{
    char Bigger;
    const char *Num1;
    const char *Num2;
    size_t FinLen;
} fixlen;

/* Read File - Output */
typedef struct rdfloutres
{
    char *Num1;
    char *Num2;
} rdflout;

/* AAL - Length */
size_t aal_len(const char *X);

/* AAL - Copy */
char *aal_copy(const char *S, size_t P);

/* AAL - Compare */
char aal_cmp(const char *A, const char *B);

/* AAL - Reverse */
char *aal_rvrs(const char *X);

/* AAL - Padding */
char *aal_pad(const char *X, const char *S);

/* AAL - Zero Check */
char aal_zrchk(const char *X);

/* AAL - Clear Initial Zeroes */
char *aal_clrizr(const char *X);

/* AAL - Minus Check (Minus Sign) */
char aal_minchk(const char *X);

/* AAL - Set Minus Sign */
char *aal_setmin(const char *X);

/* AAL - Clear Minus Sign */
char *aal_clrmin(const char *X);

/* AAL - Dot Check (Comma Sign) */
size_t aal_dotchk(const char *X);

/* AAL - Set Dot (Comma Sign) */
char *aal_setdot(const char *X, size_t P);

/* AAL - Clear Dot (Comma Sign) */
char *aal_clrdot(const char *X);

/* AAL - Fixate Numbers Length */
fixlen aal_fixlen(const char *A, const char *B);

/* AAL - Read File */
rdflout aal_rdfl(const char *Z);

/* AAL - Error Check (1 Argument) */
char aal_errchk_1(const char *X);

/* AAL - Error Check (2 Arguments) */
char aal_errchk_2(const char *A, const char *B);

/* AAL - Memory Allocator (1 Argument) */
char *aal_mem_alloc_num(size_t V);

/* AAL - Memory Allocator (1 Argument) */
char *aal_mem_alloc_1(const char *A);

/* AAL - Memory Allocator (2 Arguments) */
char *aal_mem_alloc_2(const char *A, const char *B);

/* AAL - Memory Deallocator */
int aal_mem_dealloc(char *S);

/* AAL - Addition */
char *aal_add(const char *A, const char *B);

/* AAL - Subtraction */
char *aal_sub(const char *A, const char *B);

#endif /* __AAL_H__ */

/******************************************************************************/
