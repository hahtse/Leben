//matrix-Vektor|funktionen

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>


int main(void){

	//Initialisierung
	int *vektor, **matrix, zeilen, spalten, i=0,j=0,n;
	srand(time(NULL));
	

	//Groesse
	printf("\n\nZeilen der Matrix?\t");
	scanf("%d",&zeilen);
	printf("\n\nSpalten der Matrix?\t");
	scanf("%d",&spalten);
	
	//Speicherallozierung
	matrix = (int**) calloc(zeilen,sizeof(int*));
	
	for(i=0;i<zeilen;i++)
		matrix[i]=(int*) calloc(spalten,sizeof(int));
	
	vektor = (int*) calloc(spalten,sizeof(int));
	
	//Zuweisung
	for(j=0;j<spalten;j++)
		for(i=0;i<zeilen;i++)
			matrix[i][j]=rand()%10;
	
	for(i=0;i<spalten;i++)
		vektor[i]=rand()%10;
	

	//Ausgabe
	printf("\n\nDie Matrix:\n");
	for(i=0;i<zeilen;i++)
	{
		printf("\n");
		for(j=0;j<spalten;j++)
			printf("%d\t",matrix[i][j]);
	}
	

	printf("\nDer Vektor\n");
	for(i=0;i<spalten;i++)
		printf("%d\t",vektor[i]);
	
	//Produktberechnung
	printf("\n\nMatrix*Vektor:\n");
	for(i=0;i<zeilen;i++)
	{
		n=0;
		for(j=0;j<spalten;j++)
		{
			n+=(matrix[i][j]*vektor[j]);
		}
		printf("%d\t",n);
	}	

	printf("\n\n");

	//Freigabe
	for(i=0;i<zeilen;i++)
		free(matrix[i]);
	free(matrix);
	matrix=NULL;
	free(vektor);
	vektor=NULL;

	return 0;
}
	
