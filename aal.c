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
char aal_zrchk(const char *X)
{
    char Zero;
    size_t i;
    size_t Tracer;
    size_t LenX;
    char *CleanX;

    if (X == NULL)
        return '0';
    
    CleanX = aal_clrmin(X);
    LenX = aal_len(CleanX);
    
    Zero = '0';
    Tracer = 0;
    
    for (i = 0; i < LenX; i++)
    {
        if (CleanX[i] == '0')
            Tracer++;
    }
    
    aal_mem_dealloc(CleanX);
    
    if (Tracer == LenX)
        Zero = '1';
    
    return Zero;
}

/* AAL - Clear Initial Zeroes */
char *aal_clrizr(const char *X)
{
    char Flag;
    size_t i;
    char *ClrStr;
    size_t LenX;
    
    if (X == NULL)
        return NULL;
    
    LenX = aal_len(X);
    Flag = '0';
    
    for (i = 0; i < LenX; i++)
    {
        if (X[i] != '0' && (isdigit(X[i]) || X[i] == '-' || X[i] == '.'))
        {
            ClrStr = aal_copy(X, i);
            Flag = '1';
            break;
        }
    }
    
    if (Flag == '0')
    {
        ClrStr = aal_mem_alloc_num(2);
        if (ClrStr != NULL)
        {
            ClrStr[0] = '0';
            ClrStr[1] = '\0';
        }
        return ClrStr;
    }
    
    if (aal_dotchk(ClrStr) == (size_t)-1)
    {
        char *Temp = aal_pad(ClrStr, "0");
        aal_mem_dealloc(ClrStr);
        ClrStr = Temp;
    }
    
    return ClrStr;
}

/* AAL - Minus Check (Minus Sign) */
char aal_minchk(const char *X)
{
    if (X == NULL || X[0] == '\0')
        return '0';
    
    return (X[0] == '-') ? '1' : '0';
}

/* AAL - Set Minus Sign */
char *aal_setmin(const char *X)
{
    char *FinNum;
    size_t LenX;
    size_t i;
    
    if (X == NULL)
        return NULL;
    
    LenX = aal_len(X);
    FinNum = aal_mem_alloc_num(LenX + 2);
    if (FinNum == NULL)
        return NULL;
    
    FinNum[0] = '-';
    
    for (i = 0; i < LenX; i++)
    {
        FinNum[i + 1] = X[i];
    }
    
    FinNum[LenX + 1] = '\0';
    
    return FinNum;
}

/* AAL - Clear Minus Sign */
char *aal_clrmin(const char *X)
{
    char *AbsNum;
    size_t LenX;
    size_t i;
    
    if (X == NULL)
        return NULL;
    
    if (aal_minchk(X) == '0')
    {
        AbsNum = aal_mem_alloc_num(aal_len(X) + 1);
        if (AbsNum != NULL)
            strcpy(AbsNum, X);
        return AbsNum;
    }
    
    LenX = aal_len(X);
    AbsNum = aal_mem_alloc_num(LenX);
    if (AbsNum == NULL)
        return NULL;
    
    for (i = 1; i < LenX; i++)
    {
        AbsNum[i - 1] = X[i];
    }
    
    AbsNum[LenX - 1] = '\0';
    
    return AbsNum;
}

/* AAL - Dot Check (Comma Sign) */
size_t aal_dotchk(const char *X)
{
    size_t i;
    size_t LenX;
    
    if (X == NULL)
        return (size_t)-1;
    
    LenX = aal_len(X);
    
    for (i = 0; i < LenX; i++)
    {
        if (X[i] == '.')
            return i;
    }
    
    return (size_t)-1;
}

