// Refactor of https://github.com/g0d/Specter-AAL/blob/main/aal.c by Martin Tannhaus 2025

// Begin complete rewrite with sensible structs.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct {
    char *digits;   // only digits, no decimal point
    int scale;      // number of fractional digits
    int sign;       // +1 or -1
} BigFloat;

// ---------- Helpers ----------

// Strip leading zeros
char* stripLeadingZeros(char* str) {
    while (*str == '0' && *(str + 1) != '\0') str++;
    return str;
}

// Reverse in place
void reverse(char* str) {
    int n = strlen(str);
    for (int i = 0; i < n/2; i++) {
        char tmp = str[i];
        str[i] = str[n-i-1];
        str[n-i-1] = tmp;
    }
}

// Parse string into BigFloat
BigFloat parseBigFloat(const char* s) {
    BigFloat bf;
    bf.sign = 1;
    bf.scale = 0;

    // normalize sign: handle multiple +/-
    while (*s == '+' || *s == '-') {
        if (*s == '-') bf.sign = -bf.sign;
        s++;
    }

    // must start with digit or dot now
    if (!isdigit(*s) && *s != '.') {
        fprintf(stderr, "Invalid number format: %s\n", s);
        bf.digits = strdup("0");
        bf.sign = 1;
        bf.scale = 0;
        return bf;
    }

    const char *dot = strchr(s, '.');
    if (dot) {
        bf.scale = strlen(dot+1);
        bf.digits = malloc(strlen(s)+1);
        int k=0;
        for (int i=0; s[i]; i++) {
            if (s[i] != '.') bf.digits[k++] = s[i];
        }
        bf.digits[k] = '\0';
    } else {
        bf.digits = strdup(s);
    }

    // strip leading zeros
    bf.digits = stripLeadingZeros(bf.digits);
    if (*bf.digits == '\0') { 
        bf.digits = strdup("0"); 
        bf.scale = 0; 
        bf.sign = 1; 
    }

    return bf;
}

// Format BigFloat as string
char* formatBigFloat(BigFloat bf) {
    int len = strlen(bf.digits);
    int pointPos = len - bf.scale;

    // Ensure pointPos >= 1
    if (pointPos <= 0) {
        // need leading zeros like 0.00123
        char* res = calloc(2 + bf.scale + len + 2, 1);
        int k = 0;
        if (bf.sign < 0) res[k++] = '-';
        res[k++] = '0';
        res[k++] = '.';
        for (int i=0; i < -pointPos; i++) res[k++] = '0';
        strcpy(res+k, bf.digits);
        return res;
    }

    // normal case
    char* res = calloc(len+3, 1);
    int k=0;
    if (bf.sign < 0) res[k++] = '-';
    for (int i=0; i<len; i++) {
        if (i == pointPos) res[k++]='.';
        res[k++] = bf.digits[i];
    }
    res[k] = '\0';

    // strip trailing zeros after decimal
    if (strchr(res, '.')) {
        char *end = res + strlen(res)-1;
        while (*end=='0') *end--='\0';
        if (*end=='.') *end='\0';
    }

    return res;
}

// Pads a number with leading zeros to a given length
static char* padLeft(const char* s, int len) {
    int ls = strlen(s);
    char* res = calloc(len+1, 1);
    memset(res, '0', len-ls);
    strcpy(res+(len-ls), s);
    return res;
}

// Compare two positive integers in string form
int compareDigits(const char* a, const char* b) {
    int la = strlen(a), lb = strlen(b);
    if (la != lb) return (la > lb) ? 1 : -1;
    return strcmp(a,b);
}

