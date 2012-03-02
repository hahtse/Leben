//AddMultWholeTab.c
#include <stdio.h>
main()
{
	int k, sum=0,n=0,mult=1,min;
	printf("Bitte ganze Zahl eingeben!\n");
	scanf("%d" , &k);
	min=k;
	printf("Zahl	Summe	Produkt	Anzahl\n");
	while(k!=0) {
		sum+=k;
		mult=mult*k;
		n++;
		if (k<min)
		{
			min=k;
		}
		printf("%d	%d	%d	%d\n",k,sum,mult,n);
		scanf("%d",&k);
	}
	printf("Die Summe ist: %d! \n Das Produkt ist: %d! Der kleinste Eingabewert ist %d\n",sum,mult,min);
}


