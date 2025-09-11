/******************************************************************************/
/*                                   Specter                                  */
/*                      <<Abstract Arithmetics Library>>                      */
/*                              George Delaportas                             */
/*                            Copyright © 2010-2025                           */
/******************************************************************************/

/* Headers */
#include "headers/aal.h"

/* AAL - Length */
uintptr_t aal_len(char *X)
{
    if (X == NULL) {
        return (uintptr_t)-1;  // Cast -1 to uintptr_t for consistency
    }
    
    char *P = X;  // C99: declare variables at point of first use
    
    while (*P != '\0') {
        P++;  // Fixed: removed incorrect dereference operator
    }
    
    return (uintptr_t)(P - X);  // Cast to uintptr_t for type consistency
}

/* AAL - Copy */
char *aal_copy(char *S, unsigned long P)
{
    char *CRes = aal_mem_alloc_1(S);
    if (CRes == NULL) {
        return NULL;  // Handle allocation failure gracefully
    }
    
    char *N = S;  // C99: declare at point of first use
    uintptr_t LenS = aal_len(S);
    
    while (((P + (uintptr_t)(N - S)) < LenS) && 
           (isdigit(S[P + (N - S)]) || S[P + (N - S)] == '-' || S[P + (N - S)] == '.')) {
        
        CRes[N - S] = S[P + (N - S)];
        N++;  // Fixed: removed incorrect dereference operator
    }
    
    CRes[N - S] = '\0';
    
    return CRes;
}

/* AAL - Compare */
aal_cmp_result aal_cmp(char *A, char *B)
{
    aal_cmp_result result = CMP_EQUAL;
    
    A = aal_clrizr(A);
    B = aal_clrizr(B);
    
    bool zeroA = aal_zrchk(A);
    bool zeroB = aal_zrchk(B);
    
    if (zeroA && zeroB) {
        return result;
    }
    
    uintptr_t LenA = aal_len(A);
    uintptr_t LenB = aal_len(B);
    
    bool minusA = aal_minchk(A);
    bool minusB = aal_minchk(B);
    
    if (LenA > LenB) {
        result = minusA ? CMP_B_GREATER : CMP_A_GREATER;
    } else if (LenA < LenB) {
        result = minusB ? CMP_A_GREATER : CMP_B_GREATER;
    } else {
        char *P = A;
        
        while (*P != '\0') {
            if (A[P - A] > B[P - A]) {
                result = minusA ? CMP_B_GREATER : CMP_A_GREATER;
                break;
            } else if (A[P - A] < B[P - A]) {
                result = minusB ? CMP_A_GREATER : CMP_B_GREATER;
                break;
            }
            P++;
        }
    }
    
    return result;
}

/* AAL - Reverse */
char *aal_rvrs(char *X)
{
    char *RevStr = aal_mem_alloc_1(X);
    if (RevStr == NULL) {
        return NULL;  // Handle allocation failure
    }
    
    uintptr_t LenX = aal_len(X);
    
    if (LenX > 1) {
        char *P = X;  // C99: declare at point of first use
        
        while (*P != '\0') {
            RevStr[P - X] = X[LenX - (P - X) - 1];
            P++;  // Fixed: removed incorrect dereference operator
        }
    } else {
        // Copy the single character or empty string instead of reassigning pointer
        // This preserves the original behavior while avoiding memory leak
        if (LenX == 1) {
            RevStr[0] = X[0];
        }
        // For empty string (LenX == 0), RevStr is already allocated and will be null-terminated below
    }
    
    RevStr[LenX] = '\0';
    
    return RevStr;
}

/* AAL - Padding */
char *aal_pad(char *X, char *S)
{
    // Add NULL checks before calling aal_len
    if (X == NULL || S == NULL) {
        return NULL;
    }
    
    uintptr_t LenX = aal_len(X);  // C99: declare at point of first use
    uintptr_t LenS = aal_len(S);
    
    // Check if aal_len returned error values
    if (LenX == (uintptr_t)-1 || LenS == (uintptr_t)-1) {
        return NULL;
    }
    
    char *PadStr = aal_mem_alloc_num(LenX + LenS);
    if (PadStr == NULL) {
        return NULL;  // Handle allocation failure
    }
    
    // Copy string S to the beginning of PadStr
    char *P = S;  // C99: declare at point of first use
    while (*P != '\0') {
        PadStr[P - S] = S[P - S];
        P++;  // Fixed: removed incorrect dereference operator
    }
    
    // Copy string X after S in PadStr
    P = X;
    while (*P != '\0') {
        PadStr[LenS + (P - X)] = X[P - X];
        P++;  // Fixed: removed incorrect dereference operator
    }
    
    PadStr[LenX + LenS] = '\0';
    
    return PadStr;
}