// Integer addition: |a| + |b|
char* addDigits(const char* a, const char* b) {
    int la = strlen(a), lb = strlen(b);
    int max = (la > lb ? la : lb) + 1;
    char* res = calloc(max+1, 1);

    int carry = 0, i = la-1, j = lb-1, k = max-1;
    res[max] = '\0';
    while (i >= 0 || j >= 0 || carry) {
        int da = (i>=0) ? a[i--]-'0' : 0;
        int db = (j>=0) ? b[j--]-'0' : 0;
        int sum = da + db + carry;
        res[k--] = (sum % 10) + '0';
        carry = sum/10;
    }

    char* trimmed = stripLeadingZeros(res+k+1);
    return strdup(trimmed);
}

// Integer subtraction: |a| - |b|, assumes |a| >= |b|
char* subDigits(const char* a, const char* b) {
    int la = strlen(a), lb = strlen(b);
    char* res = calloc(la+1, 1);

    int borrow = 0, i = la-1, j = lb-1, k = la-1;
    while (i >= 0) {
        int da = a[i]-'0' - borrow;
        int db = (j>=0) ? b[j]-'0' : 0;
        if (da < db) { da += 10; borrow=1; }
        else borrow=0;
        res[k--] = (da - db) + '0';
        i--; j--;
    }

    char* trimmed = stripLeadingZeros(res);
    return strdup(trimmed);
}

// BigFloat addition
BigFloat addBigFloat(BigFloat a, BigFloat b) {
    BigFloat res;

    // align scales
    if (a.scale > b.scale) {
        int diff = a.scale - b.scale;
        char* tmp = calloc(strlen(b.digits)+diff+1,1);
        strcpy(tmp, b.digits);
        for (int i=0;i<diff;i++) strcat(tmp,"0");
        b.digits = tmp;
        b.scale = a.scale;
    } else if (b.scale > a.scale) {
        int diff = b.scale - a.scale;
        char* tmp = calloc(strlen(a.digits)+diff+1,1);
        strcpy(tmp, a.digits);
        for (int i=0;i<diff;i++) strcat(tmp,"0");
        a.digits = tmp;
        a.scale = b.scale;
    }

    res.scale = a.scale;

    if (a.sign == b.sign) {
        res.digits = addDigits(a.digits, b.digits);
        res.sign = a.sign;
    } else {
        int cmp = compareDigits(a.digits, b.digits);
        if (cmp == 0) {
            res.digits = strdup("0");
            res.sign = 1;
            res.scale = 0;
        } else if (cmp > 0) {
            res.digits = subDigits(a.digits, b.digits);
            res.sign = a.sign;
        } else {
            res.digits = subDigits(b.digits, a.digits);
            res.sign = b.sign;
        }
    }

    return res;
}

// BigFloat subtraction: a - b
BigFloat subBigFloat(BigFloat a, BigFloat b) {
    BigFloat negB = b;
    negB.sign = -negB.sign;   // flip the sign of b
    return addBigFloat(a, negB);
}

// Base case used for numbers with less than karatsuba cutoff number of digits
char* mulBase(const char* a, const char* b) {
    int la = strlen(a), lb = strlen(b);
    int len = la + lb;
    int* tmp = calloc(len, sizeof(int));

    // multiply each digit
    for (int i = la-1; i >= 0; i--) {
        for (int j = lb-1; j >= 0; j--) {
            int prod = (a[i]-'0') * (b[j]-'0');
            int p1 = i+j, p2 = i+j+1;
            int sum = prod + tmp[p2];
            tmp[p2] = sum % 10;
            tmp[p1] += sum / 10;
        }
    }

    // convert to string
    char* res = calloc(len+1, 1);
    int k = 0;
    while (k < len && tmp[k] == 0) k++;  // skip leading zeros
    if (k == len) {  // all zero
        strcpy(res, "0");
    } else {
        for (int i = k; i < len; i++) res[i-k] = tmp[i] + '0';
    }

    free(tmp);
    return res;
}

