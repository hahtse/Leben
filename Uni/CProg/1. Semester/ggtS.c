//ggt mit Subtraktionsalgorithmus

#include <stdio.h>
#include <math.h>

main()
{
	int m,n;

	printf("\nm?\n");
	scanf("%d", &m);
	printf("\nn\n");
	scanf("%d", &n);

	while (m!=n)
	{
		if (m>n)
		{
			m = m-n;
		}
		else
		{
			n = n-m;
		}
	}

	if (m!=1)
		printf("\nGGT: %d\n", m);
	else
		printf("\nTeilerfremd!\n");
}
