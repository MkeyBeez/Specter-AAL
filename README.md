# Specter-AAL (early alpha)

[![Codacy Badge](https://app.codacy.com/project/badge/Grade/889562a17e174c438fd56d35780822b0)](https://app.codacy.com/gh/g0d/Specter-AAL/dashboard)

Specter - Abstract Arithmetics Library (AAL) is a library coded in native C which is able to do operations with integer numbers of virtually unlimited length. The scope and architecture of AAL is different from any other implementation because it is hardware (architecture) and bit (endianness) agnostic. It is also optimized for running even on embedded ICs.
For now though, the only functional math operations are addition and subtraction but in the future it will be able to run all other major operations.

TODO: 
void freeBigFloat(BigFloat* bf) - to properly clean up allocated memory
BigFloat copyBigFloat(BigFloat bf) - for safe copying
int compareBigFloat(BigFloat a, BigFloat b) - for comparisons
clean up memory freeing of strings once everything else works