// Karatsuba multiplication, recursive
char* mulKaratsuba(const char* x, const char* y) {
    int n = strlen(x);
    int m = strlen(y);

    // tune this cutoff length to determine when we use karatsuba
    if (n <= 32 || m <= 32) {
        return mulBase(x, y);  // your schoolbook O(n²)
    }

    // ensure equal length by padding
    int len = (n > m ? n : m);
    if (len % 2 != 0) len++;
    char* X = padLeft(x, len);
    char* Y = padLeft(y, len);

    int half = len/2;
    char* X1 = strndup(X, half);
    char* X0 = strdup(X+half);
    char* Y1 = strndup(Y, half);
    char* Y0 = strdup(Y+half);

    // recursive calls
    char* Z2 = mulKaratsuba(X1, Y1);
    char* Z0 = mulKaratsuba(X0, Y0);

    char* X1pX0 = addDigits(X1, X0);
    char* Y1pY0 = addDigits(Y1, Y0);
    char* P = mulKaratsuba(X1pX0, Y1pY0);

    // Z1 = P - Z2 - Z0
    char* tmp = subDigits(P, Z2);
    char* Z1 = subDigits(tmp, Z0);
    free(tmp);

    // assemble result = Z2*10^(2*half) + Z1*10^half + Z0
    int shift1 = 2*half;
    int shift2 = half;

    char* Z2s = calloc(strlen(Z2)+shift1+1,1);
    strcpy(Z2s, Z2);
    for (int i=0; i<shift1; i++) strcat(Z2s, "0");

    char* Z1s = calloc(strlen(Z1)+shift2+1,1);
    strcpy(Z1s, Z1);
    for (int i=0; i<shift2; i++) strcat(Z1s, "0");

    char* sum1 = addDigits(Z2s, Z1s);
    char* res  = addDigits(sum1, Z0);

    // cleanup
    free(X); free(Y);
    free(X1); free(X0); free(Y1); free(Y0);
    free(Z2); free(Z0); free(X1pX0); free(Y1pY0); free(P); free(Z1);
    free(Z2s); free(Z1s); free(sum1);

    return res;
}

// integration ? 
char* mulDigits(const char* a, const char* b) {
    return mulKaratsuba(a, b);  // dispatch to Karatsuba
}

// Integer division: returns quotient string (ignores remainder)
// Computes floor(a / b), where a and b are non-negative digit strings
// We have to specify the desired level of precision else the calculation will run on forever for repeating fractions etc.
char* divDigits(const char* a, const char* b, int precision) {
    if (strcmp(b, "0") == 0) {
        fprintf(stderr, "Division by zero!\n");
        return strdup("0");
    }

    int la = strlen(a);
    char* quotient = calloc(la + precision + 2, 1); 
    char* remainder = strdup("0");

    // Work buffer for dividend digits
    char* cur = calloc(la + precision + 2, 1);
    strcpy(cur, a);

    // Append extra zeros for fractional precision
    for (int i=0; i<precision; i++) strcat(cur, "0");

    int pos = 0;
    char* prefix = calloc(2,1); prefix[0]='\0';

    for (int i=0; cur[i]; i++) {
        // bring down next digit
        int plen = strlen(prefix);
        char* tmp = calloc(plen+2,1);
        strcpy(tmp,prefix);
        tmp[plen]=cur[i]; tmp[plen+1]='\0';

        free(prefix);
        prefix = stripLeadingZeros(tmp);

        // find quotient digit by trial
        int q=0;
        while (compareDigits(prefix,b) >= 0) {
            char* t = subDigits(prefix,b);
            free(prefix);
            prefix = t;
            q++;
        }
        quotient[pos++] = q+'0';
    }

    quotient[pos] = '\0';
    free(prefix);
    free(cur);
    free(remainder);

    // strip leading zeros
    char* trimmed = stripLeadingZeros(quotient);
    return strdup(trimmed);
}

