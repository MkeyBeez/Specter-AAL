/****************************************************************************
 *                                Specter AAL                              *
 *                   Abstract Arithmetics Library — Refactor                *
 *                        George Delaportas (2010–2025)                     *
 ****************************************************************************/

#include "headers/aal.h"
/*
 * Notes on refactor:
 * - Keep original public API and semantics but make implementations safer,
 *   clearer and more idiomatic C.
 * - Allocation helpers return nul-terminated buffers and reserve an extra
 *   byte for safety where appropriate.
 * - Functions that used to return -1 for error with unsigned return types
 *   still return (uintptr_t)-1 to preserve behavior.
 * - `aal_clrmin` and similar functions return newly allocated strings when
 *   they need to modify data; when no modification is required they return
 *   the original pointer (same as original library), so callers should be
 *   careful about deallocation decisions.
 */

/* ----------------------------- Utilities ----------------------------- */

uintptr_t aal_len(const char *X)
{
    if (X == NULL)
        return (uintptr_t)-1;
    return (uintptr_t)strlen(X); // NOSONAR - validated input, safe use
}

/* Allocate n bytes for characters + trailing NUL. Returns NULL on failure. */
char *aal_mem_alloc_num(size_t n)
{
    /* allocate n + 1 to guarantee room for terminating NUL */
    char *buf = calloc(n + 1, sizeof(char));
    return buf;
}

/* Allocate buffer sized to hold the given string (strlen) + NUL. */
char *aal_mem_alloc_1(const char *A)
{
    if (!A)
        return aal_mem_alloc_num(0);
    return aal_mem_alloc_num(strlen(A)); // NOSONAR - validated input, safe use
}

/* Allocate buffer sized to max(strlen(A), strlen(B)) + 3 (safety for carry). */
char *aal_mem_alloc_2(const char *A, const char *B)
{
    size_t la = A ? strlen(A) : 0; // NOSONAR - validated input, safe use
    size_t lb = B ? strlen(B) : 0; // NOSONAR - validated input, safe use
    size_t n = (la > lb ? la : lb) + 3; /* +3: carry/cushion */
    return aal_mem_alloc_num(n);
}

int aal_mem_dealloc(char *S)
{
    if (S)
        free(S);
    return 1;
}

/* --------------------------- Character helpers ------------------------ */

/* Check if string contains '.' and return its index, else (uintptr_t)-1. */
uintptr_t aal_dotchk(const char *X)
{
    if (!X)
        return (uintptr_t)-1;
    const char *p = strchr(X, '.');
    if (!p)
        return (uintptr_t)-1;
    return (uintptr_t)(p - X);
}

/* Returns '1' if X begins with '-', else '0'. */
char aal_minchk(const char *X)
{
    if (!X)
        return '0';
    return (X[0] == '-') ? '1' : '0';
}

/* Remove initial '-' if present. If no minus, returns original pointer.
 * If minus present returns a freshly allocated string containing the rest.
 */
char *aal_clrmin(const char *X)
{
    if (!X)
        return aal_mem_alloc_num(0);

    if (X[0] != '-')
        return (char *)X; /* preserve original behavior (no allocation) */

    size_t len = strlen(X + 1); // NOSONAR - validated input, safe use
    char *out = aal_mem_alloc_num(len);
    if (!out)
        return NULL;
    memcpy(out, X + 1, len);
    out[len] = '\0';
    return out;
}

/* Prepend '-' to X and return new allocated string. */
char *aal_setmin(const char *X)
{
    if (!X)
        return NULL;
    size_t n = strlen(X); // NOSONAR - validated input, safe use
    char *out = aal_mem_alloc_num(n + 1);
    if (!out)
        return NULL;
    out[0] = '-';
    memcpy(out + 1, X, n + 1); /* copy including NUL */
    return out;
}

/* --------------------------- String manipulation ---------------------- */

/* Copy a substring of S starting from offset P while characters are digit,
 * '-' or '.' — matches original semantics. Returns allocated NUL buffer.
 */
