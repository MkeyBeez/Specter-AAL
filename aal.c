/******************************************************************************/
/*                                   Specter                                  */
/*                      <<Abstract Arithmetics Library>>                      */
/*                              George Delaportas                             */
/*                            Copyright © 2010-2025                           */
/******************************************************************************/

#include "aal.h"

/* AAL - Length */
size_t aal_length(const char *str) {
    if (str == NULL) {
        return 0;
    }
    return strlen(str);
}

/* AAL - Copy substring starting at position */
char *aal_copy_from_position(const char *source, size_t position) {
    if (source == NULL) {
        return NULL;
    }
    
    size_t source_len = aal_length(source);
    if (position >= source_len) {
        return NULL;
    }
    
    char *result = aal_allocate_like_string(source);
    if (result == NULL) {
        return NULL;
    }
    
    size_t result_index = 0;
    for (size_t i = position; i < source_len; i++) {
        char current_char = source[i];
        if (isdigit(current_char) || current_char == '-' || current_char == '.') {
            result[result_index++] = current_char;
        } else {
            break;
        }
    }
    
    result[result_index] = '\0';
    return result;
}

/* AAL - Compare two number strings */
aal_comparison_t aal_compare(const char *a, const char *b) {
    if (a == NULL || b == NULL) {
        return AAL_EQUAL;
    }
    
    char *clean_a = aal_clear_leading_zeros(a);
    char *clean_b = aal_clear_leading_zeros(b);
    
    if (clean_a == NULL || clean_b == NULL) {
        aal_free_string(clean_a);
        aal_free_string(clean_b);
        return AAL_EQUAL;
    }
    
    bool zero_a = aal_is_zero(clean_a);
    bool zero_b = aal_is_zero(clean_b);
    
    if (zero_a && zero_b) {
        aal_free_string(clean_a);
        aal_free_string(clean_b);
        return AAL_EQUAL;
    }
    
    size_t len_a = aal_length(clean_a);
    size_t len_b = aal_length(clean_b);
    bool minus_a = aal_has_minus_sign(clean_a);
    bool minus_b = aal_has_minus_sign(clean_b);
    
    aal_comparison_t result = AAL_EQUAL;
    
    // Handle sign differences
    if (minus_a && !minus_b) {
        result = AAL_SECOND_GREATER;
    } else if (!minus_a && minus_b) {
        result = AAL_FIRST_GREATER;
    } else {
        // Same signs, compare by length and digits
        if (len_a > len_b) {
            result = minus_a ? AAL_SECOND_GREATER : AAL_FIRST_GREATER;
        } else if (len_a < len_b) {
            result = minus_b ? AAL_FIRST_GREATER : AAL_SECOND_GREATER;
        } else {
            // Same length, compare digit by digit
            for (size_t i = 0; i < len_a; i++) {
                if (clean_a[i] > clean_b[i]) {
                    result = minus_a ? AAL_SECOND_GREATER : AAL_FIRST_GREATER;
                    break;
                } else if (clean_a[i] < clean_b[i]) {
                    result = minus_b ? AAL_FIRST_GREATER : AAL_SECOND_GREATER;
                    break;
                }
            }
        }
    }
    
    aal_free_string(clean_a);
    aal_free_string(clean_b);
    return result;
}

/* AAL - Reverse string */
char *aal_reverse(const char *str) {
    if (str == NULL) {
        return NULL;
    }
    
    size_t len = aal_length(str);
    char *reversed = aal_allocate_string(len + 1);
    if (reversed == NULL) {
        return NULL;
    }
    
    if (len <= 1) {
        strcpy(reversed, str);
    } else {
        for (size_t i = 0; i < len; i++) {
            reversed[i] = str[len - i - 1];
        }
    }
    
    reversed[len] = '\0';
    return reversed;
}

/* AAL - Pad string with prefix */
char *aal_pad_prefix(const char *str, const char *prefix) {
    if (str == NULL || prefix == NULL) {
        return NULL;
    }
    
    size_t str_len = aal_length(str);
    size_t prefix_len = aal_length(prefix);
    
    char *padded = aal_allocate_string(str_len + prefix_len + 1);
    if (padded == NULL) {
        return NULL;
    }
    
    strcpy(padded, prefix);
    strcat(padded, str);
    
    return padded;
}

/* AAL - Check if string represents zero */
bool aal_is_zero(const char *str) {
    if (str == NULL) {
        return false;
    }
    
    char *abs_str = aal_clear_minus_sign(str);
    if (abs_str == NULL) {
        return false;
    }
    
    bool is_zero = true;
    size_t len = aal_length(abs_str);
    
    for (size_t i = 0; i < len; i++) {
        if (abs_str[i] != '0' && abs_str[i] != '.') {
            is_zero = false;
            break;
        }
    }
    
    aal_free_string(abs_str);
    return is_zero;
}

