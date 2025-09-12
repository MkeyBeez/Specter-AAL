/******************************************************************************/
/*                                   Specter                                  */
/*                              George Delaportas                             */
/*                            Copyright © 2010-2025                           */
/*                         Refactored by Martin Tannhaus                      */
/******************************************************************************/

#ifndef AAL_H
#define AAL_H

// BigFloat structure for arbitrary precision decimal arithmetic
typedef struct {
    char *digits;   // only digits, no decimal point
    int scale;      // number of fractional digits
    int sign;       // +1 or -1
} BigFloat;

// Core BigFloat operations
BigFloat parseBigFloat(const char* s);
char* formatBigFloat(BigFloat bf);

// Arithmetic operations
BigFloat addBigFloat(BigFloat a, BigFloat b);
BigFloat subBigFloat(BigFloat a, BigFloat b);
BigFloat mulBigFloat(BigFloat a, BigFloat b);
BigFloat divBigFloat(BigFloat a, BigFloat b, int precision);
BigFloat modBigFloat(BigFloat a, BigFloat b);

// Utility functions for digit string operations
int compareDigits(const char* a, const char* b);
char* addDigits(const char* a, const char* b);
char* subDigits(const char* a, const char* b);
char* mulDigits(const char* a, const char* b);
char* divDigits(const char* a, const char* b, int precision);
char* modDigits(const char* a, const char* b);

// Helper functions
char* stripLeadingZeros(char* str);
void reverse(char* str);

#endif // AAL_H
