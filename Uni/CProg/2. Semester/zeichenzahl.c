//C-Programmierung II 1.Uebung

#include <stdio.h>
#include <ctype.h>

int main(void){

	int zeilen=0,zeichen=0,zeichenzeile=0,worte=0,alphanum=0,nichtalphanum=0,erstezahl,flagspace,flagziffer=0;
	char blubb;
	printf("Willkommen zur Zeichenkettenauswertung!");


	while((blubb=getchar())!=EOF){
		zeichen++;
		if(isalnum(blubb)){
			alphanum++;
			flagspace=0;
			if(isdigit(blubb)&&flagziffer==0){
				flagziffer=1;
				erstezahl++;
			}	
		}
		else{
			nichtalphanum++;
			if(isspace(blubb)&&flagspace==0){
				worte++;
				flagspace=1;
			}
			else if(!isspace(blubb))
				flagspace=0;

		}
		zeichenzeile++;
		if(flagziffer==0)
			erstezahl++;
		if(blubb=='\n'){
			zeilen++;
			printf("\n\n%d",zeichenzeile);
			if(flagziffer!=0)
				printf("\t%d\t",erstezahl);
			else
				printf("\t\tKeine Zahl in der Zeile\t");
			zeichenzeile=0;
			erstezahl=0;
			flagziffer=0;
		}

	}

	printf("\n\n");
	printf("Zeichen: %d\tWorte: %d\tZeilen: %d",zeichen,worte,zeilen);
	printf("\nAlphanumerische: %d\tNichtalphanum: %d\n\n",alphanum,nichtalphanum);
	return 0;
}