/* AAL - Clear leading zeros */
char *aal_clear_leading_zeros(const char *str) {
    if (str == NULL) {
        return NULL;
    }
    
    size_t len = aal_length(str);
    size_t start = 0;
    
    // Skip leading zeros, but preserve minus sign
    if (len > 0 && str[0] == '-') {
        start = 1;
    }
    
    while (start < len && str[start] == '0') {
        start++;
    }
    
    // If we've consumed all digits, keep one zero
    if (start == len || (start < len && !isdigit(str[start]) && str[start] != '.')) {
        start--;
    }
    
    char *result = aal_copy_from_position(str, start > 0 ? start : 0);
    
    // Handle the case where we need to preserve the minus sign
    if (str[0] == '-' && result != NULL && result[0] != '-') {
        char *temp = aal_set_minus_sign(result);
        aal_free_string(result);
        result = temp;
    }
    
    // Ensure we have at least "0" for zero values
    int decimal_pos = aal_find_decimal_point(result);
    if (result != NULL && decimal_pos == 0) {
        char *temp = aal_pad_prefix(result, "0");
        aal_free_string(result);
        result = temp;
    }
    
    return result;
}

/* AAL - Check for minus sign */
bool aal_has_minus_sign(const char *str) {
    return (str != NULL && str[0] == '-');
}

/* AAL - Add minus sign */
char *aal_set_minus_sign(const char *str) {
    if (str == NULL) {
        return NULL;
    }
    
    return aal_pad_prefix(str, "-");
}

/* AAL - Remove minus sign */
char *aal_clear_minus_sign(const char *str) {
    if (str == NULL || !aal_has_minus_sign(str)) {
        if (str == NULL) return NULL;
        
        char *result = aal_allocate_like_string(str);
        if (result != NULL) {
            strcpy(result, str);
        }
        return result;
    }
    
    size_t len = aal_length(str);
    char *result = aal_allocate_string(len);
    if (result == NULL) {
        return NULL;
    }
    
    strcpy(result, str + 1);
    return result;
}

/* AAL - Find decimal point position */
int aal_find_decimal_point(const char *str) {
    if (str == NULL) {
        return -1;
    }
    
    for (size_t i = 0; str[i] != '\0'; i++) {
        if (str[i] == '.') {
            return (int)i;
        }
    }
    
    return -1;
}

/* AAL - Insert decimal point at position */
char *aal_set_decimal_point(const char *str, size_t position) {
    if (str == NULL) {
        return NULL;
    }
    
    size_t len = aal_length(str);
    if (position > len) {
        return NULL;
    }
    
    char *result = aal_allocate_string(len + 2);
    if (result == NULL) {
        return NULL;
    }
    
    strncpy(result, str, position);
    result[position] = '.';
    strcpy(result + position + 1, str + position);
    
    return result;
}

/* AAL - Remove decimal point */
char *aal_clear_decimal_point(const char *str) {
    if (str == NULL) {
        return NULL;
    }
    
    int dot_pos = aal_find_decimal_point(str);
    if (dot_pos < 0) {
        char *result = aal_allocate_like_string(str);
        if (result != NULL) {
            strcpy(result, str);
        }
        return result;
    }
    
    size_t len = aal_length(str);
    char *result = aal_allocate_string(len);
    if (result == NULL) {
        return NULL;
    }
    
    strncpy(result, str, (size_t)dot_pos);
    strcpy(result + dot_pos, str + dot_pos + 1);
    
    return result;
}

