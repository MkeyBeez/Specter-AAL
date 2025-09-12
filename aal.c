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

// ---------- Core Arithmetic ----------

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


// ---------- Demo ----------
int main() {
    BigFloat a = parseBigFloat("100");
    BigFloat b = parseBigFloat("1");

    BigFloat sum = subBigFloat(a, b);

    char* result = formatBigFloat(sum);
    printf("Result: %s\n", result);

    free(result);
    return 0;
}