/* AAL - Zero Check - Updated to return bool */
bool aal_zrchk(char *X)
{
    if (X == NULL) {
        return false;
    }
    
    char *CleanX = aal_clrmin(X);
    if (CleanX == NULL) {
        return false;
    }
    
    uintptr_t LenX = aal_len(CleanX);
    uintptr_t zeroCount = 0;
    
    char *P = CleanX;
    while (*P != '\0') {
        if (CleanX[P - CleanX] == '0') {
            zeroCount++;
        }
        P++;
    }
    
    aal_mem_dealloc(CleanX);
    
    return (zeroCount == LenX);
}

/* AAL - Clear Initial Zeroes - Updated with bool logic */
char *aal_clrizr(char *X)
{
    if (X == NULL) {
        return NULL;
    }
    
    char *ClrStr = aal_mem_alloc_1(X);
    if (ClrStr == NULL) {
        return NULL;
    }
    
    bool foundNonZero = false;
    char *P = X;
    
    while (*P != '\0') {
        if (X[P - X] != '0' && (isdigit(X[P - X]) || X[P - X] == '-' || X[P - X] == '.')) {
            aal_mem_dealloc(ClrStr);
            ClrStr = aal_copy(X, (uintptr_t)(P - X));
            foundNonZero = true;
            break;
        }
        P++;
    }
    
    if (foundNonZero && aal_dotchk(ClrStr) == 0) {
        char *Temp = aal_pad(ClrStr, "0");
        aal_mem_dealloc(ClrStr);
        ClrStr = Temp;
    }
    
    if (!foundNonZero) {
        aal_mem_dealloc(ClrStr);
        ClrStr = aal_mem_alloc_num(2);
        if (ClrStr != NULL) {
            ClrStr[0] = '0';
            ClrStr[1] = '\0';
        }
    }
    
    return ClrStr;
}

/* AAL - Minus Check - Updated to return bool */
bool aal_minchk(char *X)
{
    if (X == NULL || X[0] == '\0') {
        return false;
    }
    
    return (X[0] == '-');
}

/* AAL - Set Minus Sign */
char *aal_setmin(char *X)
{
    if (X == NULL) {
        return NULL;
    }
    
    uintptr_t LenX = aal_len(X);  // Get length first for proper allocation
    char *FinNum = aal_mem_alloc_num(LenX + 2);  // Allocate for original + minus + null terminator
    if (FinNum == NULL) {
        return NULL;  // Handle allocation failure
    }
    
    FinNum[0] = '-';
    
    char *P = X;  // C99: declare at point of first use
    while (*P != '\0') {
        FinNum[(P - X) + 1] = X[P - X];
        P++;  // Fixed: removed incorrect dereference operator
    }
    
    FinNum[(P - X) + 1] = '\0';
    
    return FinNum;
}

/* AAL - Clear Minus Sign */
char *aal_clrmin(char *X)
{
    if (X == NULL) {
        return NULL;
    }
    
    if (aal_minchk(X) == '0') {
        // No minus sign, create a copy of the original string
        uintptr_t LenX = aal_len(X);
        char *AbsNum = aal_mem_alloc_num(LenX + 1);
        if (AbsNum != NULL) {
            strcpy(AbsNum, X);
        }
        return AbsNum;
    }
    
    // Has minus sign, allocate memory for the string without the minus sign
    uintptr_t LenX = aal_len(X);
    char *AbsNum = aal_mem_alloc_num(LenX);  // One less character (no minus sign)
    if (AbsNum == NULL) {
        return NULL;
    }
    
    char *P = X + 1;  // Start from second character (skip minus sign)
    uintptr_t index = 0;
    
    while (*P != '\0') {
        AbsNum[index] = *P;
        P++;
        index++;
    }
    
    AbsNum[index] = '\0';
    
    return AbsNum;
}