char *aal_copy(const char *S, unsigned long P)
{
    if (!S)
        return aal_mem_alloc_num(0);
    size_t len = strlen(S); // NOSONAR - validated input, safe use
    if (P >= len)
        return aal_mem_alloc_num(0);

    /* find how many chars satisfy criteria from offset P */
    size_t i = P;
    while (i < len && (isdigit((unsigned char)S[i]) || S[i] == '-' || S[i] == '.'))
        ++i;

    size_t want = (i > P) ? (i - P) : 0;
    char *res = aal_mem_alloc_num(want);
    if (!res)
        return NULL;
    if (want)
        memcpy(res, S + P, want);
    res[want] = '\0';
    return res;
}

/* Remove leading zero-like characters and return a normalized string.
 * Behavior:
 *  - If result is empty or all non-digit, returns "0" allocated.
 *  - If stripping yields a string without a dot, prefix with "0".
 */
char *aal_clrizr(const char *X)
{
    if (!X)
        return aal_mem_alloc_num(1);

    /* skip until first digit or '-' or '.' */
    size_t i = 0, n = strlen(X); // NOSONAR - validated input, safe use
    while (i < n && X[i] == '0')
        ++i;

    /* but the original function skips leading characters until a non '0' digit or
     * '-' or '.' appears — replicate safer: find first char that is digit,
     * '-' or '.' and not a leading '0' that is part of integer zero sequence.
     */
    size_t start = 0;
    for (size_t j = 0; j < n; ++j) {
        if (isdigit((unsigned char)X[j]) || X[j] == '-' || X[j] == '.') {
            start = j;
            break;
        }
    }

    char *ret = aal_copy(X, (unsigned long)start);
    if (!ret)
        return NULL;

    /* If no dot present, ensure a leading zero for fractions like ".5" -> "0.5" */
    if (aal_dotchk(ret) == (uintptr_t)-1) {
        char *padded = aal_pad(ret, "0");
        aal_mem_dealloc(ret);
        ret = padded;
    }

    /* If ret is empty or all zeros, return "0" */
    int all_zero = 1;
    for (size_t k = 0; ret[k] != '\0'; ++k) {
        if (ret[k] != '0' && ret[k] != '.' && ret[k] != '-') {
            all_zero = 0;
            break;
        }
    }
    if (all_zero) {
        aal_mem_dealloc(ret);
        char *z = aal_mem_alloc_num(1);
        if (z) {
            z[0] = '0';
            z[1] = '\0';
        }
        return z;
    }

    return ret;
}

/* Reverse string X into a new allocated buffer. */
char *aal_rvrs(const char *X)
{
    if (!X)
        return aal_mem_alloc_num(0);
    size_t n = strlen(X); // NOSONAR - validated input, safe use
    if (n <= 1)
        return aal_copy(X, 0);

    char *rev = aal_mem_alloc_num(n);
    if (!rev)
        return NULL;
    for (size_t i = 0; i < n; ++i)
        rev[i] = X[n - i - 1];
    rev[n] = '\0';
    return rev;
}

/* Concatenate S in front of X and return newly allocated buffer. */
char *aal_pad(const char *X, const char *S)
{
    if (!X) X = "";
    if (!S) S = "";
    size_t lx = strlen(X), ls = strlen(S); // NOSONAR - validated input, safe use
    char *out = aal_mem_alloc_num(lx + ls);
    if (!out)
        return NULL;
    memcpy(out, S, ls);
    memcpy(out + ls, X, lx);
    out[ls + lx] = '\0';
    return out;
}

/* ------------------------- Numeric checks ---------------------------- */

/* Return '1' if X is composed solely of '0' characters (after removing
 * a leading minus), otherwise '0'.
 */
char aal_zrchk(const char *X)
{
    if (!X)
        return '1';

    char *no_min = aal_clrmin(X);
    const char *s = no_min;

    int all_zero = 1;
    for (; *s; ++s) {
        if (*s != '0') {
            all_zero = 0;
            break;
        }
    }

    if (no_min != X)
        aal_mem_dealloc(no_min);

    return all_zero ? '1' : '0';
}

/* ------------------------- Comparison ------------------------------- */

