#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <errno.h>
#include <fcntl.h>

int main(int argc, char *argv[]) {
  mode_t mode;
  int rc, ewert = 0, pno = 1, erfolg = 0, fd;
  struct stat buf;
  char pname[100], eab[100];

  mode = 0644;
  if (argv[1] != NULL) sscanf(argv[1], "%d", &pno);
  
  do {
  	sprintf(pname, "Pipe_%d", pno);
	printf("PNAME: %s\n", pname);
  	rc = stat(pname, &buf);
  	if(rc < 0) {						// Testen ob Datei existiert
		if(errno != ENOENT) {				// Fehler bei stat...nicht Datei nicht gefunden!
			printf("Unbekannter Fehler!\n");
			perror("stat");
			exit(1);
		}
		rc = mkfifo(pname, mode);			// falls nicht: Pipe erzeugen
		if (rc < 0) {					// falls nicht erfolgreich: Fehler
        	        printf("Fehler beim Erzeugen der Pipe!\n");
			perror("mkfifo");
			exit(2);
		}
		printf("%s erzeugt!\n", pname);
		erfolg = 1;
  	} else {						// falls doch: Testen ob Pipe
		if (!S_ISFIFO(buf.st_mode))			// Datei ist keine Pipe
			printf("%s keine Pipe!\n", pname);
		else 	printf(" %s existiert bereits.\n", pname);	// Datei ist eine Pipe
		pno++;
  	}
  } while (erfolg != 1 && pno < 4);
  
  fd = open(pname, O_WRONLY);
  do {
        printf("Ihre Eingabe: \n");
        if (fgets(eab, sizeof(eab), stdin) == NULL) break;
       	rc = write(fd, eab, sizeof(eab));
        if (rc > 0) printf("Erfolgreich gesendet!\n");
        else perror("Fehler bei der Übertragung!");
  } while (1);
  close(fd);
  exit(0);
}
