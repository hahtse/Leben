#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(void){
	
	double d1f, d2f, exph, h, fexakt,x1,x2;
	float x0 = 0.5;
	double V[41],R[41];
	int k;
	long p;
	double q= 0.43212345;

	//A1.a)
	fexakt = exp(x0);
	printf("\n\nNumerik, 1. Beleg\nA1.a)\n\nf'(%.1f) = %.10f", x0, fexakt);
	printf("\n10^exph\t\td1f\t\tabs(d1f-f'(x0))\td2f\t\tabs(d2f-f'(x0))");
	for (exph = -1;exph>=-11;exph-=0.5){
		h = pow(10,exph);
		d1f = ((1/h)*(exp(x0+h)-exp(x0)));
		d2f = ((1/(2*h))*(exp(x0+h)-exp(x0-h)));
		
		printf("\n%.1f\t\t%.10f\t%.10f\t%.10f\t%.10f",exph,d1f,fabs(d1f-fexakt),d2f,fabs(d2f-fexakt));
	}
	
	//A1.b)
	fexakt = -(2*x0/(pow(pow(x0,2)-0.0169,2)));
	printf("\n\nA1.b)\n\nf'(%.1f) = %.10f", x0, fexakt);
	printf("\n10^exph\t\td1f\t\tabs(d1f-f'(x0))\td2f\t\tabs(d2f-f'(x0))");
	for (exph = -1;exph>=-11;exph-=0.5){
		h = pow(10,exph);
		d1f = ((1/h)*(1/(pow(x0+h,2)-0.0169)-1/(pow(x0,2)-0.0169)));
		d2f = ((1/(2*h))*((1/(pow((x0+h),2)-0.0169))-1/(pow(x0-h,2)-0.0169)));
		printf("\n%.1f\t\t%.10f\t%.10f\t%.10f\t%.10f",exph,d1f,fabs(d1f-fexakt),d2f,fabs(d2f-fexakt));
	}
	
	V[0]=(exp(1)-1)/exp(1);
	for(k=1;k<=40;k++){
		V[k]=1-k*V[k-1];
	}

	R[40]=0.7/41;
	for(k=40;k>=1;k--){
		R[k-1]=(1-R[k])/k;
	}

	printf("\n\nA2.c)\n\nk\t(V): I(k)\t\t(R): I(k)");
	for(k=0;k<=40;k++){
		printf("\n%d\t%.10f\t\t%.10f",k, V[k], R[k]);
	}

	printf("\n\nA2.c) - gestoerter Startwert");
	V[0]=(exp(1)-1)/exp(1) + pow(10,-6);
	for(k=1;k<=40;k++){
		V[k]=1-k*V[k-1];
	}

	R[40]=0.7/41 + pow(10,-1);
	for(k=40;k>=1;k--){
		R[k-1]=(1-R[k])/k;
	}

	for(k=0;k<=40;k++){
		printf("\n%d\t%.10f\t\t%.10f",k, V[k], R[k]);
	}

	printf("\n\nA3.a)");
	printf("\n\np=10^\tx1\t\tx2");
	for(k=0;k<=12;k++){
		p = pow(10,k);
		x1 = -p-sqrt(pow(p,2)+q);
		x2 = -p+sqrt(pow(p,2)+q);
		printf("\n%d\t%.10f\t%.10f",k,x1,x2);
	}

	printf("\n\nA3.b)");
	printf("\n\np=10^\tx1\t\tx2");
	for(k=0;k<=12;k++){
		p = pow(10,k);
		x1 = -p-sqrt(pow(p,2)+q);
		x2 = -q/x1;
		printf("\n%d\t%#.10f\t%.10f",k,x1,x2);
	}

	printf("\n");
return 0;
}

