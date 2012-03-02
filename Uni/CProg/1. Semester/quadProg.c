//quadratische Berechnung

#include<stdio.h>
#include<math.h>

int quadr(void)
{
	float a,b,c,d,x1,x2,rt,it,eps=0.00000000000000001;
	char blah,blubb;
	printf("\n\n\t\tWillkommen zum Löser der quadratischen Gleichungen!\n\n");
	printf("\nBitte geben sie Koeffizienten für a*x²+b*x+c=0 ein!\n");
	printf("\nBitte a angeben:  ");
	scanf("%f",&a);
	printf("\nBitte b angeben: ");
	scanf("%f",&b);
	printf("\nBitte c angeben: ");
	scanf("%f",&c);

	if(fabs(a)<eps&&fabs(b)>=eps)
	{
		x1=(-c/b);
		printf("\nLineare Gleichung! x = %f\n",x1);
	}
	if(fabs(a)<eps&&fabs(b)<eps)
	{
		printf("\nNice try! Bitte versuchen sies nochmal mit Koeffizienten die tatsächlich Sinn machen...\n");
	}
	else
	{
		d=((b*b)/(4*a*a)-(c/a));
		rt=(-b/(a*2));
		if(d<0)
		{
			it=(sqrt(-d));
			printf("\n2 Imaginäre Lösungen!\n");
			printf("\nx1=%f+i*%f\n",rt,it);
			printf("\nx2=%f-i*%f\n",rt,it);
		}
		else
		{
			it=sqrt(d);
			x1=(rt+it);
			x2=(rt-it);
			printf("\n 2 reelle Lösungen!\n");
			printf("\nx1=%f\n\nx2=%f\n\n",x1,x2);
		}
	}
	
	printf("\n\nWeitere Berechnung? j/n:  ");
	blubb=getchar();
	scanf("%c",&blah);
	while(toupper(blah)!='J'||toupper(blah)!='N')
	{
		printf("\n\nUngültige Eingabe! Bite nochmal versuchen!\n\n");
		printf("\n\nWeitere Berechnung? j/n:  ");
		scanf("%c",&blah);
	}
	if(toupper(blah)=='J')
		quadr();
	//printf("%c",blah);
}

int main(void)
{
	quadr();
}
	
