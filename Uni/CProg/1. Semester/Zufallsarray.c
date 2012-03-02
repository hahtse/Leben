//Zufallsarray.c
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <limits.h>


int main()
{
	int zufallszahl, n, i,summe=0,produkt=1;
	srand(time(NULL));
	zufallszahl=(rand()%20)+1;
	int array[zufallszahl];
	printf("\nZufallszahlenarray\n");
	for(i=0;i<zufallszahl;i++)
	{
		n=rand()%10;	
		array[i]=n;
		printf("\t%d", array[i]);
	}
	for(i=0;i<zufallszahl;i++)
	{
		summe+=array[i];
	}
	printf("\n\nSumme: %d\n",summe);
	for(i=0;i<zufallszahl;i++)
	{
		if(produkt<=(INT_MAX/array[i]))
			produkt=produkt*array[i];
	}
	printf("\nProdukt: %d\n",produkt);
	return 0;
}
