/******************************************************************************/
/*                                   Specter                                  */
/*                   <<Abstract Arithmetics Library Header>>                  */
/*                              George Delaportas                             */
/*                            Copyright © 2010-2025                           */
/******************************************************************************/

#ifndef AAL_H
#define AAL_H

/* Libraries */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

/* AAL Header Tag */
#define __AAL_H__ 1

/* Return codes */
typedef enum {
    AAL_SUCCESS = 0,
    AAL_ERROR_NULL_POINTER = -1,
    AAL_ERROR_INVALID_INPUT = -2,
    AAL_ERROR_MEMORY_ALLOCATION = -3,
    AAL_ERROR_FILE_IO = -4
} aal_error_t;

/* Comparison results */
typedef enum {
    AAL_EQUAL = 0,
    AAL_FIRST_GREATER = 1,
    AAL_SECOND_GREATER = 2
} aal_comparison_t;

/* Fixate Numbers Length - Result */
typedef struct {
    aal_comparison_t bigger;
    char *num1;
    char *num2;
    size_t final_length;
} aal_fixlen_t;

/* Read File - Output */
typedef struct {
    char *num1;
    char *num2;
    aal_error_t error;
} aal_rdfile_t;

/* AAL - Length */
size_t aal_length(const char *str);

/* AAL - Copy substring starting at position */
char *aal_copy_from_position(const char *source, size_t position);

/* AAL - Compare two number strings */
aal_comparison_t aal_compare(const char *a, const char *b);

/* AAL - Reverse string */
char *aal_reverse(const char *str);

/* AAL - Pad string with prefix */
char *aal_pad_prefix(const char *str, const char *prefix);

/* AAL - Check if string represents zero */
bool aal_is_zero(const char *str);

/* AAL - Clear leading zeros */
char *aal_clear_leading_zeros(const char *str);

/* AAL - Check for minus sign */
bool aal_has_minus_sign(const char *str);

/* AAL - Add minus sign */
char *aal_set_minus_sign(const char *str);

/* AAL - Remove minus sign */
char *aal_clear_minus_sign(const char *str);

/* AAL - Find decimal point position */
int aal_find_decimal_point(const char *str);

/* AAL - Insert decimal point at position */
char *aal_set_decimal_point(const char *str, size_t position);

/* AAL - Remove decimal point */
char *aal_clear_decimal_point(const char *str);

/* AAL - Equalize string lengths with padding */
aal_fixlen_t aal_equalize_lengths(const char *a, const char *b);

/* AAL - Read two numbers from file */
aal_rdfile_t aal_read_file(const char *filename);

/* AAL - Validate single number string */
aal_error_t aal_validate_number(const char *str);

/* AAL - Validate two number strings */
aal_error_t aal_validate_two_numbers(const char *a, const char *b);

/* AAL - Memory allocation helpers */
char *aal_allocate_string(size_t length);
char *aal_allocate_like_string(const char *str);
char *aal_allocate_for_two_strings(const char *a, const char *b);

/* AAL - Memory deallocation */
void aal_free_string(char *str);
void aal_free_fixlen_result(aal_fixlen_t *result);
void aal_free_rdfile_result(aal_rdfile_t *result);

/* AAL - Arithmetic operations */
char *aal_add(const char *a, const char *b);
char *aal_subtract(const char *a, const char *b);

#endif /* AAL_H */

/******************************************************************************/