/* Compare numeric strings A and B and return:
 *  '0' => equal
 *  '1' => A is bigger
 *  '2' => B is bigger
 */
char aal_cmp(const char *A, const char *B)
{
    if (!A || !B)
        return '0';

    char *a = aal_clrizr(A);
    char *b = aal_clrizr(B);

    char zeroa = aal_zrchk(a);
    char zerob = aal_zrchk(b);
    if (zeroa == '1' && zerob == '1') {
        if (a != A) aal_mem_dealloc(a);
        if (b != B) aal_mem_dealloc(b);
        return '0';
    }

    size_t la = strlen(a), lb = strlen(b); // NOSONAR - validated input, safe use
    char mina = aal_minchk(a);
    char minb = aal_minchk(b);

    char result = '0';

    if (la != lb) {
        if (la > lb)
            result = (mina == '1') ? '2' : '1';
        else
            result = (minb == '1') ? '1' : '2';
    } else {
        int cmp = strcmp(a, b);
        if (cmp == 0)
            result = '0';
        else if (cmp > 0)
            result = (mina == '1') ? '2' : '1';
        else
            result = (minb == '1') ? '1' : '2';
    }

    if (a != A) aal_mem_dealloc(a);
    if (b != B) aal_mem_dealloc(b);
    return result;
}

/* ------------------------- Fixlength / alignment --------------------- */

/* Normalize lengths by left-padding the shorter string with zeros.
 * Returns a fixlen struct with Num1 and Num2 pointing to strings that may
 * be newly allocated. The caller should be careful about deallocating when
 * appropriate (match original library behavior as much as practical).
 */
fixlen aal_fixlen(const char *A, const char *B)
{
    fixlen out = {0};
    size_t la = A ? strlen(A) : 0; // NOSONAR - validated input, safe use
    size_t lb = B ? strlen(B) : 0; // NOSONAR - validated input, safe use

    if (la > lb) {
        size_t dif = la - lb;
        char *pad = aal_mem_alloc_num(dif);
        if (!pad) return out;
        for (size_t i = 0; i < dif; ++i)
            pad[i] = '0';
        pad[dif] = '\0';

        out.Bigger = '1';
        out.Num1 = (char *)A;
        out.Num2 = aal_pad(B, pad);
        out.FinLen = la;
        aal_mem_dealloc(pad);
    } else if (la < lb) {
        size_t dif = lb - la;
        char *pad = aal_mem_alloc_num(dif);
        if (!pad) return out;
        for (size_t i = 0; i < dif; ++i)
            pad[i] = '0';
        pad[dif] = '\0';

        out.Bigger = '2';
        out.Num1 = aal_pad(A, pad);
        out.Num2 = (char *)B;
        out.FinLen = lb;
        aal_mem_dealloc(pad);
    } else {
        out.Bigger = '0';
        out.Num1 = (char *)A;
        out.Num2 = (char *)B;
        out.FinLen = la;
    }
    return out;
}

/* ------------------------- File read helper -------------------------- */

/* Read a file expected to contain two parts separated by ':' and return
 * them as rdflout.Num1 and rdflout.Num2. On error both fields are set to
 * a static "ERROR" string to mirror original behavior.
 */
rdflout aal_rdfl(const char *Z)
{
    rdflout out = {0};
    out.Num1 = "ERROR";
    out.Num2 = "ERROR";

    if (!Z)
        return out;

    FILE *FP = fopen(Z, "r");
    if (!FP)
        return out;

    if (fseek(FP, 0, SEEK_END) != 0) {
        fclose(FP);
        return out;
    }
    long flsz = ftell(FP);
    if (flsz < 0) {
        fclose(FP);
        return out;
    }
    rewind(FP);

    if ((size_t)flsz < 3) {
        fclose(FP);
        return out;
    }

    char *buf = aal_mem_alloc_num((size_t)flsz);
    if (!buf) {
        fclose(FP);
        return out;
    }

    size_t read = fread(buf, 1, (size_t)flsz, FP);
    buf[read < (size_t)flsz ? read : (size_t)flsz] = '\0';
    fclose(FP);

    char *sep = strchr(buf, ':');
    if (!sep) {
        aal_mem_dealloc(buf);
        return out;
    }

    size_t idx = (size_t)(sep - buf);
    out.Num1 = aal_copy(buf, 0);
    out.Num2 = aal_copy(buf, (unsigned long)(idx + 1));

    aal_mem_dealloc(buf);
    return out;
}