/* AAL - Dot Check - Updated with proper return values */
uintptr_t aal_dotchk(char *X)
{
    if (X == NULL) {
        return (uintptr_t)-1;
    }
    
    char *P = X;
    
    while (*P != '\0') {
        if (X[P - X] == '.') {
            return (uintptr_t)(P - X);
        }
        P++;
    }
    
    return (uintptr_t)-1;
}

/* AAL - Set Dot (Comma Sign) */
char *aal_setdot(char *X, char *P)
{
    if (X == NULL || P == NULL) {
        return NULL;
    }
    
    uintptr_t LenX = aal_len(X);
    char *RealNum = aal_mem_alloc_num(LenX + 2);  // +1 for dot, +1 for null terminator
    if (RealNum == NULL) {
        return NULL;
    }
    
    uintptr_t dotPosition = (uintptr_t)*P;  // Position where to insert the dot
    char *N = X;  // C99: declare at point of first use
    
    // Copy characters before the dot position
    while ((uintptr_t)(N - X) != dotPosition) {
        RealNum[N - X] = X[N - X];
        N++;  // Fixed: removed incorrect dereference operator
    }
    
    // Insert the dot
    RealNum[N - X] = '.';
    
    // Move to next position for continuing the copy
    uintptr_t dotIndex = (uintptr_t)(N - X);
    
    // Copy remaining characters after the dot
    while (*N != '\0') {
        RealNum[dotIndex + 1 + (N - X - dotIndex)] = X[N - X];
        N++;  // Fixed: removed incorrect dereference operator
    }
    
    RealNum[dotIndex + 1 + (N - X - dotIndex)] = '\0';
    
    return RealNum;
}

/* AAL - Clear Dot (Comma Sign) */
char *aal_clrdot(char *X)
{
    if (X == NULL) {
        return NULL;
    }
    
    // Check if there's actually a dot to remove
    uintptr_t dotPos = aal_dotchk(X);
    if (dotPos == (uintptr_t)-1) {
        // No dot found, return a copy of the original string
        uintptr_t LenX = aal_len(X);
        char *IntNum = aal_mem_alloc_num(LenX + 1);
        if (IntNum != NULL) {
            strcpy(IntNum, X);
        }
        return IntNum;
    }
    
    uintptr_t LenX = aal_len(X);
    char *IntNum = aal_mem_alloc_num(LenX);  // One less character (no dot)
    if (IntNum == NULL) {
        return NULL;
    }
    
    char *P = X;  // C99: declare at point of first use
    uintptr_t writeIndex = 0;
    
    // Copy characters before the dot
    while (*P != '.') {
        IntNum[writeIndex] = *P;
        P++;  // Fixed: removed incorrect dereference operator
        writeIndex++;
    }
    
    // Skip the dot
    P++;
    
    // Copy characters after the dot
    while (*P != '\0') {
        IntNum[writeIndex] = *P;
        P++;  // Fixed: removed incorrect dereference operator  
        writeIndex++;
    }
    
    IntNum[writeIndex] = '\0';
    
    return IntNum;
}

/* AAL - Fixate Numbers Length - Updated with cleaner logic */
fixlen aal_fixlen(char *A, char *B)
{
    fixlen result = {0};
    
    if (A == NULL || B == NULL) {
        return result;
    }
    
    uintptr_t LenA = aal_len(A);
    uintptr_t LenB = aal_len(B);
    
    if (LenA > LenB) {
        uintptr_t DifLen = LenA - LenB;
        char *padding = aal_mem_alloc_num(DifLen + 1);
        if (padding == NULL) {
            return result;
        }
        
        for (uintptr_t i = 0; i < DifLen; i++) {
            padding[i] = '0';
        }
        padding[DifLen] = '\0';
        
        result.bigger = '1';
        result.num1 = A;
        result.num2 = aal_pad(B, padding);
        result.finalLen = LenA;
        
        aal_mem_dealloc(padding);
    } else if (LenA < LenB) {
        uintptr_t DifLen = LenB - LenA;
        char *padding = aal_mem_alloc_num(DifLen + 1);
        if (padding == NULL) {
            return result;
        }
        
        for (uintptr_t i = 0; i < DifLen; i++) {
            padding[i] = '0';
        }
        padding[DifLen] = '\0';
        
        result.bigger = '2';
        result.num1 = aal_pad(A, padding);
        result.num2 = B;
        result.finalLen = LenB;
        
        aal_mem_dealloc(padding);
    } else {
        result.bigger = '0';
        result.num1 = A;
        result.num2 = B;
        result.finalLen = LenA;
    }
    
    return result;
}