/* AAL - Equalize string lengths with padding */
aal_fixlen_t aal_equalize_lengths(const char *a, const char *b) {
    aal_fixlen_t result = {AAL_EQUAL, NULL, NULL, 0};
    
    if (a == NULL || b == NULL) {
        result.error = AAL_ERROR_NULL_POINTER;
        return result;
    }
    
    size_t len_a = aal_length(a);
    size_t len_b = aal_length(b);
    
    if (len_a > len_b) {
        size_t diff = len_a - len_b;
        char *zeros = aal_allocate_string(diff + 1);
        if (zeros == NULL) {
            result.error = AAL_ERROR_MEMORY_ALLOCATION;
            return result;
        }
        
        for (size_t i = 0; i < diff; i++) {
            zeros[i] = '0';
        }
        zeros[diff] = '\0';
        
        result.bigger = AAL_FIRST_GREATER;
        result.num1 = aal_allocate_like_string(a);
        result.num2 = aal_pad_prefix(b, zeros);
        result.final_length = len_a;
        
        if (result.num1 != NULL) strcpy(result.num1, a);
        aal_free_string(zeros);
        
    } else if (len_a < len_b) {
        size_t diff = len_b - len_a;
        char *zeros = aal_allocate_string(diff + 1);
        if (zeros == NULL) {
            result.error = AAL_ERROR_MEMORY_ALLOCATION;
            return result;
        }
        
        for (size_t i = 0; i < diff; i++) {
            zeros[i] = '0';
        }
        zeros[diff] = '\0';
        
        result.bigger = AAL_SECOND_GREATER;
        result.num1 = aal_pad_prefix(a, zeros);
        result.num2 = aal_allocate_like_string(b);
        result.final_length = len_b;
        
        if (result.num2 != NULL) strcpy(result.num2, b);
        aal_free_string(zeros);
        
    } else {
        result.bigger = AAL_EQUAL;
        result.num1 = aal_allocate_like_string(a);
        result.num2 = aal_allocate_like_string(b);
        result.final_length = len_a;
        
        if (result.num1 != NULL) strcpy(result.num1, a);
        if (result.num2 != NULL) strcpy(result.num2, b);
    }
    
    return result;
}

/* AAL - Read two numbers from file */
aal_rdfile_t aal_read_file(const char *filename) {
    aal_rdfile_t result = {NULL, NULL, AAL_SUCCESS};
    
    if (filename == NULL) {
        result.error = AAL_ERROR_NULL_POINTER;
        return result;
    }
    
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        result.error = AAL_ERROR_FILE_IO;
        return result;
    }
    
    // Get file size
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    rewind(file);
    
    if (file_size < 3) {
        result.error = AAL_ERROR_INVALID_INPUT;
        fclose(file);
        return result;
    }
    
    char *file_content = aal_allocate_string((size_t)file_size + 1);
    if (file_content == NULL) {
        result.error = AAL_ERROR_MEMORY_ALLOCATION;
        fclose(file);
        return result;
    }
    
    size_t bytes_read = fread(file_content, 1, (size_t)file_size, file);
    file_content[bytes_read] = '\0';
    fclose(file);
    
    // Find separator ':'
    char *separator = strchr(file_content, ':');
    if (separator == NULL) {
        result.error = AAL_ERROR_INVALID_INPUT;
        aal_free_string(file_content);
        return result;
    }
    
    *separator = '\0';  // Split string at separator
    
    result.num1 = aal_allocate_string(strlen(file_content) + 1);
    result.num2 = aal_allocate_string(strlen(separator + 1) + 1);
    
    if (result.num1 == NULL || result.num2 == NULL) {
        result.error = AAL_ERROR_MEMORY_ALLOCATION;
        aal_free_string(result.num1);
        aal_free_string(result.num2);
        result.num1 = result.num2 = NULL;
    } else {
        strcpy(result.num1, file_content);
        strcpy(result.num2, separator + 1);
    }
    
    aal_free_string(file_content);
    return result;
}

/* AAL - Validate single number string */
aal_error_t aal_validate_number(const char *str) {
    if (str == NULL) {
        return AAL_ERROR_NULL_POINTER;
    }
    
    size_t len = aal_length(str);
    if (len == 0) {
        return AAL_ERROR_INVALID_INPUT;
    }
    
    bool has_dot = false;
    
    for (size_t i = 0; i < len; i++) {
        if (isdigit(str[i])) {
            continue;
        } else if (str[i] == '-' && i == 0) {
            continue;
        } else if (str[i] == '.' && !has_dot && i != 0 && i != len - 1) {
            has_dot = true;
        } else {
            return AAL_ERROR_INVALID_INPUT;
        }
    }
    
    return AAL_SUCCESS;
}

/* AAL - Validate two number strings */
aal_error_t aal_validate_two_numbers(const char *a, const char *b) {
    aal_error_t error_a = aal_validate_number(a);
    if (error_a != AAL_SUCCESS) {
        return error_a;
    }
    
    return aal_validate_number(b);
}

/* AAL - Memory allocation helpers */
char *aal_allocate_string(size_t length) {
    return calloc(length, sizeof(char));
}

char *aal_allocate_like_string(const char *str) {
    if (str == NULL) {
        return NULL;
    }
    return aal_allocate_string(aal_length(str) + 1);
}

char *aal_allocate_for_two_strings(const char *a, const char *b) {
    if (a == NULL || b == NULL) {
        return NULL;
    }
    
    size_t len_a = aal_length(a);
    size_t len_b = aal_length(b);
    size_t max_len = (len_a >= len_b) ? len_a : len_b;
    
    return aal_allocate_string(max_len + 3);  // Extra space for operations
}