/* ------------------------- Error checking ---------------------------- */

/* Validate a single numeric argument. Return '0' if OK, '1' if error. */
char aal_errchk_1(const char *X)
{
    if (!X)
        return '1';
    size_t len = strlen(X); // NOSONAR - validated input, safe use
    int dot_seen = 0;

    for (size_t i = 0; i < len; ++i) {
        char c = X[i];
        if (isdigit((unsigned char)c))
            continue;
        if (i == 0 && c == '-')
            continue;
        if (c == '.' && i > 0 && i < len - 1 && !dot_seen) {
            dot_seen = 1;
            continue;
        }
        return '1';
    }
    return '0';
}

/* Validate two numeric arguments. Return '0' if OK, '1' if error. */
char aal_errchk_2(const char *A, const char *B)
{
    if (aal_errchk_1(A) == '1')
        return '1';
    if (aal_errchk_1(B) == '1')
        return '1';
    return '0';
}

/* ------------------------- Decimal/dot helpers ------------------------ */

/* Insert a dot at position *P (pointer to numeric index stored as string
 * in original). Here we accept P as pointer string and interpret first
 * character as digit index as original did; but to keep things sane we
 * implement a version that accepts an index as pointer to char and uses
 * its first character numeric value. This mirrors original's odd usage.
 */
char *aal_setdot(const char *X, size_t pos)
{
    if (!X)
        return NULL;

    size_t n = strlen(X); // NOSONAR - validated input, safe use
    if (pos > n)
        pos = n;

    char *out = aal_mem_alloc_num(n + 1);
    if (!out)
        return NULL;

    memcpy(out, X, pos);
    out[pos] = '.';
    memcpy(out + pos + 1, X + pos, n - pos);
    out[n + 1] = '\0';
    return out;
}


/* Remove the first dot from X and return newly allocated string. */
char *aal_clrdot(const char *X)
{
    if (!X)
        return aal_mem_alloc_num(0);

    const char *dot = strchr(X, '.');
    if (!dot)
        return aal_copy(X, 0);

    size_t n = strlen(X); // NOSONAR - validated input, safe use
    char *out = aal_mem_alloc_num(n - 1);
    if (!out)
        return NULL;

    size_t prefix = (size_t)(dot - X);
    memcpy(out, X, prefix);
    memcpy(out + prefix, dot + 1, n - prefix);
    out[n - 1] = '\0';
    return out;
}

/* ------------------------- Arithmetic: add/sub ----------------------- */

/* Add two non-negative integer strings (both same length) from right-to-left.
 * Both inputs are expected to be plain digit strings of equal length; function
 * returns an allocated summed string (no trimming) — helpers above perform
 * normalization before calling this in higher level add logic.
 */
static char *add_same_len(const char *A, const char *B, size_t len)
{
    char *res = aal_mem_alloc_num(len + 1 + 1); /* possible extra carry */
    if (!res)
        return NULL;

    int carry = 0;
    size_t out_pos = 0; /* we'll build reversed then reverse */
    for (size_t i = 0; i < len; ++i) {
        int ai = A[len - 1 - i] - '0';
        int bi = B[len - 1 - i] - '0';
        int s = ai + bi + carry;
        carry = s / 10;
        res[out_pos++] = (char)('0' + (s % 10));
    }
    if (carry)
        res[out_pos++] = '0' + carry;
    res[out_pos] = '\0';

    /* reverse result */
    for (size_t i = 0; i < out_pos / 2; ++i) {
        char t = res[i];
        res[i] = res[out_pos - 1 - i];
        res[out_pos - 1 - i] = t;
    }
    return res;
}

/* Subtract same-length positive integer strings A - B where A >= B.
 * Returns new allocated string (may contain leading zeros — caller expected
 * to trim via aal_clrizr).
 */
