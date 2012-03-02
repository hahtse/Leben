//Hans-Christian Heinz
//09INB1
//50425

#include <stdio.h>
#include <stdlib.h>

struct knoten
{
	int element1;
	int element2;
	struct knoten *next;
};

struct adjazenz
{
	int element;
	struct adjazenz *next;
};

int adjazenzmatrix(int**, int);
int verketteteliste(struct knoten*, int);
int nachfolgeliste(struct adjazenz**, int);
int** RzuS(int**, int);
struct knoten* MatZuLi(int**, int);
struct adjazenz** MatZuAr(int**, int);
void schreibeAdjazenzmatrix(int** , int);
void schreibeVerketteteliste(struct knoten*);
void schreibeNachfolgeliste(struct adjazenz**, int);
void schreibeMenge(struct knoten*);

int main(void){
	
	int n=3, i, j;	//n=3 zu Testzwecken, i und j Laufvariablen für Schleifen
	int a=0, b=0;	//Variablen bei der Werteingabe
	int flag=1;		//Trigger fuer Beendigung der Werteingabe
	
	int **Rmat, **Smat;				//Relationen R und S in Adjazenzmatrix-Form
	struct knoten *Rli, *Sli;		//R und S als verkettete Listen
	struct adjazenz **Rar, **Sar;	//R und S als kombiniertes Array/Listen-Ding.

	printf("\n\nWillkommen zum Programm \"Projektaufgabe 2 - Speicherung von binaeren Relationen\"\n\n");
	printf("\nBitte geben sie eine Zahl n ein, die die Anzahl der Elemente der Relation angibt: ");
	scanf("%d", &n);
	printf("\n\nDanke!\n\n Bitte geben sie nun Relationspaare mit Elementen von 0 bis %d ein.", n-1);
	printf("\nDie Eingabe wird abgebrochen, wenn ein negatives Element eingegeben wird.\n\n");


	//Rmat wird initialisiert und erstmal komplett mit 0 belegt
	Rmat = (int**) calloc(n,sizeof(int*));

	for(i=0; i<n; i++){
		Rmat[i] = (int*) calloc (n, sizeof(int));
	}

	for(i=0;i<n;i++){
		for(j=0;j<n;j++){
			Rmat[i][j]=0;
		}
	}
	
	//Smat wird nur initialisiert
	Smat = (int**) calloc(n,sizeof(int*));

	for(i=0; i<n; i++){
		Smat[i] = (int*) calloc (n, sizeof(int));
	}

	//die Werte werden nacheinander eingelesen, die Eingabe stoppt, sobald einen negative Zahl auftaucht
	//moegliche Fehlerquelle: wenn z.B. .1 oder sowas eingeben wird, habs hier nicht ausgeschlossen, weil nicht gefordert
	while(flag==1){
		do{
			printf("\nBitte erstes Element von eingeben:");
			scanf("%d",&a);
		}while(a>=n);

		do{
			printf("\nBitte zweites Element eingeben:");
			scanf("%d",&b);
		}while(b>=n);

		if(a<0||b<0)
			flag=0;
		else
		{
			Rmat[a][b]=1;
		}
	}

	/*//Kleines Testarray
	for(i=0;i<n;i++){
		Rmat[0][i]=1;
		Rmat[2][i]=1;
	}
	Rmat[1][1]=1;*/

	//die restlichen Funktionen sind alle ausgelagert
	schreibeAdjazenzmatrix(Rmat, n);
	
	Rli=MatZuLi(Rmat,n);

	schreibeVerketteteliste(Rli);

	Rar=MatZuAr(Rmat, n);

	schreibeNachfolgeliste(Rar, n);

	Smat=RzuS(Rmat, n);
	
	schreibeAdjazenzmatrix(Smat, n);

	Sli=MatZuLi(Smat,n);

	Sar=MatZuAr(Smat,n);

	schreibeVerketteteliste(Sli);
	
	schreibeNachfolgeliste(Sar,n);

	printf("\n\nR=");
	schreibeMenge(Rli);
	printf("\n\nS=");
	schreibeMenge(Sli);
	
	printf("\n\nPress any Key to exit\n");
	scanf("%d");
	return 0;
}

//Diese Funktion ueberprueft, ob R in Adjazenzmatrixform eine Aequivalente Relation ist. Wenn ja, wird 1 zurueckgegeben
int adjazenzmatrix(int** Rmat, int n){
	
	int flag=1;
	int i, j, k;

	//ueberprueft auf Reflexivitaet..
	for(i=0;i<n;i++){
		if(Rmat[i][i]==0)
			flag=0;
	}

	//...auf Symmetrie...
	if(flag==1){
		for(i=0;i<n;i++){
			for(j=0;j<n;j++){
				if(Rmat[i][j]!=Rmat[j][i])
					flag=0;
			}
		}
	}

	//...und auf Transitivität. Letzteres Mittels der Umwandlung von x0->x1 in |x0 v x1
	if(flag==1){
		for(i=0;i<n;i++){
			for(j=0;j<n;j++){
				for(k=0;k<n;k++){
					if(!(!(Rmat[i][j]==1&&Rmat[j][k]==1))||Rmat[i][k]!=1)
						flag=0;
				}
			}
		}
	}

	return flag;
}

//Tut das gleiche mit der verketteten Liste

