:: Messages
@echo "************************************"
@echo "*           # Specter #            *"
@echo "*   Abstract Arithmetics Library   *"
@echo "*      Copyright (C) 2010-2025     *"
@echo "************************************"
@echo.
@echo "George Delaportas"
@echo "Copyright (C) 2010-2025"
@echo.
@echo "Installing..."

:: Compile
gcc -std=gnu99 -pedantic -O3 -c aal.c -lm

:: Link
gcc -o specter console.c aal.c -lm

:: Clean up
del *.o

:: Finalization
@echo "Done!"
