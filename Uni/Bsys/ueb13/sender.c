#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>

int pipe_exists(char[]);



int main () {
	mode_t mode=0644; // Zugriffsmodus User darf rw, rest darf r
	char pname[10]; // Dateiname
	char zeile[200]; // Eingabe
	int rc; // Return Value
	int pd; // Pipe Deskriptor
	int i=1; // Zählveriable
	int c=0; // Nummer der Pipe
 
	printf("SENDER: Testen auf freie Pipe...\n");
 
	do {
		sprintf(pname, "pipe%d", i);
		if(!pipe_exists(pname)){ 
			c=i;  //Pipe existiert und ist frei
			i=3;
		}
		i++;
	} while(i<4);
	 
	if(c==0) {
		printf("SENDER: Keine freie Pipe gefunden!\n\t ## Abbruch ##\n");
		exit(2);
	}
  
	printf("SENDER: freie Pipe mit Nr %d gefunden !\n", c);
	printf("SENDER: Pipe wird angelegt...\n");
	rc = mkfifo (pname, mode);
	if (rc < 0) {
		perror("SENDER: FEHLER mkfifo ->");
		exit (3);
    } else {
		printf ("SENDER: Pipe \"%s\" wurde erfolgreich angelegt !\n", pname);
    }
	
	pd = open(pname, O_WRONLY);
	if(pd==-1) {
		perror("SENDER: FEHLER open ->");
		exit (4);
	}
	
	printf("SENDER(%d): Senden der Daten kann jetzt beginnen...\n", c);
	
	while(fgets(zeile, 200, stdin)) {
		if(write(pd, &zeile, sizeof(zeile))<0) {
			perror("SENDER: FEHLER write ->");
			close(pd);
			exit(5);
		}
	}
	close(pd);
  
 return 0;
} 



int pipe_exists(char pname[10]) {
	mode_t mode=0644;
	int rc;
	struct stat buf;
	rc = stat (pname, &buf);
	if(rc<0) {
		return 0;
	}
	return 1;
}
