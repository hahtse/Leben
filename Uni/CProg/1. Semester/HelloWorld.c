//HelloWorld.c
#include <stdio.h>
#include <time.h>
#include <stdlib.h>


int main()
{
	int zufallszahl, n, i;
	srand(time(NULL));
	zufallszahl=(rand()%20)+1;
	int array[zufallszahl];
	printf("/nZufallszahlenarray/n");
	for(i=0,i<zufallszahl,i++)
	{
		srand(time(NULL));
		n=rand()%10		
		array[i]=n;
		printf("/t%d", array[i]);
	}
	return 0;
}