/* AAL - Set Dot (Comma Sign) */
char *aal_setdot(const char *X, size_t Pos)
{
    char *RealNum;
    size_t LenX;
    size_t i;
    size_t j;
    
    if (X == NULL || Pos > aal_len(X))
        return NULL;
    
    LenX = aal_len(X);
    RealNum = aal_mem_alloc_num(LenX + 2);
    if (RealNum == NULL)
        return NULL;
    
    for (i = 0, j = 0; i <= LenX; i++, j++)
    {
        if (i == Pos)
        {
            RealNum[j] = '.';
            j++;
        }
        if (i < LenX)
        {
            RealNum[j] = X[i];
        }
    }
    
    RealNum[LenX + 1] = '\0';
    
    return RealNum;
}

/* AAL - Clear Dot (Comma Sign) */
char *aal_clrdot(const char *X)
{
    char *IntNum;
    size_t DotPos;
    size_t LenX;
    size_t i;
    size_t j;
    
    if (X == NULL)
        return NULL;
    
    DotPos = aal_dotchk(X);
    if (DotPos == (size_t)-1)
    {
        IntNum = aal_mem_alloc_num(aal_len(X) + 1);
        if (IntNum != NULL)
            strcpy(IntNum, X);
        return IntNum;
    }
    
    LenX = aal_len(X);
    IntNum = aal_mem_alloc_num(LenX);
    if (IntNum == NULL)
        return NULL;
    
    for (i = 0, j = 0; i < LenX; i++)
    {
        if (i != DotPos)
        {
            IntNum[j] = X[i];
            j++;
        }
    }
    
    IntNum[j] = '\0';
    
    return IntNum;
}

/* AAL - Fixate Numbers Length */
fixlen aal_fixlen(const char *A, const char *B)
{
    size_t DifLen;
    size_t LenA;
    size_t LenB;
    fixlen locfxlnrs = {'0', NULL, NULL, 0};
    char *Tmp;
    size_t i;
    
    if (A == NULL || B == NULL)
        return locfxlnrs;
    
    LenA = aal_len(A);
    LenB = aal_len(B);
    
    if (LenA > LenB)
    {
        DifLen = LenA - LenB;
        Tmp = aal_mem_alloc_num(DifLen + 1);
        if (Tmp == NULL)
            return locfxlnrs;
        
        for (i = 0; i < DifLen; i++)
        {
            Tmp[i] = '0';
        }
        Tmp[DifLen] = '\0';
        
        locfxlnrs.Bigger = '1';
        locfxlnrs.Num1 = (char *)A;
        locfxlnrs.Num2 = aal_pad(B, Tmp);
        locfxlnrs.FinLen = LenA;
        
        aal_mem_dealloc(Tmp);
    }
    else if (LenA < LenB)
    {
        DifLen = LenB - LenA;
        Tmp = aal_mem_alloc_num(DifLen + 1);
        if (Tmp == NULL)
            return locfxlnrs;
        
        for (i = 0; i < DifLen; i++)
        {
            Tmp[i] = '0';
        }
        Tmp[DifLen] = '\0';
        
        locfxlnrs.Bigger = '2';
        locfxlnrs.Num1 = aal_pad(A, Tmp);
        locfxlnrs.Num2 = (char *)B;
        locfxlnrs.FinLen = LenB;
        
        aal_mem_dealloc(Tmp);
    }
    else
    {
        locfxlnrs.Bigger = '0';
        locfxlnrs.Num1 = (char *)A;
        locfxlnrs.Num2 = (char *)B;
        locfxlnrs.FinLen = LenA;
    }
    
    return locfxlnrs;
}

