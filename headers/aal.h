#ifndef AAL_H
#define AAL_H

/* Libraries */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>
#include <string.h>

/* Fixate Numbers Length - Result */
typedef struct {
    char Bigger;        /* '0' if equal length, '1' if A bigger, '2' if B bigger */
    char *Num1;         /* Adjusted first number string */
    char *Num2;         /* Adjusted second number string */
    uintptr_t FinLen;   /* Final length after padding */
} fixlen;

/* Read File - Output */
typedef struct {
    char *Num1;
    char *Num2;
} rdflout;

/* Core string utilities */
uintptr_t aal_len(const char *X);
char     *aal_copy(const char *S, unsigned long P);
char      aal_cmp(const char *A, const char *B);
char     *aal_rvrs(const char *X);
char     *aal_pad(const char *X, const char *S);

/* Numeric checks */
char      aal_zrchk(const char *X);
char     *aal_clrizr(const char *X);
char      aal_minchk(const char *X);
char     *aal_setmin(const char *X);
char     *aal_clrmin(const char *X);

/* Dot (decimal) utilities */
uintptr_t aal_dotchk(const char *X);
char     *aal_setdot(const char *X, size_t pos);
char     *aal_clrdot(const char *X);

/* Alignment helpers */
fixlen    aal_fixlen(const char *A, const char *B);

/* File input */
rdflout   aal_rdfl(const char *Z);

/* Error checking */
char      aal_errchk_1(const char *X);
char      aal_errchk_2(const char *A, const char *B);

/* Memory management */
char     *aal_mem_alloc_num(size_t V);
char     *aal_mem_alloc_1(const char *A);
char     *aal_mem_alloc_2(const char *A, const char *B);
int       aal_mem_dealloc(char *S);

/* Arithmetic */
char     *aal_add(const char *A, const char *B);
char     *aal_sub(const char *A, const char *B);

#endif /* AAL_H */
