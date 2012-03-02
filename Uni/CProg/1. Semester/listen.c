//lustige listen
/*Probleme:	
				einzufuegende zahl wird ohne triggerzahl am ende eingefügt - das darf nicht sein (nich nachvollziehbar, da aufhänger)
		programm hängt sich an unterschiedlichen punkten bei zugriff auf pointer durch unterprogramme auf.
*/

#include <stdio.h>
#include <time.h>
#include <stdlib.h>

struct knoten
{
	int element;
	struct knoten *next;
};

void ausgabe(struct knoten *liste)
{	
	printf("\n\n");	
	while (liste!=NULL){
		printf(" %d ",liste->element);
		liste=liste->next;
	}
	printf("\n");
}
	

struct knoten* verkettung(struct knoten *liste1, struct knoten *liste2)
{
	struct knoten *lauf=NULL;
	
	if (liste1==NULL)
		return liste2;
	else {
		lauf=liste1;
		while(lauf->next!=NULL)
		{
			lauf=lauf->next;
		}
		lauf->next=liste2;
		return liste1;
	}
}

void einfuegen(struct knoten* liste)
{
	int suche, i;
	struct knoten *k, *lauf=liste;

	printf("\nBitte zu suchendes Element eingeben:  "); 
	scanf("%d",&suche);
	printf("\nBitte einzufuegendes Element angeben: ");
	scanf("%d",&i);

	while(liste!=NULL)
	{
		if(liste->element==suche)
		{
			k=(struct knoten*) calloc(1,sizeof(struct knoten));
			k->element=i;
			k->next=liste->next;
			liste->next=k;
			liste=k;
		}

		liste=liste->next;
	}
}

struct knoten* loeschen(struct knoten* liste)
{
	int i;
	struct knoten *lauf=liste, *rueck=liste;
	printf("\nBitte zu loeschende Knoten eingeben");
	scanf("%d",&i);
	while (liste!=NULL&&liste->element==i)
	{
		lauf=liste;
		liste=liste->next;
		rueck=liste;
		free(lauf);
	}
	if(liste==NULL)
		return NULL;
	else
		lauf=liste->next;
	while(lauf!=NULL)
	{
		if(lauf->element==i)
		{
			liste->next=lauf->next;
			free(lauf);
		}
		liste=lauf;
		if(liste!=NULL)
			lauf=liste->next;
	}
	return rueck;
}
	
void freigabe(struct knoten* liste)
{
	struct knoten *lauf=liste;
	while(liste!=NULL){
		lauf=liste->next;
		free(liste);
		liste=lauf;
	}
}

void addtwo(struct knoten* liste)
{
	int i, j;
	struct knoten *k, *lauf;
	while(liste!=NULL){
		i=0;
		j=0;
		lauf=liste;
		while(j<2&&lauf->next!=NULL){
			i+=lauf->element;
			lauf=lauf->next;
			j++;
		}
	k=(struct knoten*) calloc(1,sizeof(struct knoten));
	k->element=i;
	if(liste->next->next!=NULL){
		k->next=liste->next->next->next;
	}
	else{
		k->next=NULL;
	}
	liste->next->next=k;
	liste=k->next;
	}
}
			
struct knoten* clreven(struct knoten* liste)
{
	struct knoten *lauf=liste, *rueck=liste;
	while (liste!=NULL&&liste->element%2==0)
	{
		lauf=liste;
		liste=liste->next;
		rueck=liste;
		free(lauf);
	}
	if(liste==NULL)
		return NULL;
	else
		lauf=liste->next;
	while(lauf!=NULL)
	{
		if(lauf->element%2==0)
		{
			liste->next=lauf->next;
			free(lauf);
		}
		liste=lauf;
		if(liste!=NULL)
			lauf=liste->next;
	}
	return rueck;
}
	


int main (void){
	struct knoten *k, *liste1=NULL, *liste2=NULL, *lauf=NULL;
	int i;

	srand(time(NULL));
	i=rand()%10;

	while(i>0) {
		k=(struct knoten*) calloc(1,sizeof(struct knoten));
		k->element=i;
		k->next=NULL;
		if(liste1==NULL) {
			liste1=k;
			lauf=k;
		}
		else
		{
			lauf->next=k;
			lauf=k;	
		}
		i=rand()%10;
	}

	i=rand()%10;

	while(i>0) {
		k=(struct knoten*) calloc(1,sizeof(struct knoten));
		k->element=i;
		k->next=liste2;
		liste2=k;
		i=rand()%10;
	}	

	printf("\n\nListe 1:");
	ausgabe(liste1);
	printf("\n\nListe 2:");
	ausgabe(liste2);

	liste1=verkettung(liste1,liste2);
	printf("\n\nVerkettete Liste:");
	ausgabe(liste1);


/*	einfuegen(liste1);
	printf("\n\nListe mit einfuegungen:");
	ausgabe(liste1);


	liste1=loeschen(liste1);
	printf("Liste mit ausloeschungen:");
	ausgabe(liste1);*/

	addtwo(liste1);
	printf("\nListe mit summen jeweils zweier Glieder:\n");
	ausgabe(liste1);

	liste1=clreven(liste1);
	printf("\nListe ohne gerade Zahlen:\n");
	ausgabe(liste1);

	freigabe(liste1);
}

