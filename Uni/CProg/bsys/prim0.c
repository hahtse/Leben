/* Berechnung von Primzahlen nach Prof. Jahn */
#include <stdio.h>
#include <math.h>

short isPrime(long p);

int main(){
  long i, n, anz=0;
  printf ("Bis zu welcher Zahl sollen Primzahlen ausgegeben werden? ");
  scanf ("%ld", &n);
  for ( i = 2; i <= n; i++ )
    if ( isPrime(i) ) {
        anz++;
        printf ("%ld ", i);
    }
  printf ("\n%ld Primzahlen\n", anz);
  return 0;
}

short isPrime(long p){
  long s, d;
  s = (long) sqrt(p) + 1;
  if ( p <  2 ) return 0;
  if ( p == 2 ) return 1;
  if ( p % 2 == 0 ) return 0;
  for ( d = 3; d <= s; d+=2 )
    if ( p % d == 0 ) return 0;
  return 1;
}

