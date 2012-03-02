/* tmpfname.c
 * temporaeren Filename erzeugen
 * mod. 06.04.2009
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main() {
 char tmpn1[50], tmpn2[50], *dir="/tmp", *prfx="tmpn", *p;
 int fd;
 tmpnam (tmpn1);
 printf ("tmpnam: %s\n", tmpn1);
 p=tempnam(dir, NULL);
 printf ("tempnam (V.1): %s\n", p);
 p=tempnam(dir, prfx);
 printf ("tempnam (V.2): %s\n", p);
 strcpy (tmpn2, "fnameXXXXXX");
 p = (char *) mktemp (tmpn2);
 printf ("mktemp: %s %s\n", p, tmpn2);
 strcpy (tmpn2, "fnameXXXXXX");
 fd = mkstemp(tmpn2);
 printf ("mkstemp: %s\n", tmpn2);
 close (fd); 
 unlink (tmpn2);
 exit (0);
}