/* AAL - Read File */
rdflout aal_rdfl(const char *Z)
{
    rdflout locrdflout = {0};  // C99: initialize struct to zero
    
    if (Z == NULL) {
        locrdflout.num1 = "ERROR";
        locrdflout.num2 = "ERROR";
        return locrdflout;
    }
    
    FILE *FP = fopen(Z, "r");  // C99: declare at point of first use
    if (FP == NULL) {
        locrdflout.num1 = "ERROR";
        locrdflout.num2 = "ERROR";
        return locrdflout;
    }
    
    // Get file size
    if (fseek(FP, 0, SEEK_END) != 0) {
        locrdflout.num1 = "ERROR";
        locrdflout.num2 = "ERROR";
        fclose(FP);
        return locrdflout;
    }
    
    long flsz = ftell(FP);  // Use long for ftell return type
    if (flsz < 0) {
        locrdflout.num1 = "ERROR";
        locrdflout.num2 = "ERROR";
        fclose(FP);
        return locrdflout;
    }
    
    rewind(FP);
    
    if ((unsigned long)flsz < 3) {
        locrdflout.num1 = "ERROR";
        locrdflout.num2 = "ERROR";
        fclose(FP);
        return locrdflout;
    }
    
    char *FlStrm = aal_mem_alloc_num((unsigned long)flsz + 1);  // +1 for null terminator
    if (FlStrm == NULL) {
        locrdflout.num1 = "ERROR";
        locrdflout.num2 = "ERROR";
        fclose(FP);
        return locrdflout;
    }
    
    size_t file_res = fread(FlStrm, 1, (unsigned long)flsz, FP);
    FlStrm[file_res] = '\0';  // Null terminate the string
    
    if (file_res == 0) {
        locrdflout.num1 = "ERROR";
        locrdflout.num2 = "ERROR";
        aal_mem_dealloc(FlStrm);
        fclose(FP);
        return locrdflout;
    }
    
    // Find the colon separator
    unsigned long i = 0;  // C99: declare at point of first use
    bool colonFound = false;
    
    while (i < file_res && FlStrm[i] != '\0') {
        if (FlStrm[i] == ':') {
            colonFound = true;
            break;
        }
        i++;
    }
    
    if (!colonFound) {
        locrdflout.num1 = "ERROR";
        locrdflout.num2 = "ERROR";
    } else {
        locrdflout.num1 = aal_copy(FlStrm, 0);
        locrdflout.num2 = aal_copy(FlStrm, i + 1);
    }
    
    aal_mem_dealloc(FlStrm);
    fclose(FP);
    
    return locrdflout;
}

/* AAL - Error Check (1 Argument) - Updated to return bool */
bool aal_errchk_1(char *X)
{
    if (X == NULL) {
        return true;  // Error for NULL input
    }
    
    uintptr_t LenX = aal_len(X);
    if (LenX == 0) {
        return true;  // Error for empty string
    }
    
    bool dotFound = false;
    char *P = X;
    
    while (*P != '\0') {
        if (!isdigit(X[P - X])) {
            uintptr_t position = (uintptr_t)(P - X);
            
            if ((position == 0 && X[P - X] != '-') ||              // First char must be digit or minus
                (position > 0 && X[P - X] != '.') ||               // Non-first chars must be digit or dot
                (position == LenX - 1 && X[P - X] == '.') ||      // Last char cannot be dot
                (X[P - X] == '.' && dotFound)) {                  // Only one dot allowed
                
                return true;  // Error found
            }
            
            if (X[P - X] == '.') {
                dotFound = true;
            }
        }
        P++;
    }
    
    return false;  // No error found
}

/* AAL - Error Check (2 Arguments) - Updated to return bool */
bool aal_errchk_2(char *A, char *B)
{
    return (aal_errchk_1(A) || aal_errchk_1(B));
}

