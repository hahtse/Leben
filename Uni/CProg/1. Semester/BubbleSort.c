//BubbleSort.c
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main()
{
	int n, i, j,k=0,summe=0,median=0;
	srand(time(NULL));
	n=(rand()%10)+1;
	int a[n];
	printf("\n\n!!BubbleSort!!\n\n");
	for(i=0;i<n;i++)
	{
		a[i]=rand()%101;
		printf("\t%d",a[i]);
	}
	printf("\n\n");
	for(i=(n-1);i>=1;i--)
	{
		for(j=0;j<=(i-1);j++)
		{
			if(a[j]>a[j+1])
			{
				k=a[j];
				a[j]=a[j+1];
				a[j+1]=k;
			}
		}
	}
	for(i=0;i<n;i++)
	{
		printf("\t%d",a[i]);
	}
	for(i=0;i<n;i++)
	{
		summe+=a[i];
	}
	summe=summe/n;
	printf("\n\narithmetisches Mittel: %d\n",summe);
	return 0;
}