/*Ich muss zugeben, hier (und bei der nachfolgeliste) war ich faul.
Es ist wesentlich aufwaendiger die Aequivalenzkriterien in der gegebenen Form zu pruefen,
(vor allem die Transitivitaet!) als die ganze Sache einfach umzuformen und
die schon bestehende (und sehr schnelle) Ueberpruefung der Adjazenzmatrixform zu benutzen*/
int verketteteliste(struct knoten* Rli, int n){

	int i,j;
	struct knoten *lauf;
	int **matrix;

	lauf=Rli;
	matrix = (int**) calloc(n,sizeof(int));
	for(i=0;i<n;i++)
		matrix[i] = (int*) calloc(n,sizeof(int));

	for(i=0;i<n;i++){
		for(j=0;j<n;j++){
			matrix[i][j]=0;
		}
	}

	while(lauf!=NULL){
		matrix[lauf->element1][lauf->element2]=1;
		lauf=lauf->next;
	}
	return adjazenzmatrix(matrix,n);
}


//Ueberprueft die Nachfolgeliste
int nachfolgeliste(struct adjazenz** Rar, int n){

	struct adjazenz *lauf;
	int i,j, **matrix;

	matrix = (int**) calloc(n,sizeof(int));
	for(i=0;i<n;i++)
		matrix[i] = (int*) calloc(n,sizeof(int));

	for(i=0;i<n;i++){
		for(j=0;j<n;j++){
			matrix[i][j]=0;
		}
	}

	for(i=0;i<n;i++){
		lauf=Rar[i];
		while(lauf!=NULL){
			matrix[i][lauf->element]=1;
		}
	}
	return adjazenzmatrix(matrix, n);
}


//wandelt eine gegebene Relation R in Adjazenzform in eine Aequivalenzrelation S in Adjazenzform um
int** RzuS(int** Rmat, int n){

	int i,j,k;
	int **Smat;

	//Wenn R schon aequivalent ist, dann muss man es natuerlich nicht noch umformen...
	if(adjazenzmatrix(Rmat, n)==1)
		return Rmat;


	Smat = (int**) calloc(n,sizeof(int));
	for(i=0;i<n;i++)
		Smat[i] = (int*) calloc(n,sizeof(int));

	for(i=0;i<n;i++){
		for(j=0;j<n;j++){
			Smat[i][j]=Rmat[i][j];
		}
	}

	//Reflexiv machen
	for(i=0;i<n;i++){
		Smat[i][i]=1;
	}

	//schreibeAdjazenzmatrix(Smat, n);

	//Symmetrisch machen
	for(i=0;i<n;i++){
		for(j=0;j<n;j++){
			if(Smat[i][j]==1)
				Smat[j][i]=1;
		}
	}

	//schreibeAdjazenzmatrix(Smat, n);

	//Transitiv machen
	for(i=0;i<n;i++){
		for(j=0;j<n;j++){
			for(k=0;k<n;k++){
				if(Smat[i][j]==1&&Smat[j][k]==1)
						Smat[i][k]=1;
			}
		}
	}
	//schreibeAdjazenzmatrix(Smat, n);

	return Smat;
}

//Wandelt eine bel. Adjazenzmatrix in eine einfach verkettete Liste um
struct knoten* MatZuLi(int** Rmat,int n){

	struct knoten *k, *Rli=NULL, *lauf;
	int i,j;

	for(i=0;i<n;i++){
		for(j=0;j<n;j++){
			if(Rmat[i][j]==1){
				k = (struct knoten *) calloc(1, sizeof(struct knoten));
				k->element1=i;
				k->element2=j;
				k->next=NULL;
				if(Rli==NULL){
					Rli=k;
					lauf=k;
				}
				else{
					lauf->next=k;
					lauf=k;
				}
			}
		}
	}

	return Rli;
}


//wandelt eine bel. Adjazenzmatrix in Nachfolgeliste (Array/Liste-Hybrid) um.
struct adjazenz** MatZuAr(int** Rmat,int n){
	
	struct adjazenz *p, *q; //q ist aktueller Knoten, p ist Laufvariable
	struct adjazenz **Rar;
	int i,j;

	Rar = (struct adjazenz **) calloc(n, sizeof(struct adjazenz *));

	for(i=0;i<n;i++){
		for(j=0;j<n;j++){
			if(Rmat[i][j]==1){
				p = (struct adjazenz*) calloc(1, sizeof(struct knoten));
				p->element = j;
				p->next = NULL;
				if(Rar[i]==NULL){
					Rar[i] = p;
					q = p;
				}
				else{
					q->next = p;
					q = p;
				}
			}
		}
	}

	return Rar;
}

//Ausgabefunktionen
void schreibeAdjazenzmatrix(int** Rmat, int n){

	int i,j;

	printf("\n\nAdjazenzmatrix:\n");

	for(i=0;i<n;i++){
		for(j=0;j<n;j++){
			printf("%d\t",Rmat[i][j]);
		}
		printf("\n");
	}
}

void schreibeVerketteteliste(struct knoten* Rli){

	struct knoten *k;

	printf("\n\nVerkettete Liste:\n\n");

	k=Rli;
	while(k!=NULL){
		printf("|%d|%d|->", k->element1, k->element2);
		k=k->next;
	}
	printf("||");
}

void schreibeNachfolgeliste(struct adjazenz** Rar, int n){

	int i;
	struct adjazenz *q;

	printf("\n\nNachfolgerliste:\n");

	for(i=0;i<n;i++){
		q=Rar[i];
		printf("\n%d\t", i);
		while(q!=NULL){
			printf("%d\t",q->element);
			q=q->next;
		}
	}
}
void schreibeMenge(struct knoten* Liste){

	struct knoten *k;

	k=Liste;
	printf("{");
	if(k!=NULL){
		printf("(%d,%d)",k->element1, k->element2);
		k=k->next;
	}
	while(k!=NULL){
		printf(",(%d,%d)", k->element1, k->element2);
		k=k->next;
	}
	printf("}");
}

