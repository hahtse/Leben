//AddWholeTab.c
#include <stdio.h>
main()
{
	int k, sum=0,n=1;
	printf("Bitte ganze Zahl eingeben: ");
	scanf("%d" , &k);
	printf("\nZahl	Summe	Anzahl");
	while(k!=0) {
		sum+=k;
		scanf("%d" , &k);
		printf("	%d	%d",sum,n);
		n++;
	}
	printf("Das Ergebnis ist: %d! \n", sum);
}


