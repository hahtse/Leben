//ASCII.c

#include<stdio.h>

main()
{
	int n=33;

	while (n<=126)
	{
		if (n%3!=0)
			printf("\t|\t");
		else
			printf("\n");
		printf(" %d\t%c", n, n);
		n++;
	}
	printf("\n\n");
}
