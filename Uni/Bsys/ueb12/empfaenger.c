#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/wait.h>

int pipe_exists(char[]);


int main () {
	mode_t mode=0644; // Zugriffsmodus
	char pname[10]; // Dateiname
	char zeile[200]; // Eingabe
	int rc; // Return Value
	int pid; // Testpid
	int status; // Endestatus
	int pids[3]; // Child Pids
	pids[0]=-1;
	pids[1]=-1;
	pids[2]=-1;
	int pipe_working[3]; // Überprüft ob die Pipe bereits bearbeitet wird
	pipe_working[0]=0;
	pipe_working[1]=0;
	pipe_working[2]=0;
	int pd; // Pipe Deskriptor
	int i=0, j; // Zählveriable
	int anz_cld=0; // Anzahl Childs
 
	do {
		sprintf(pname, "pipe%d", i+1);
		if(pipe_exists(pname) && !pipe_working[i]) { // Pipe vorhanden -> Child anlegen //pipe nicht am arbeiten
			
			pids[i]= fork();
		
			if(pids[i]<0) { // Fehler bei fork
				perror("EMPFAENGER: FEHLER fork ->");
				exit(1);
			}
			//Anfang Child Prozess
			if(pids[i]==0) { 
				printf("EMPFAENGER(%d): Neuer Empfaenger online -> pipe %d\n", getpid(), i+1);
				
				pd= open(pname, O_RDONLY);
				if(pd<0) {
					perror("EMPFAENGER: FEHLER open ->");
					exit(2);
				}
				
				while(read(pd, &zeile, 200)>0) {
					printf("EMPFAENGER(%d) - Ausgabe: %s", getpid(), zeile);
				}
				
				printf("EMPFAENGER(%d): Sender wurde beendet !\n\t ## Bye ## \n", getpid());
				close(pd);
				
				remove(pname);
				exit(0); //ende von child
			}
			
			 //Anfang Parent
			if(pids[i]>0) { 
				anz_cld++;
				pipe_working[i]=1;
			}
		}
		
		i++;
		if(i==3) i=0; //Damit i von 1 bis 3 läuft, Childs werden nacheinander abgearbeitet
		
		for(j=0; j<3; j++) {
			pid=waitpid(pids[j], &status, WNOHANG);
			//printf("Test : pid(%d) noch da ? -> %d\n", pids[j], pid);
			if(pid>0) {
			printf("Anzahl Childs: %d\nPipe: %d\n pid: %d\n", anz_cld, j, pid);
			anz_cld--; pipe_working[j]=0; pids[j]=-1; 
			}
		}
	
	} while(anz_cld>0);
 
	printf("EMPFAENGER: Alle Childs beendet, keine Sender mehr.\n\t ## Ende ##\n");

 return 0;
//ende parent
} 



int pipe_exists(char pname[10]) {
	mode_t mode=0644;
	int rc;
	struct stat buf;
	
	rc = stat (pname, &buf);
	if(rc<0) {
		return 0;//geht nicht
	}
	if(S_ISFIFO(buf.st_mode)){
		return 1; //pipe existiert
	}
	return 0;//geht nicht
	
}
