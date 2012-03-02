//ggtA.c
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

	while (n!=0)
	{
		r= m%n;
		m=n;
		n=r;
	}

	printf("\nDas GGT ist: %d\n\n", m);
}