// Integer modulo: returns remainder string
char* modDigits(const char* a, const char* b) {
    if (strcmp(b, "0") == 0) {
        fprintf(stderr, "Modulo by zero!\n");
        return strdup("0");
    }

    char* prefix = calloc(2,1); prefix[0] = '\0';

    for (int i=0; a[i]; i++) {
        // bring down next digit
        int plen = strlen(prefix);
        char* tmp = calloc(plen+2,1);
        strcpy(tmp,prefix);
        tmp[plen] = a[i]; tmp[plen+1] = '\0';
        free(prefix);
        prefix = stripLeadingZeros(tmp);

        // subtract divisor until remainder < b
        while (compareDigits(prefix, b) >= 0) {
            char* t = subDigits(prefix, b);
            free(prefix);
            prefix = t;
        }
    }

    // if remainder empty, it's zero
    if (*prefix == '\0') {
        free(prefix);
        return strdup("0");
    }

    return prefix; // remainder
}

// BigFloat multiplication
BigFloat mulBigFloat(BigFloat a, BigFloat b) {
    BigFloat res;
    res.sign = a.sign * b.sign;
    res.scale = a.scale + b.scale;
    res.digits = mulDigits(a.digits, b.digits);

    // special case: result is "0"
    if (strcmp(res.digits, "0") == 0) {
        res.sign = 1;
        res.scale = 0;
    }

    return res;
}

BigFloat divBigFloat(BigFloat a, BigFloat b, int precision) {
    BigFloat res;

    if (strcmp(b.digits, "0") == 0) {
        fprintf(stderr, "Division by zero!\n");
        res.digits = strdup("0");
        res.sign = 1;
        res.scale = 0;
        return res;
    }

    // Result sign
    res.sign = a.sign * b.sign;

    // Effective dividend and divisor are integers
    // Align by shifting decimals: multiply dividend by 10^precision
    int shift = precision + b.scale - a.scale;

    char* dividend = strdup(a.digits);
    for (int i=0; i<shift; i++) {
        char* tmp = calloc(strlen(dividend)+2,1);
        strcpy(tmp, dividend);
        strcat(tmp, "0");
        free(dividend);
        dividend = tmp;
    }

    char* divisor = strdup(b.digits);

    // perform integer division
    char* q = divDigits(dividend, divisor, 0);

    res.digits = q;
    res.scale = precision;

    // normalize if result is zero
    if (strcmp(res.digits, "0") == 0) {
        res.sign = 1;
        res.scale = 0;
    }

    free(dividend);
    free(divisor);

    return res;
}

BigFloat modBigFloat(BigFloat a, BigFloat b) {
    BigFloat res;

    if (strcmp(b.digits, "0") == 0) {
        fprintf(stderr, "Modulo by zero!\n");
        res.digits = strdup("0");
        res.sign = 1;
        res.scale = 0;
        return res;
    }

    // Align by making both integers
    int maxScale = (a.scale > b.scale ? a.scale : b.scale);

    // scale up a
    char* da = strdup(a.digits);
    for (int i=0; i < maxScale - a.scale; i++) {
        char* tmp = calloc(strlen(da)+2,1);
        strcpy(tmp, da);
        strcat(tmp, "0");
        free(da);
        da = tmp;
    }

    // scale up b
    char* db = strdup(b.digits);
    for (int i=0; i < maxScale - b.scale; i++) {
        char* tmp = calloc(strlen(db)+2,1);
        strcpy(tmp, db);
        strcat(tmp, "0");
        free(db);
        db = tmp;
    }

    // compute remainder
    char* rem = modDigits(da, db);

    res.digits = rem;
    res.scale = maxScale;
    res.sign = a.sign; // remainder takes dividend's sign

    // normalize zero
    if (strcmp(res.digits, "0") == 0) {
        res.sign = 1;
        res.scale = 0;
    }

    free(da);
    free(db);
    return res;
}

// ---------- Demo ----------
int main() {
    // Testing modulo
    BigFloat a = parseBigFloat("22.5");
    BigFloat b = parseBigFloat("7");
    
    BigFloat test_r = modBigFloat(a, b);
    char* test_result = formatBigFloat(test_r);
    printf("%s\n", test_result);
    return 0;
}