/* AAL - Memory deallocation */
void aal_free_string(char *str) {
    if (str != NULL) {
        free(str);
    }
}

void aal_free_fixlen_result(aal_fixlen_t *result) {
    if (result != NULL) {
        aal_free_string(result->num1);
        aal_free_string(result->num2);
        result->num1 = result->num2 = NULL;
    }
}

void aal_free_rdfile_result(aal_rdfile_t *result) {
    if (result != NULL) {
        aal_free_string(result->num1);
        aal_free_string(result->num2);
        result->num1 = result->num2 = NULL;
    }
}

/* AAL - Addition */
char *aal_add(const char *a, const char *b) {
    if (aal_validate_two_numbers(a, b) != AAL_SUCCESS) {
        return NULL;
    }
    
    aal_fixlen_t equalized = aal_equalize_lengths(a, b);
    if (equalized.num1 == NULL || equalized.num2 == NULL) {
        aal_free_fixlen_result(&equalized);
        return NULL;
    }
    
    char *result = aal_allocate_for_two_strings(equalized.num1, equalized.num2);
    if (result == NULL) {
        aal_free_fixlen_result(&equalized);
        return NULL;
    }
    
    int carry = 0;
    size_t result_index = 0;
    
    // Process from right to left
    for (int i = (int)equalized.final_length - 1; i >= 0; i--) {
        int digit_a = equalized.num1[i] - '0';
        int digit_b = equalized.num2[i] - '0';
        int sum = digit_a + digit_b + carry;
        
        if (sum > 9) {
            carry = 1;
            sum -= 10;
        } else {
            carry = 0;
        }
        
        result[result_index++] = (char)(sum + '0');
    }
    
    if (carry > 0) {
        result[result_index++] = '1';
    }
    
    result[result_index] = '\0';
    
    // Reverse the result
    char *final_result = aal_reverse(result);
    aal_free_string(result);
    aal_free_fixlen_result(&equalized);
    
    // Clear leading zeros
    char *cleaned_result = aal_clear_leading_zeros(final_result);
    aal_free_string(final_result);
    
    return cleaned_result;
}

/* AAL - Subtraction */
char *aal_subtract(const char *a, const char *b) {
    if (aal_validate_two_numbers(a, b) != AAL_SUCCESS) {
        return NULL;
    }
    
    // Determine which number is larger
    aal_comparison_t comparison = aal_compare(a, b);
    const char *larger = a;
    const char *smaller = b;
    bool result_negative = false;
    
    if (comparison == AAL_SECOND_GREATER) {
        larger = b;
        smaller = a;
        result_negative = true;
    } else if (comparison == AAL_EQUAL) {
        char *zero = aal_allocate_string(2);
        if (zero != NULL) {
            strcpy(zero, "0");
        }
        return zero;
    }
    
    aal_fixlen_t equalized = aal_equalize_lengths(larger, smaller);
    if (equalized.num1 == NULL || equalized.num2 == NULL) {
        aal_free_fixlen_result(&equalized);
        return NULL;
    }
    
    char *result = aal_allocate_for_two_strings(equalized.num1, equalized.num2);
    if (result == NULL) {
        aal_free_fixlen_result(&equalized);
        return NULL;
    }
    
    int borrow = 0;
    size_t result_index = 0;
    
    // Process from right to left
    for (int i = (int)equalized.final_length - 1; i >= 0; i--) {
        int digit_larger = equalized.num1[i] - '0';
        int digit_smaller = equalized.num2[i] - '0';
        
        digit_larger -= borrow;
        
        if (digit_larger < digit_smaller) {
            digit_larger += 10;
            borrow = 1;
        } else {
            borrow = 0;
        }
        
        int diff = digit_larger - digit_smaller;
        result[result_index++] = (char)(diff + '0');
    }
    
    result[result_index] = '\0';
    
    // Reverse the result
    char *final_result = aal_reverse(result);
    aal_free_string(result);
    aal_free_fixlen_result(&equalized);
    
    // Clear leading zeros
    char *cleaned_result = aal_clear_leading_zeros(final_result);
    aal_free_string(final_result);
    
    // Add minus sign if needed
    if (result_negative && cleaned_result != NULL && !aal_is_zero(cleaned_result)) {
        char *negative_result = aal_set_minus_sign(cleaned_result);
        aal_free_string(cleaned_result);
        cleaned_result = negative_result;
    }
    
    return cleaned_result;
}

/******************************************************************************/