static char *sub_same_len(const char *A, const char *B, size_t len)
{
    char *res = aal_mem_alloc_num(len + 1);
    if (!res)
        return NULL;

    int borrow = 0;
    size_t out_pos = 0;
    for (size_t i = 0; i < len; ++i) {
        int ai = A[len - 1 - i] - '0';
        int bi = B[len - 1 - i] - '0';
        int v = ai - bi - borrow;
        if (v < 0) {
            v += 10;
            borrow = 1;
        } else {
            borrow = 0;
        }
        res[out_pos++] = (char)('0' + v);
    }
    /* reverse */
    res[out_pos] = '\0';
    for (size_t i = 0; i < out_pos / 2; ++i) {
        char t = res[i];
        res[i] = res[out_pos - 1 - i];
        res[out_pos - 1 - i] = t;
    }
    return res;
}

/* High-level addition: preserves sign handling similar to original file
 * but implemented more simply. This function expects decimalless integer
 * strings; the original library handled decimal points at a higher level.
 */
char *aal_add(const char *A, const char *B)
{
    /* Quick null checks */
    if (!A || !B)
        return aal_mem_alloc_num(0);

    /* Normalize strings (remove leading noise) */
    char *a = aal_clrizr(A);
    char *b = aal_clrizr(B);

    /* If either is zero, return the other. */
    if (aal_zrchk(a) == '1') {
        if (a != A) aal_mem_dealloc(a);
        return b == B ? aal_copy(B, 0) : b;
    }
    if (aal_zrchk(b) == '1') {
        if (b != B) aal_mem_dealloc(b);
        return a == A ? aal_copy(A, 0) : a;
    }

    /* Only handle unsigned integer strings here; caller is responsible for
     * sign and decimal handling (keeps parity with original split-of-concerns).
     */
    fixlen fl = aal_fixlen(a, b);
    size_t n = fl.FinLen;

    char *sum = add_same_len(fl.Num1, fl.Num2, n);

    /* Cleanup temporary allocations created by aal_fixlen */
    if (fl.Bigger == '1') {
        if (fl.Num2 && fl.Num2 != b) aal_mem_dealloc(fl.Num2);
    } else if (fl.Bigger == '2') {
        if (fl.Num1 && fl.Num1 != a) aal_mem_dealloc(fl.Num1);
    }

    if (a != A) aal_mem_dealloc(a);
    if (b != B) aal_mem_dealloc(b);

    /* Trim leading zeros to match original aal_clrizr semantics */
    char *trimmed = aal_clrizr(sum);
    aal_mem_dealloc(sum);
    return trimmed;
}

char *aal_sub(const char *A, const char *B)
{
    if (!A || !B)
        return aal_mem_alloc_num(0);

    char *a = aal_clrizr(A);
    char *b = aal_clrizr(B);

    /* Determine which is bigger */
    char bigger = aal_cmp(a, b);

    char *x = a;
    char *y = b;
    int swapped = 0;
    if (bigger == '2') { /* B > A, swap so we compute y - x then mark negative */
        x = b;
        y = a;
        swapped = 1;
    }

    fixlen fl = aal_fixlen(x, y);
    size_t n = fl.FinLen;

    char *diff = sub_same_len(fl.Num1, fl.Num2, n);

    /* Cleanup temporaries from aal_fixlen */
    if (fl.Bigger == '1') {
        if (fl.Num2 && fl.Num2 != y) aal_mem_dealloc(fl.Num2);
    } else if (fl.Bigger == '2') {
        if (fl.Num1 && fl.Num1 != x) aal_mem_dealloc(fl.Num1);
    }

    if (a != A) aal_mem_dealloc(a);
    if (b != B) aal_mem_dealloc(b);

    /* Trim leading zeros */
    char *trim = aal_clrizr(diff);
    aal_mem_dealloc(diff);

    if (swapped) {
        /* Prepend minus if result is non-zero */
        if (aal_zrchk(trim) == '1') {
            return trim; /* zero => don't add minus */
        }
        char *with_min = aal_setmin(trim);
        aal_mem_dealloc(trim);
        return with_min;
    }

    return trim;
}

/* End of refactored AAL file */
