//AddWholeTab.c
#include <stdio.h>
main()
{
	int k, sum=0,n=0;
	printf("Bitte ganze Zahl eingeben!\n");
	scanf("%d" , &k);
	printf("Zahl	Summe	Anzahl\n");
	while(k!=0) {
		sum+=k;
		n++;
		printf("%d	%d	%d\n",k,sum,n);
		scanf("%d",&k);
	}
	printf("Das Ergebnis ist: %d! \n", sum);
}