/* AAL - Read File */
rdflout aal_rdfl(const char *Z)
{
    unsigned long i;
    unsigned long flsz;
    char *FlStrm;
    size_t file_res;
    rdflout locrdflout = {NULL, NULL};
    FILE *FP;
    
    if (Z == NULL)
        return locrdflout;
    
    FP = fopen(Z, "r");
    
    if (FP == NULL)
    {
        locrdflout.Num1 = "ERROR";
        locrdflout.Num2 = "ERROR";
        return locrdflout;
    }
    
    fseek(FP, 0, SEEK_END);
    flsz = ftell(FP);
    rewind(FP);
    
    if (flsz < 3)
    {
        fclose(FP);
        locrdflout.Num1 = "ERROR";
        locrdflout.Num2 = "ERROR";
        return locrdflout;
    }
    
    FlStrm = aal_mem_alloc_num(flsz + 1);
    if (FlStrm == NULL)
    {
        fclose(FP);
        return locrdflout;
    }
    
    file_res = fread(FlStrm, 1, flsz, FP);
    FlStrm[flsz] = '\0';
    
    if (file_res != flsz)
    {
        aal_mem_dealloc(FlStrm);
        fclose(FP);
        locrdflout.Num1 = "ERROR";
        locrdflout.Num2 = "ERROR";
        return locrdflout;
    }
    
    i = 0;
    while (FlStrm[i] != ':' && FlStrm[i] != '\0')
        i++;
    
    if (FlStrm[i] == ':')
    {
        locrdflout.Num1 = aal_copy(FlStrm, 0);
        locrdflout.Num2 = aal_copy(FlStrm, i + 1);
    }
    else
    {
        locrdflout.Num1 = "ERROR";
        locrdflout.Num2 = "ERROR";
    }
    
    aal_mem_dealloc(FlStrm);
    fclose(FP);
    
    return locrdflout;
}

/* AAL - Error Check (1 Argument) */
char aal_errchk_1(const char *X)
{
    short int Check;
    char Dot;
    char Err;
    size_t i;
    size_t LenX;
    
    if (X == NULL)
        return '1';
    
    LenX = aal_len(X);
    Dot = '0';
    Err = '0';
    
    for (i = 0; i < LenX; i++)
    {
        Check = isdigit((unsigned char)X[i]);
        
        if (Check == 0)
        {
            if ((i == 0 && X[i] != '-') || 
                (i > 0 && X[i] != '.') || 
                (i == LenX - 1 && X[i] == '.') || 
                (X[i] == '.' && Dot == '1'))
            {
                Err = '1';
                break;
            }
            
            if (X[i] == '.')
                Dot = '1';
        }
    }
    
    return Err;
}

/* AAL - Error Check (2 Arguments) */
char aal_errchk_2(const char *A, const char *B)
{
    char Err;
    
    if (A == NULL || B == NULL)
        return '1';
    
    Err = aal_errchk_1(A);
    
    if (Err == '0')
        Err = aal_errchk_1(B);
    
    return Err;
}

/* AAL - Memory Allocator (1 Argument) */
char *aal_mem_alloc_num(size_t V)
{
    if (V == 0)
        return NULL;
    
    return calloc(V + 1, sizeof(char));
}

/* AAL - Memory Allocator (1 Argument) */
char *aal_mem_alloc_1(const char *A)
{
    if (A == NULL)
        return NULL;
    
    return aal_mem_alloc_num(aal_len(A));
}

/* AAL - Memory Allocator (2 Arguments) */
char *aal_mem_alloc_2(const char *A, const char *B)
{
    size_t LenA;
    size_t LenB;
    
    if (A == NULL || B == NULL)
        return NULL;
    
    LenA = aal_len(A);
    LenB = aal_len(B);
    
    if (LenA >= LenB)
        return aal_mem_alloc_num(LenA + 2);
    else
        return aal_mem_alloc_num(LenB + 2);
}

/* AAL - Memory Deallocator */
int aal_mem_dealloc(char *S)
{
    if (S != NULL)
        free(S);
    
    return 1;
}