/* AAL - Memory Allocator (1 Argument) */
char *aal_mem_alloc_num(unsigned long V)
{
    if (V == 0) {
        return NULL;  // Handle zero-size allocation request
    }
    
    char *alloc_mem = calloc(V, sizeof(char));  // Fixed: sizeof(char) instead of sizeof(alloc_mem)
    
    return alloc_mem;  // Returns NULL if allocation fails, valid pointer if successful
}

/* AAL - Memory Allocator (1 Argument) */
char *aal_mem_alloc_1(char *A)
{
    if (A == NULL) {
        return NULL;  // Handle NULL input
    }
    
    uintptr_t len = aal_len(A);
    if (len == (uintptr_t)-1) {  // aal_len returns -1 for NULL input
        return NULL;
    }
    
    // Allocate space for string length + null terminator
    char *alloc_mem = calloc(len + 1, sizeof(char));  // Fixed: sizeof(char) and +1 for null terminator
    
    return alloc_mem;  // Returns NULL if allocation fails, valid pointer if successful
}

/* AAL - Memory Allocator (2 Arguments) */
char *aal_mem_alloc_2(char *A, char *B)
{
    if (A == NULL || B == NULL) {
        return NULL;  // Handle NULL inputs
    }
    
    uintptr_t LenA = aal_len(A);
    uintptr_t LenB = aal_len(B);
    
    // Handle error returns from aal_len
    if (LenA == (uintptr_t)-1 || LenB == (uintptr_t)-1) {
        return NULL;
    }
    
    uintptr_t allocSize;  // C99: declare at point of first use
    
    if (LenA >= LenB) {
        allocSize = LenA + 2;  // Original logic: add 2 when A is longer
    } else {
        allocSize = LenB + 1;  // Fixed: add 1 for null terminator consistency
    }
    
    char *alloc_mem = calloc(allocSize, sizeof(char));  // Use sizeof(char) for clarity
    
    return alloc_mem;  // Returns NULL if allocation fails, valid pointer if successful
}

/* AAL - Memory Deallocator */
int aal_mem_dealloc(char *S)
{
    if (S == NULL) {
        return 0;  // Return 0 for NULL pointer (no operation performed)
    }
    
    free(S);
    
    return 1;  // Return 1 for successful deallocation
}

/* Helper function: Normalize number for decimal arithmetic */
char *normalize_for_decimal_arithmetic(char *num, uintptr_t targetDecimalPlaces, bool hasDecimal)
{
    if (num == NULL) return NULL;
    
    char *result;
    
    if (!hasDecimal) {
        // No decimal point, just pad with zeros at the end
        uintptr_t originalLen = aal_len(num);
        result = aal_mem_alloc_num(originalLen + targetDecimalPlaces + 1);
        if (result == NULL) return NULL;
        
        strcpy(result, num);
        // Pad with zeros
        for (uintptr_t i = 0; i < targetDecimalPlaces; i++) {
            result[originalLen + i] = '0';
        }
        result[originalLen + targetDecimalPlaces] = '\0';
    } else {
        // Has decimal point, remove it and pad as needed
        char *withoutDot = aal_clrdot(num);
        if (withoutDot == NULL) return NULL;
        
        uintptr_t dotPos = aal_dotchk(num);
        uintptr_t originalLen = aal_len(num);
        uintptr_t currentDecimalPlaces = originalLen - dotPos - 1;
        
        if (currentDecimalPlaces >= targetDecimalPlaces) {
            // Already has enough decimal places, just remove dot
            result = withoutDot;
        } else {
            // Need to pad with zeros
            uintptr_t paddingNeeded = targetDecimalPlaces - currentDecimalPlaces;
            uintptr_t withoutDotLen = aal_len(withoutDot);
            
            result = aal_mem_alloc_num(withoutDotLen + paddingNeeded + 1);
            if (result == NULL) {
                aal_mem_dealloc(withoutDot);
                return NULL;
            }
            
            strcpy(result, withoutDot);
            // Pad with zeros
            for (uintptr_t i = 0; i < paddingNeeded; i++) {
                result[withoutDotLen + i] = '0';
            }
            result[withoutDotLen + paddingNeeded] = '\0';
            
            aal_mem_dealloc(withoutDot);
        }
    }
    
    return result;
}

