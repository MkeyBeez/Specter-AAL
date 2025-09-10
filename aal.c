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
char aal_cmp(char *A, char *B)
{
    char Bigger = '0';  // C99: initialize at declaration
    
    A = aal_clrizr(A);
    B = aal_clrizr(B);
    
    char ZeroA = aal_zrchk(A);  // C99: declare at point of first use
    char ZeroB = aal_zrchk(B);
    
    if (ZeroA == '1' && ZeroB == '1') {
        return Bigger;
    }
    
    uintptr_t LenA = aal_len(A);  // C99: declare at point of first use
    uintptr_t LenB = aal_len(B);
    
    char MinA = aal_minchk(A);  // C99: declare at point of first use
    char MinB = aal_minchk(B);
    
    if (LenA > LenB) {
        if (MinA == '1') {
            Bigger = '2';
        } else {
            Bigger = '1';
        }
    } else if (LenA < LenB) {
        if (MinB == '1') {
            Bigger = '1';
        } else {
            Bigger = '2';
        }
    } else {
        char *P = A;  // C99: declare at point of first use
        
        while (*P != '\0') {
            if (A[P - A] > B[P - A]) {
                if (MinA == '1') {
                    Bigger = '2';
                } else {
                    Bigger = '1';
                }
                break;
            } else if (A[P - A] < B[P - A]) {
                if (MinB == '1') {
                    Bigger = '1';
                } else {
                    Bigger = '2';
                }
                break;
            }
            
            P++;  // Fixed: removed incorrect dereference operator
        }
    }
    
    return Bigger;
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
    uintptr_t LenX = aal_len(X);  // C99: declare at point of first use
    uintptr_t LenS = aal_len(S);
    
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

/* AAL - Zero Check */

/* AAL - Zero Check */
char aal_zrchk(char *X)
{
    if (X == NULL) {
        return '0';
    }
    
    char *CleanX = aal_clrmin(X);  // C99: declare at point of first use
    if (CleanX == NULL) {
        return '0';  // Handle allocation failure
    }
    
    uintptr_t LenX = aal_len(CleanX);
    char Zero = '0';  // C99: initialize at declaration
    uintptr_t Tracer = 0;  // C99: initialize at declaration
    
    char *P = CleanX;  // C99: declare at point of first use
    while (*P != '\0') {
        if (CleanX[P - CleanX] == '0') {
            Tracer++;
        }
        P++;  // Fixed: removed incorrect dereference operator
    }
    
    aal_mem_dealloc(CleanX);  // Clean up allocated memory
    
    if (Tracer == LenX) {
        Zero = '1';
    }
    
    return Zero;
}

/* AAL - Clear Initial Zeroes */
char *aal_clrizr(char *X)
{
    if (X == NULL) {
        return NULL;
    }
    
    char *ClrStr = aal_mem_alloc_1(X);
    if (ClrStr == NULL) {
        return NULL;  // Handle allocation failure
    }
    
    char Flag = '0';  // C99: initialize at declaration
    char *P = X;      // C99: declare at point of first use
    
    while (*P != '\0') {
        if (X[P - X] != '0' && (isdigit(X[P - X]) || X[P - X] == '-' || X[P - X] == '.')) {
            aal_mem_dealloc(ClrStr);  // Free the initial allocation
            ClrStr = aal_copy(X, (uintptr_t)(P - X));
            Flag = '1';
            break;
        }
        P++;  // Fixed: removed incorrect dereference operator
    }
    
    if (Flag == '1' && aal_dotchk(ClrStr) == 0) {
        char *Temp = aal_pad(ClrStr, "0");
        aal_mem_dealloc(ClrStr);
        ClrStr = Temp;
    }
    
    if (Flag == '0') {
        aal_mem_dealloc(ClrStr);
        ClrStr = aal_mem_alloc_num(2);
        if (ClrStr != NULL) {
            ClrStr[0] = '0';
            ClrStr[1] = '\0';
        }
    }
    
    return ClrStr;
}

/* AAL - Minus Check (Minus Sign) */
char aal_minchk(char *X)
{
    if (X == NULL || X[0] == '\0') {
        return '0';
    }
    
    char Minus = '0';  // C99: initialize at declaration
    
    if (X[0] == '-') {
        Minus = '1';
    }
    
    return Minus;
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

/* AAL - Dot Check (Comma Sign) */
uintptr_t aal_dotchk(char *X)
{
    if (X == NULL) {
        return (uintptr_t)-1;
    }
    
    char *P = X;    // C99: declare at point of first use
    char Flag = 0;  // C99: initialize at declaration
    
    while (*P != '\0') {
        if (X[P - X] == '.') {
            Flag = '1';
            break;
        }
        P++;  // Fixed: removed incorrect dereference operator
    }
    
    if (Flag == '1') {
        return (uintptr_t)(P - X);
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

/* AAL - Fixate Numbers Length */
fixlen aal_fixlen(char *A, char *B)
{
    fixlen locfxlnrs = {0};  // C99: initialize struct to zero
    
    if (A == NULL || B == NULL) {
        return locfxlnrs;  // Return zero-initialized struct for error case
    }
    
    uintptr_t LenA = aal_len(A);
    uintptr_t LenB = aal_len(B);
    
    if (LenA > LenB) {
        uintptr_t DifLen = LenA - LenB;  // C99: declare at point of first use
        char *Tmp = aal_mem_alloc_num(DifLen + 1);  // +1 for null terminator
        if (Tmp == NULL) {
            return locfxlnrs;  // Handle allocation failure
        }
        
        // Fill padding string with zeros
        for (uintptr_t i = 0; i < DifLen; i++) {  // C99: cleaner loop
            Tmp[i] = '0';
        }
        Tmp[DifLen] = '\0';
        
        locfxlnrs.Bigger = '1';
        locfxlnrs.Num1 = A;
        locfxlnrs.Num2 = aal_pad(B, Tmp);
        locfxlnrs.FinLen = LenA;
        
        aal_mem_dealloc(Tmp);
    } else if (LenA < LenB) {
        uintptr_t DifLen = LenB - LenA;  // C99: declare at point of first use
        char *Tmp = aal_mem_alloc_num(DifLen + 1);  // +1 for null terminator
        if (Tmp == NULL) {
            return locfxlnrs;  // Handle allocation failure
        }
        
        // Fill padding string with zeros
        for (uintptr_t i = 0; i < DifLen; i++) {  // C99: cleaner loop
            Tmp[i] = '0';
        }
        Tmp[DifLen] = '\0';
        
        locfxlnrs.Bigger = '2';
        locfxlnrs.Num1 = aal_pad(A, Tmp);
        locfxlnrs.Num2 = B;
        locfxlnrs.FinLen = LenB;
        
        aal_mem_dealloc(Tmp);
    } else {
        locfxlnrs.Bigger = '0';
        locfxlnrs.Num1 = A;
        locfxlnrs.Num2 = B;
        locfxlnrs.FinLen = LenA;
    }
    
    return locfxlnrs;
}

/* AAL - Read File */
rdflout aal_rdfl(const char *Z)
{
    rdflout locrdflout = {0};  // C99: initialize struct to zero
    
    if (Z == NULL) {
        locrdflout.Num1 = "ERROR";
        locrdflout.Num2 = "ERROR";
        return locrdflout;
    }
    
    FILE *FP = fopen(Z, "r");  // C99: declare at point of first use
    if (FP == NULL) {
        locrdflout.Num1 = "ERROR";
        locrdflout.Num2 = "ERROR";
        return locrdflout;
    }
    
    // Get file size
    if (fseek(FP, 0, SEEK_END) != 0) {
        locrdflout.Num1 = "ERROR";
        locrdflout.Num2 = "ERROR";
        fclose(FP);
        return locrdflout;
    }
    
    long flsz = ftell(FP);  // Use long for ftell return type
    if (flsz < 0) {
        locrdflout.Num1 = "ERROR";
        locrdflout.Num2 = "ERROR";
        fclose(FP);
        return locrdflout;
    }
    
    rewind(FP);
    
    if ((unsigned long)flsz < 3) {
        locrdflout.Num1 = "ERROR";
        locrdflout.Num2 = "ERROR";
        fclose(FP);
        return locrdflout;
    }
    
    char *FlStrm = aal_mem_alloc_num((unsigned long)flsz + 1);  // +1 for null terminator
    if (FlStrm == NULL) {
        locrdflout.Num1 = "ERROR";
        locrdflout.Num2 = "ERROR";
        fclose(FP);
        return locrdflout;
    }
    
    size_t file_res = fread(FlStrm, 1, (unsigned long)flsz, FP);
    FlStrm[file_res] = '\0';  // Null terminate the string
    
    if (file_res == 0) {
        locrdflout.Num1 = "ERROR";
        locrdflout.Num2 = "ERROR";
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
        locrdflout.Num1 = "ERROR";
        locrdflout.Num2 = "ERROR";
    } else {
        locrdflout.Num1 = aal_copy(FlStrm, 0);
        locrdflout.Num2 = aal_copy(FlStrm, i + 1);
    }
    
    aal_mem_dealloc(FlStrm);
    fclose(FP);
    
    return locrdflout;
}

/* AAL - Error Check (1 Argument) */
char aal_errchk_1(char *X)
{
    if (X == NULL) {
        return '1';  // Error for NULL input
    }
    
    uintptr_t LenX = aal_len(X);
    if (LenX == 0) {
        return '1';  // Error for empty string
    }
    
    bool Dot = false;     // C99: use bool instead of char for clarity
    char Err = '0';       // C99: initialize at declaration
    char *P = X;          // C99: declare at point of first use
    
    while (*P != '\0') {
        int Check = isdigit(X[P - X]);  // Use int for isdigit return
        
        if (Check == 0) {
            // Character is not a digit, check if it's valid
            uintptr_t position = (uintptr_t)(P - X);
            
            if ((position == 0 && X[P - X] != '-') ||                    // First char must be digit or minus
                (position > 0 && X[P - X] != '.') ||                     // Non-first chars must be digit or dot
                (position == LenX - 1 && X[P - X] == '.') ||            // Last char cannot be dot
                (X[P - X] == '.' && Dot)) {                             // Only one dot allowed
                
                Err = '1';
                break;
            }
            
            if (X[P - X] == '.') {
                Dot = true;
            }
        }
        
        P++;  // Fixed: removed incorrect dereference operator
    }
    
    return Err;
}

/* AAL - Error Check (2 Arguments) */
char aal_errchk_2(char *A, char *B)
{
    // Check first argument
    if (aal_errchk_1(A) == '1') {
        return '1';  // Error in first argument
    }
    
    // Check second argument
    if (aal_errchk_1(B) == '1') {
        return '1';  // Error in second argument
    }
    
    return '0';  // Both arguments are valid
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

/* AAL - Addition */
char *aal_add(char *A, char *B)
{
    if (A == NULL || B == NULL) {
        return NULL;
    }
    
    // Get fixed-length versions of both numbers
    fixlen newfxlnrs = aal_fixlen(A, B);
    if (newfxlnrs.Num1 == NULL || newfxlnrs.Num2 == NULL) {
        return NULL;
    }
    
    uintptr_t len = newfxlnrs.FinLen;
    
    // Allocate memory for result (may need one extra digit for carry)
    char *Result = aal_mem_alloc_num(len + 2);
    if (Result == NULL) {
        // Clean up allocated memory from aal_fixlen if needed
        return NULL;
    }
    
    int carry = 0;  // C99: use int for arithmetic
    uintptr_t resultPos = 0;
    
    // Process digits from right to left (least significant to most significant)
    for (uintptr_t i = 0; i < len; i++) {  // C99: declare loop variable in for statement
        // Get digits from right to left
        char digitA = newfxlnrs.Num1[len - 1 - i];
        char digitB = newfxlnrs.Num2[len - 1 - i];
        
        // Convert chars to integers and add with carry
        int sum = (digitA - '0') + (digitB - '0') + carry;
        
        // Calculate new carry and digit
        carry = sum / 10;
        int digit = sum % 10;
        
        // Store digit in result (we'll reverse later)
        Result[resultPos] = (char)(digit + '0');
        resultPos++;
    }
    
    // Handle final carry if present
    if (carry > 0) {
        Result[resultPos] = (char)(carry + '0');
        resultPos++;
    }
    
    // Null terminate
    Result[resultPos] = '\0';
    
    // Reverse the result since we built it backwards
    char *ReversedResult = aal_rvrs(Result);
    aal_mem_dealloc(Result);
    
    // Clean up memory allocated by aal_fixlen (if they're different from originals)
    if (newfxlnrs.Num1 != A) {
        aal_mem_dealloc(newfxlnrs.Num1);
    }
    if (newfxlnrs.Num2 != B) {
        aal_mem_dealloc(newfxlnrs.Num2);
    }
    
    // Clean up leading zeros and return
    char *CleanResult = aal_clrizr(ReversedResult);
    if (ReversedResult != CleanResult) {
        aal_mem_dealloc(ReversedResult);
    }
    
    return CleanResult;
}

/* AAL - Subtraction */
char *aal_sub(char *A, char *B)
{
    if (A == NULL || B == NULL) {
        return NULL;
    }
    
    // Determine which number is bigger to handle sign
    char Bigger = aal_cmp(A, B);
    bool isNegative = false;
    
    char *minuend = A;    // Number to subtract from
    char *subtrahend = B; // Number to subtract
    
    // If B > A, swap them and remember result should be negative
    if (Bigger == '2') {
        minuend = B;
        subtrahend = A;
        isNegative = true;
    }
    
    // Get fixed-length versions of both numbers
    fixlen newfxlnrs = aal_fixlen(minuend, subtrahend);
    if (newfxlnrs.Num1 == NULL || newfxlnrs.Num2 == NULL) {
        return NULL;
    }
    
    uintptr_t len = newfxlnrs.FinLen;
    
    // Allocate memory for result
    char *Result = aal_mem_alloc_num(len + 1);
    if (Result == NULL) {
        return NULL;
    }
    
    int borrow = 0;  // C99: use int for arithmetic
    uintptr_t resultPos = 0;
    
    // Process digits from right to left (least significant to most significant)
    for (uintptr_t i = 0; i < len; i++) {  // C99: declare loop variable in for statement
        // Get digits from right to left
        char digitA = newfxlnrs.Num1[len - 1 - i];
        char digitB = newfxlnrs.Num2[len - 1 - i];
        
        // Convert chars to integers
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
        
        // Store digit in result (we'll reverse later if needed)
        Result[resultPos] = (char)(difference + '0');
        resultPos++;
    }
    
    // Null terminate
    Result[resultPos] = '\0';
    
    // Reverse the result since we built it backwards
    char *ReversedResult = aal_rvrs(Result);
    aal_mem_dealloc(Result);
    
    // Clean up leading zeros
    char *CleanResult = aal_clrizr(ReversedResult);
    if (ReversedResult != CleanResult) {
        aal_mem_dealloc(ReversedResult);
    }
    
    // Add negative sign if needed
    char *FinalResult = CleanResult;
    if (isNegative && aal_zrchk(CleanResult) != '1') {  // Don't make -0
        FinalResult = aal_setmin(CleanResult);
        aal_mem_dealloc(CleanResult);
    }
    
    // Clean up memory allocated by aal_fixlen (if they're different from originals)
    if (newfxlnrs.Num1 != minuend) {
        aal_mem_dealloc(newfxlnrs.Num1);
    }
    if (newfxlnrs.Num2 != subtrahend) {
        aal_mem_dealloc(newfxlnrs.Num2);
    }
    
    return FinalResult;
}