/* AAL - Addition */
char *aal_add(const char *A, const char *B)
{
    short int k;
    char TmpA;
    char TmpB;
    char Flag;
    size_t i;
    char *S;
    char *T;
    char *SS;
    char *CS;
    char *Result;
    fixlen newfxlnrs;
    
    if (A == NULL || B == NULL)
        return NULL;
    
    S = aal_mem_alloc_num(3);
    T = aal_mem_alloc_num(3);
    SS = aal_mem_alloc_2(A, B);
    CS = aal_mem_alloc_2(A, B);
    Result = aal_mem_alloc_2(A, B);
    
    if (S == NULL || T == NULL || SS == NULL || CS == NULL || Result == NULL)
    {
        aal_mem_dealloc(S);
        aal_mem_dealloc(T);
        aal_mem_dealloc(SS);
        aal_mem_dealloc(CS);
        aal_mem_dealloc(Result);
        return NULL;
    }
    
    Flag = '0';
    
    newfxlnrs = aal_fixlen(A, B);
    if (newfxlnrs.Num1 == NULL || newfxlnrs.Num2 == NULL)
    {
        aal_mem_dealloc(S);
        aal_mem_dealloc(T);
        aal_mem_dealloc(SS);
        aal_mem_dealloc(CS);
        aal_mem_dealloc(Result);
        return NULL;
    }
    
    for (i = 0; i < newfxlnrs.FinLen; i++)
    {
        TmpA = newfxlnrs.Num1[newfxlnrs.FinLen - i - 1];
        TmpB = newfxlnrs.Num2[newfxlnrs.FinLen - i - 1];
        
        if (i > 0)
        {
            k = ((TmpA - '0') + (TmpB - '0') + (CS[i - 1] - '0'));
            
            if (k > 9)
            {
                S[0] = '1';
                S[1] = (k - 10) + '0';
                S[2] = '\0';
            }
            else
            {
                S[0] = k + '0';
                S[1] = '\0';
            }
        }
        else
        {
            k = (TmpA - '0') + (TmpB - '0');
            
            if (k > 9)
            {
                S[0] = '1';
                S[1] = (k - 10) + '0';
                S[2] = '\0';
            }
            else
            {
                S[0] = k + '0';
                S[1] = '\0';
            }
        }
        
        SS[i] = (S[1] != '\0') ? S[1] : S[0];
        
        if (k > 9)
            CS[i] = '1';
        else
            CS[i] = '0';
        
        if (i == newfxlnrs.FinLen - 1)
        {
            if (Flag == '1')
            {
                T = aal_rvrs(S);
                if (T != NULL)
                {
                    strncpy(&Result[i], T, 3);
                    aal_mem_dealloc(T);
                }
            }
            else
            {
                strncpy(&Result[i], S, 3);
            }
        }
        else
        {
            Flag = '1';
            Result[i] = SS[i];
        }
    }
    
    if (Flag == '1')
    {
        char *Reversed = aal_rvrs(Result);
        if (Reversed != NULL)
        {
            strcpy(Result, Reversed);
            aal_mem_dealloc(Reversed);
        }
    }
    
    aal_mem_dealloc(S);
    aal_mem_dealloc(T);
    aal_mem_dealloc(SS);
    aal_mem_dealloc(CS);
    
    if (newfxlnrs.Bigger != '0' && newfxlnrs.Num1 != A && newfxlnrs.Num1 != B)
        aal_mem_dealloc((char *)newfxlnrs.Num1);
    if (newfxlnrs.Bigger != '0' && newfxlnrs.Num2 != A && newfxlnrs.Num2 != B)
        aal_mem_dealloc((char *)newfxlnrs.Num2);
    
    return aal_clrizr(Result);
}