/* Helper function: Insert decimal point into integer result */
char *insert_decimal_point(char *intResult, uintptr_t decimalPlaces)
{
    if (intResult == NULL || decimalPlaces == 0) {
        // No decimal places needed, return copy of integer result
        if (intResult == NULL) return NULL;
        uintptr_t len = aal_len(intResult);
        char *result = aal_mem_alloc_num(len + 1);
        if (result != NULL) {
            strcpy(result, intResult);
        }
        return result;
    }
    
    uintptr_t intLen = aal_len(intResult);
    
    if (intLen <= decimalPlaces) {
        // Need to pad with leading zeros
        uintptr_t zerosNeeded = decimalPlaces - intLen + 1; // +1 for the leading zero before decimal
        char *result = aal_mem_alloc_num(intLen + zerosNeeded + 2); // +1 for dot, +1 for null
        if (result == NULL) return NULL;
        
        result[0] = '0';
        result[1] = '.';
        
        // Add padding zeros
        for (uintptr_t i = 0; i < zerosNeeded - 1; i++) {
            result[2 + i] = '0';
        }
        
        // Add the actual digits
        strcpy(result + 1 + zerosNeeded, intResult);
        
        return result;
    } else {
        // Insert decimal point within the number
        uintptr_t dotPosition = intLen - decimalPlaces;
        char *result = aal_mem_alloc_num(intLen + 2); // +1 for dot, +1 for null
        if (result == NULL) return NULL;
        
        // Copy integer part
        for (uintptr_t i = 0; i < dotPosition; i++) {
            result[i] = intResult[i];
        }
        
        // Insert decimal point
        result[dotPosition] = '.';
        
        // Copy fractional part
        for (uintptr_t i = dotPosition; i < intLen; i++) {
            result[i + 1] = intResult[i];
        }
        
        result[intLen + 1] = '\0';
        
        return result;
    }
}

/* Integer-only addition */
char *aal_add_integers(char *A, char *B)
{
    // Get fixed-length versions of both numbers
    fixlen newfxlnrs = aal_fixlen(A, B);
    if (newfxlnrs.num1 == NULL || newfxlnrs.num2 == NULL) {
        return NULL;
    }
    
    uintptr_t len = newfxlnrs.finalLen;
    
    // Allocate memory for result (may need one extra digit for carry)
    char *Result = aal_mem_alloc_num(len + 2);
    if (Result == NULL) {
        return NULL;
    }
    
    int carry = 0;
    uintptr_t resultPos = 0;
    
    // Process digits from right to left
    for (uintptr_t i = 0; i < len; i++) {
        char digitA = newfxlnrs.num1[len - 1 - i];
        char digitB = newfxlnrs.num2[len - 1 - i];
        
        int sum = (digitA - '0') + (digitB - '0') + carry;
        carry = sum / 10;
        int digit = sum % 10;
        
        Result[resultPos] = (char)(digit + '0');
        resultPos++;
    }
    
    if (carry > 0) {
        Result[resultPos] = (char)(carry + '0');
        resultPos++;
    }
    
    Result[resultPos] = '\0';
    
    // Reverse and clean up
    char *ReversedResult = aal_rvrs(Result);
    aal_mem_dealloc(Result);
    
    if (newfxlnrs.num1 != A) aal_mem_dealloc(newfxlnrs.num1);
    if (newfxlnrs.num2 != B) aal_mem_dealloc(newfxlnrs.num2);
    
    char *CleanResult = aal_clrizr(ReversedResult);
    if (ReversedResult != CleanResult) {
        aal_mem_dealloc(ReversedResult);
    }
    
    return CleanResult;
}

