//Collatz.c
#include <stdio.h>
#include <limits.h>

main()
{
	unsigned long int i=0, n;
	printf("\nGib Wert für n ein (n nat Zahl)!  ");
	scanf("%lu" , &n);
	while (ULONG_MAX<=n)
	{
		printf("\n\nFehler! n ist zu gross, bitte neues n eingeben!\n");
		scanf("%lu" , &n);
	}
	printf("\n\ni\t\t|\tn\n--------------\t|---------------\n");
	while(n>1) 
	{
		if (n%2 == 0)
		{
			n = n/2;
		}
		else if ((ULONG_MAX-1)/3>n)
		{
			n = 3*n+1;
		}
		else
		{
			printf("\nFehler, n ist zu gross!\n\n");
			return 1;
		}
		i++;
		if (i==1000000)
		{
			printf("\nFehler, Folge terminiert nicht!\n\n");
			return 1;
		}
		printf("%lu \t\t| \t%lu\n",i,n);
	}
	printf("\nAnzahl der Iterationen: %lu\n", i);
}


