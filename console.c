/******************************************************************************/
/*                                   Specter                                  */
/*                                 <<Console>>                                */
/*                              George Delaportas                             */
/*                            Copyright © 2010-2025                           */
/*                         Refactored by Martin Tannhaus                      */
/******************************************************************************/
/* Headers */
#include "headers/console.h"
#include <time.h>

/* Utility function to get current time in milliseconds */
long getCurrentTimeMs() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
}

/* Function to safely read string input */
void safeStringInput(char* buffer, int maxLen) {
    if (fgets(buffer, maxLen, stdin) != NULL) {
        // Remove newline if present
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len-1] == '\n') {
            buffer[len-1] = '\0';
        }
    }
}

/* Function to perform the selected operation */
void performOperation(int operation, const char* input1, const char* input2) {
    BigFloat num1 = parseBigFloat(input1);
    BigFloat num2 = parseBigFloat(input2);
    BigFloat result;
    char* resultStr;
    long startTime, endTime;
    
    printf("\nCalculating...\n");
    startTime = getCurrentTimeMs();
    
    switch(operation) {
        case 1: // Addition
            result = addBigFloat(num1, num2);
            printf("Operation: %s + %s\n", input1, input2);
            break;
        case 2: // Subtraction
            result = subBigFloat(num1, num2);
            printf("Operation: %s - %s\n", input1, input2);
            break;
        case 3: // Multiplication
            result = mulBigFloat(num1, num2);
            printf("Operation: %s × %s\n", input1, input2);
            break;
        case 4: // Division
            printf("Enter precision (decimal places): ");
            int precision;
            scanf("%d", &precision);
            getchar(); // consume newline
            result = divBigFloat(num1, num2, precision);
            printf("Operation: %s ÷ %s (precision: %d)\n", input1, input2, precision);
            break;
        case 5: // Modulo
            result = modBigFloat(num1, num2);
            printf("Operation: %s mod %s\n", input1, input2);
            break;
        default:
            printf("Invalid operation!\n");
            return;
    }
    
    endTime = getCurrentTimeMs();
    
    resultStr = formatBigFloat(result);
    printf("Result: %s\n", resultStr);
    printf("\nBenchmark :: Delay: %ldms\n", endTime - startTime);
    
    // Cleanup
    free(resultStr);
    free(num1.digits);
    free(num2.digits);
    free(result.digits);
}

/* Function to handle keyboard input */
void handleKeyboardInput(int operation) {
    char input1[10000];
    char input2[10000];
    
    printf("\n* --- %s --- *\n", 
           operation == 1 ? "Addition" :
           operation == 2 ? "Subtraction" :
           operation == 3 ? "Multiplication" :
           operation == 4 ? "Division" :
           operation == 5 ? "Modulo" : "Unknown");
    
    printf("Please enter number 1: ");
    safeStringInput(input1, sizeof(input1));
    
    printf("Please enter number 2: ");
    safeStringInput(input2, sizeof(input2));
    
    performOperation(operation, input1, input2);
}

/* Function to handle file input */
void handleFileInput(int operation) {
    char filename[256];
    FILE* file;
    char input1[10000] = {0};
    char input2[10000] = {0};
    
    printf("\n* --- File Input --- *\n");
    printf("Enter filename: ");
    safeStringInput(filename, sizeof(filename));
    
    file = fopen(filename, "r");
    if (!file) {
        printf("Error: Could not open file '%s'\n", filename);
        return;
    }
    
    // Read two numbers from file
    if (fgets(input1, sizeof(input1), file) == NULL) {
        printf("Error: Could not read first number from file\n");
        fclose(file);
        return;
    }
    
    if (fgets(input2, sizeof(input2), file) == NULL) {
        printf("Error: Could not read second number from file\n");
        fclose(file);
        return;
    }
    
    fclose(file);
    
    // Remove newlines
    size_t len1 = strlen(input1);
    if (len1 > 0 && input1[len1-1] == '\n') input1[len1-1] = '\0';
    
    size_t len2 = strlen(input2);
    if (len2 > 0 && input2[len2-1] == '\n') input2[len2-1] = '\0';
    
    printf("Number 1 from file: %s\n", input1);
    printf("Number 2 from file: %s\n", input2);
    
    performOperation(operation, input1, input2);
}

/* Main Function */
int main(int argc, char *argv[]) {
    int operation = 0;
    int inputMethod = 0;
    
    /* Main Menu */
    printf("=====================\n");
    printf("*** Specter (AAL) ***\n");
    printf("---------------------\n");
    printf("    |Version 2.0|    \n");
    printf("=====================\n");
    printf("\n");
    printf("1. Addition\n");
    printf("2. Subtraction\n");
    printf("3. Multiplication\n");
    printf("4. Division\n");
    printf("5. Modulo\n");
    printf("6. Power (Not implemented)\n");
    printf("7. Sqrt (Not implemented)\n");
    printf("0. Exit\n");
    printf("\n");
    printf("Select: ");
    
    if (scanf("%d", &operation) != 1) {
        printf("Invalid input!\n");
        return 1;
    }
    getchar(); // consume newline
    
    /* First Level Choices */
    if (operation == 0) {
        printf("Goodbye!\n");
        return 0;
    }
    
    if (operation < 1 || operation > 7) {
        printf("\nWrong selection!\n");
        return 1;
    }
    
    if (operation == 6 || operation == 7) {
        printf("\nOperation not yet implemented in BigFloat library!\n");
        return 1;
    }
    
    /* Input Method Selection */
    printf("\n\n\n");
    printf("1. Keyboard input\n");
    printf("2. File input\n");
    printf("\n");
    printf("Select: ");
    
    if (scanf("%d", &inputMethod) != 1) {
        printf("Invalid input!\n");
        return 1;
    }
    getchar(); // consume newline
    
    /* Second Level Choices */
    if (inputMethod == 1) {
        handleKeyboardInput(operation);
    } else if (inputMethod == 2) {
        handleFileInput(operation);
    } else {
        printf("\nWrong selection!\n");
        return 1;
    }
    
    return 0;
}
/******************************************************************************/
