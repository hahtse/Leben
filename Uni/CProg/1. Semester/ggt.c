//ggtE.c
#include <stdio.h>
#include <limits.h>
#include <math.h>

main()
{
	int m,n,r;
	
	printf("\nWillkommen! Bitte beachten sie, dass das Programm nur für natürliche Zahlen die groesser als 0 sind funktioniert!\n");
	printf("\nBitte erste Zahl eingeben!\n");
	scanf("%d" , &m);
	printf("\nBitte zweite Zahl eingeben!\n");
	scanf("%d", &n);

	do 
	{
		r= m%n;
		m=n;
		n=r;
	} while (n>0);

	printf("\nDas GGT ist: %d\n\n", m);
}


