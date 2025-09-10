/******************************************************************************/
/*                                   Specter                                  */
/*                      	   <<Specter Library>>                            */
/*                              George Delaportas                             */
/*                            Copyright © 2010-2025                           */
/******************************************************************************/



/* Headers */
#include "headers/specter.h"
#include "aal.h"

/* Specter - Initializer */
char *specter_initializer(char *A, char *B, int V)
{
	char Err;
	char ZeroA;
	char ZeroB;
	char MinA;
	char MinB;
	char Bigger;
	char *Result = aal_mem_alloc_2(A, B);
	uintptr_t DotA;
	uintptr_t DotB;
	
	Err = aal_errchk_2(A, B);
	
	if ((aal_len(A) == 1 && isdigit(A[0]) == 0) || (aal_len(B) == 1 && isdigit(B[0]) == 0))
		Err = '1';
	
	if (V < 1 || V > 2)
		Err = '1';
	
	if (Err == '1')
	{
		Result[0] = '#';
		Result[1] = '\0';
	}
	else
	{
		A = aal_clrizr(A);
		B = aal_clrizr(B);
		
		MinA = aal_minchk(A);
		MinB = aal_minchk(B);
		
		ZeroA = aal_zrchk(A);
		ZeroB = aal_zrchk(B);
		
		DotA = aal_dotchk(A);
		DotB = aal_dotchk(B);
		
		if (V == 1)
		{
			if (ZeroA == '1')
			{
				if (MinB == '1')
					B = aal_clrmin(B);
				
				Result = B;
			}
			else if (ZeroB == '1')
			{
				if (MinA == '1')
					A = aal_clrmin(A);
				
				Result = A;
			}
			else
			{
				if (MinA == '0' && MinB == '0')
					Result = aal_add(A, B);
				else if (MinA == '1' && MinB == '1')
				{
					A = aal_clrmin(A);
					B = aal_clrmin(B);
	
					Result = aal_add(A, B);
					Result = aal_setmin(Result);
				}
				else
				{
					if (MinA == '1')
					{
						A = aal_clrmin(A);
						
						Result = aal_sub(A, B);
						
						Bigger = aal_cmp(A, B);
						
						if (Bigger == '1')
							Result = aal_setmin(Result);
					}
					
					if (MinB == '1')
					{
						B = aal_clrmin(B);
						
						Result = aal_sub(A, B);
						
						Bigger = aal_cmp(A, B);
						
						if (Bigger == '2')
							Result = aal_setmin(Result);
					}
				}
			}
		}
		else
		{
			if (ZeroA == '1')
			{
				if (MinB == '0')
					B = aal_setmin(B);
				else
					B = aal_clrmin(B);
				
				Result = B;
			}
			else if (ZeroB == '1')
				Result = A;
			else
			{
				if (MinA == '0' && MinB == '0')
				{
					Result = aal_sub(A, B);
					
					Bigger = aal_cmp(A, B);
					
					if (Bigger == '2')
						Result = aal_setmin(Result);
				}
				else if (MinA == '1' && MinB == '1')
				{
					A = aal_clrmin(A);
					B = aal_clrmin(B);
					
					Result = aal_sub(A, B);
					
					Bigger = aal_cmp(A, B);
					
					if (Bigger == '1')
						Result = aal_setmin(Result);
				}
				else
				{
					if (MinA == '1')
					{
						A = aal_clrmin(A);
						
						Result = aal_add(A, B);
						Result = aal_setmin(Result);
					}
					
					if (MinB == '1')
					{
						B = aal_clrmin(B);
						
						Result = aal_add(A, B);
					}
				}
			}
		}
	}
	
	return aal_clrizr(Result);
}

/* Specter - Addition */
char *specter_add(char *A, char *B)
{	
	return specter_initializer(A, B, 1);
}

/* Specter - Subtraction */
char *specter_sub(char *A, char *B)
{
	return specter_initializer(A, B, 2);
}

/******************************************************************************/