/* AAL - Subtraction */
char *aal_sub(const char *A, const char *B)
{
    short int k;
    char TmpA;
    char TmpB;
    char Flag;
    char Bigger;
    size_t i;
    char *S;
    char *T;
    char *SS;
    char *CS;
    char *Buffer;
    char *Result;
    fixlen newfxlnrs;
    
    if (A == NULL || B == NULL)
        return NULL;
    
    S = aal_mem_alloc_num(3);
    T = aal_mem_alloc_num(3);
    SS = aal_mem_alloc_2(A, B);
    CS = aal_mem_alloc_2(A, B);
    Buffer = aal_mem_alloc_2(A, B);
    Result = aal_mem_alloc_2(A, B);
    
    if (S == NULL || T == NULL || SS == NULL || CS == NULL || Buffer == NULL || Result == NULL)
    {
        aal_mem_dealloc(S);
        aal_mem_dealloc(T);
        aal_mem_dealloc(SS);
        aal_mem_dealloc(CS);
        aal_mem_dealloc(Buffer);
        aal_mem_dealloc(Result);
        return NULL;
    }
    
    Flag = '0';
    Bigger = aal_cmp(A, B);
    
    if (Bigger == '2')
    {
        char *Temp = aal_copy(B, 0);
        if (Temp != NULL)
        {
            strcpy(Buffer, Temp);
            aal_mem_dealloc(Temp);
        }
        Temp = aal_copy(A, 0);
        if (Temp != NULL)
        {
            strcpy((char *)B, Temp);
            aal_mem_dealloc(Temp);
        }
        strcpy((char *)A, Buffer);
    }
    
    newfxlnrs = aal_fixlen(A, B);
    if (newfxlnrs.Num1 == NULL || newfxlnrs.Num2 == NULL)
    {
        aal_mem_dealloc(S);
        aal_mem_dealloc(T);
        aal_mem_dealloc(SS);
        aal_mem_dealloc(CS);
        aal_mem_dealloc(Buffer);
        aal_mem_dealloc(Result);
        return NULL;
    }
    
    for (i = 0; i < newfxlnrs.FinLen; i++)
    {
        TmpA = newfxlnrs.Num1[newfxlnrs.FinLen - i - 1];
        TmpB = newfxlnrs.Num2[newfxlnrs.FinLen - i - 1];
        
        if (i > 0)
        {
            if ((TmpA - '0') >= ((TmpB - '0') + (CS[i - 1] - '0')))
            {
                k = (TmpA - '0') - ((TmpB - '0') + (CS[i - 1] - '0'));
                S[0] = k + '0';
                S[1] = '\0';
            }
            else
            {
                k = (10 + (TmpA - '0')) - ((TmpB - '0') + (CS[i - 1] - '0'));
                S[0] = '1';
                S[1] = k + '0';
                S[2] = '\0';
            }
        }
        else
        {
            if ((TmpA - '0') >= (TmpB - '0'))
            {
                k = (TmpA - '0') - (TmpB - '0');
                S[0] = k + '0';
                S[1] = '\0';
            }
            else
            {
                k = (10 + (TmpA - '0')) - (TmpB - '0');
                S[0] = '1';
                S[1] = k + '0';
                S[2] = '\0';
            }
        }
        
        SS[i] = (S[1] != '\0') ? S[1] : S[0];
        
        if (S[1] == '\0')
            CS[i] = '0';
        else
            CS[i] = '1';
        
        if (i == newfxlnrs.FinLen - 1)
        {
            if (Flag == '1')
            {
                T = aal_rvrs(S);
                if (T != NULL)
                {
                    strncpy(&Result[i], T, 3);
                    aal_mem_dealloc(T);
                }
            }
            else
            {
                strncpy(&Result[i], S, 3);
            }
        }
        else
        {
            Flag = '1';
            Result[i] = SS[i];
        }
    }
    
    if (Flag == '1')
    {
        char *Reversed = aal_rvrs(Result);
        if (Reversed != NULL)
        {
            strcpy(Result, Reversed);
            aal_mem_dealloc(Reversed);
        }
    }
    
    aal_mem_dealloc(S);
    aal_mem_dealloc(T);
    aal_mem_dealloc(SS);
    aal_mem_dealloc(CS);
    aal_mem_dealloc(Buffer);
    
    if (newfxlnrs.Bigger != '0' && newfxlnrs.Num1 != A && newfxlnrs.Num1 != B)
        aal_mem_dealloc((char *)newfxlnrs.Num1);
    if (newfxlnrs.Bigger != '0' && newfxlnrs.Num2 != A && newfxlnrs.Num2 != B)
        aal_mem_dealloc((char *)newfxlnrs.Num2);
    
    return aal_clrizr(Result);
}

/******************************************************************************/
