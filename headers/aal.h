/******************************************************************************/
/*                                   Specter                                  */
/*                   <<Abstract Arithmetics Library Header>>                  */
/*                              George Delaportas                             */
/*                            Copyright © 2010-2025                           */
/******************************************************************************/

/* AAL Header Tag */
#ifndef __AAL_H__
#define __AAL_H__ 1

/* Libraries */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include <stddef.h>

/* Comparison result enum */
typedef enum {
    CMP_EQUAL = 0,
    CMP_A_GREATER = 1, 
    CMP_B_GREATER = 2
} aal_cmp_result;

/* Fixate Numbers Length - Result */
typedef struct fixlenres
{
    char bigger;         /* Updated field name */
    char *num1;          /* Updated field name, changed from const char* to char* since they may be allocated */
    char *num2;          /* Updated field name, changed from const char* to char* since they may be allocated */
    uintptr_t finalLen;  /* Updated field name, changed from size_t to uintptr_t for consistency */
} fixlen;

/* Read File - Output */
typedef struct rdfloutres
{
    char *Num1;
    char *Num2;
} rdflout;

/* Function Declarations */

/* AAL - Length */
uintptr_t aal_len(char *X);

/* AAL - Copy */
char *aal_copy(char *S, unsigned long P);

/* AAL - Compare - Updated to return aal_cmp_result */
aal_cmp_result aal_cmp(char *A, char *B);

/* AAL - Reverse */
char *aal_rvrs(char *X);

/* AAL - Padding */
char *aal_pad(char *X, char *S);

/* AAL - Zero Check - Updated to return bool */
bool aal_zrchk(char *X);

/* AAL - Clear Initial Zeroes */
char *aal_clrizr(char *X);

/* AAL - Minus Check (Minus Sign) - Updated to return bool */
bool aal_minchk(char *X);

/* AAL - Set Minus Sign */
char *aal_setmin(char *X);

/* AAL - Clear Minus Sign */
char *aal_clrmin(char *X);

/* AAL - Dot Check (Comma Sign) */
uintptr_t aal_dotchk(char *X);

/* AAL - Set Dot (Comma Sign) */
char *aal_setdot(char *X, char *P);

/* AAL - Clear Dot (Comma Sign) */
char *aal_clrdot(char *X);

/* AAL - Fixate Numbers Length */
fixlen aal_fixlen(char *A, char *B);

/* AAL - Read File */
rdflout aal_rdfl(const char *Z);

/* AAL - Error Check (1 Argument) - Updated to return bool */
bool aal_errchk_1(char *X);

/* AAL - Error Check (2 Arguments) - Updated to return bool */
bool aal_errchk_2(char *A, char *B);

/* AAL - Memory Allocator (Numeric) */
char *aal_mem_alloc_num(unsigned long V);

/* AAL - Memory Allocator (1 Argument) */
char *aal_mem_alloc_1(char *A);

/* AAL - Memory Allocator (2 Arguments) */
char *aal_mem_alloc_2(char *A, char *B);

/* AAL - Memory Deallocator */
int aal_mem_dealloc(char *S);

/* AAL - Addition */
char *aal_add(char *A, char *B);

/* AAL - Subtraction */
char *aal_sub(char *A, char *B);

/* Helper function declarations for decimal arithmetic */
char *aal_add_integers(char *A, char *B);
char *normalize_for_decimal_arithmetic(char *num, uintptr_t targetDecimalPlaces, bool hasDecimal);
char *insert_decimal_point(char *intResult, uintptr_t decimalPlaces);

#endif /* __AAL_H__ */
/******************************************************************************/
