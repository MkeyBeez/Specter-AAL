#ifndef __CONSOLE_H__
#define __CONSOLE_H__

/* Libraries */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* AAL Header */
#ifndef AAL_H
#include "aal.h"
#endif

/* Function declarations */
long getCurrentTimeMs(void);
void safeStringInput(char* buffer, int maxLen);
void performOperation(int operation, const char* input1, const char* input2);
void handleKeyboardInput(int operation);
void handleFileInput(int operation);

/* Main Function */
int main(int argc, char *argv[]);

#endif /* __CONSOLE_H__ */