/* AAL - Addition with Decimal Point Support */
char *aal_add(char *A, char *B)
{
    if (A == NULL || B == NULL) {
        return NULL;
    }
    
    // Check for decimal points in both numbers
    uintptr_t dotPosA = aal_dotchk(A);
    uintptr_t dotPosB = aal_dotchk(B);
    
    // Determine if we're dealing with decimals
    bool hasDecimalA = (dotPosA != (uintptr_t)-1);
    bool hasDecimalB = (dotPosB != (uintptr_t)-1);
    
    // If neither has decimals, use the original integer logic
    if (!hasDecimalA && !hasDecimalB) {
        return aal_add_integers(A, B);
    }
    
    // Handle decimal addition
    uintptr_t lenA = aal_len(A);
    uintptr_t lenB = aal_len(B);
    
    // Calculate decimal places for each number
    uintptr_t decimalPlacesA = hasDecimalA ? (lenA - dotPosA - 1) : 0;
    uintptr_t decimalPlacesB = hasDecimalB ? (lenB - dotPosB - 1) : 0;
    uintptr_t maxDecimalPlaces = (decimalPlacesA > decimalPlacesB) ? decimalPlacesA : decimalPlacesB;
    
    // Create normalized versions (remove decimal points and pad with zeros)
    char *normalizedA = normalize_for_decimal_arithmetic(A, maxDecimalPlaces, hasDecimalA);
    char *normalizedB = normalize_for_decimal_arithmetic(B, maxDecimalPlaces, hasDecimalB);
    
    if (normalizedA == NULL || normalizedB == NULL) {
        if (normalizedA) aal_mem_dealloc(normalizedA);
        if (normalizedB) aal_mem_dealloc(normalizedB);
        return NULL;
    }
    
    // Perform integer addition on normalized numbers
    char *integerResult = aal_add_integers(normalizedA, normalizedB);
    
    // Clean up normalized strings
    aal_mem_dealloc(normalizedA);
    aal_mem_dealloc(normalizedB);
    
    if (integerResult == NULL) {
        return NULL;
    }
    
    // Insert decimal point back into result
    char *finalResult = insert_decimal_point(integerResult, maxDecimalPlaces);
    aal_mem_dealloc(integerResult);
    
    return finalResult;
}

/* AAL - Subtraction - Updated with bool logic */
char *aal_sub(char *A, char *B)
{
    if (A == NULL || B == NULL) {
        return NULL;
    }
    
    // Determine which number is bigger to handle sign
    aal_cmp_result cmpResult = aal_cmp(A, B);
    bool isNegative = false;
    
    char *minuend = A;
    char *subtrahend = B;
    
    // If B > A, swap them and remember result should be negative
    if (cmpResult == CMP_B_GREATER) {
        minuend = B;
        subtrahend = A;
        isNegative = true;
    }
    
    // Get fixed-length versions of both numbers
    fixlen newfxlnrs = aal_fixlen(minuend, subtrahend);
    if (newfxlnrs.num1 == NULL || newfxlnrs.num2 == NULL) {
        return NULL;
    }
    
    uintptr_t len = newfxlnrs.finalLen;
    
    // Allocate memory for result
    char *Result = aal_mem_alloc_num(len + 1);
    if (Result == NULL) {
        return NULL;
    }
    
    int borrow = 0;
    uintptr_t resultPos = 0;
    
    // Process digits from right to left
    for (uintptr_t i = 0; i < len; i++) {
        char digitA = newfxlnrs.num1[len - 1 - i];
        char digitB = newfxlnrs.num2[len - 1 - i];
        
        int numA = (digitA - '0') - borrow;
        int numB = (digitB - '0');
        
        int difference;
        if (numA >= numB) {
            difference = numA - numB;
            borrow = 0;
        } else {
            difference = (10 + numA) - numB;
            borrow = 1;
        }
        
        Result[resultPos] = (char)(difference + '0');
        resultPos++;
    }
    
    Result[resultPos] = '\0';
    
    // Reverse and clean up
    char *ReversedResult = aal_rvrs(Result);
    aal_mem_dealloc(Result);
    
    char *CleanResult = aal_clrizr(ReversedResult);
    if (ReversedResult != CleanResult) {
        aal_mem_dealloc(ReversedResult);
    }
    
    // Add negative sign if needed
    char *FinalResult = CleanResult;
    if (isNegative && !aal_zrchk(CleanResult)) {  // Don't make -0
        FinalResult = aal_setmin(CleanResult);
        aal_mem_dealloc(CleanResult);
    }
    
    // Clean up memory allocated by aal_fixlen
    if (newfxlnrs.num1 != minuend) aal_mem_dealloc(newfxlnrs.num1);
    if (newfxlnrs.num2 != subtrahend) aal_mem_dealloc(newfxlnrs.num2);
    
    return FinalResult;
}
