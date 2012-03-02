#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>

int pno = 0;

int ispipe(char pname[100]) {
  int rc;
  struct stat buf;
  rc = stat(pname, &buf);
  if (rc >= 0 && S_ISFIFO(buf.st_mode)) return 1;
  else return 0;
}
void chldsig(int signr) {
	int status;
	int pid;
	pid = wait(&status);
	printf("Child Prozess mit Status |%x|%x| beendet.\n", (status>>8)&0xFF, status&0x7F);
	pno--;
}

int main(int argc, char *argv[]) {
  int rc, ewert = 0, erfolg = 0, fd[4], i, status, flags, l;
  char pname[4][100], eab[100];

  sigset(SIGCHLD, chldsig);

  printf("Warte auf Pipes...\n");
  for (i = 1; i <= 3; i++) {
	sprintf(pname[pno+1], "Pipe_%d", i);
	if (ispipe(pname[pno+1])) {
		printf("%s offen!\n", pname[pno+1]);
		pno++;
	}
  }
  for (i = 1; i <= pno; i++) {
	fd[i] = open(pname[i], O_RDONLY);
	printf("Fileno %s: %d\n", pname[i], fd[i]);
	flags = fcntl(fd[i], F_GETFL);
	if (flags < 0) {
		printf("Kann Flags nicht lesen!\n");
		perror("fcntl");
		exit(1);
	}
	flags = flags | O_NONBLOCK;
	rc = fcntl(fd[i], F_SETFL, flags);
	if (rc < 0) {
		printf("Kann Flags nicht schreiben!\n");
		perror("fcntl");
		exit(1);
	}
  }
  while (1) {
	for (i = 1; i <= pno; i++) {		
		l = read(fd[i], eab, sizeof(eab));
		if (l > 0) {
			printf("Ausgabe von %s: %s\n", pname[i], eab);
		} else if (l == 0) {
			printf("%s geschlossen.\n", pname[i]);
			close(fd[i]);
			remove(pname[i]);
			for (l = i; l < pno; l++) {
				strcpy(pname[l], pname[l+1]);
				fd[l] = fd[l+1];
			}
			pno--;
			break;
		} else {
			if (errno == EAGAIN || errno == EINTR) continue;
			else printf("Übertragungsfehler!!\n");
		}
	}
	if (pno == 0) break;
  }
  printf("Vorbei...Ich bin dann mal weg...\n");
  exit(0);
}
