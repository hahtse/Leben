#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>


#define CONTROLLEN  CMSG_LEN(sizeof(int))
static struct cmsghdr *cmptr = NULL;
int recv_fd(int fd);

int main(void) {

	int fd, sockd, rc, *p;
	struct sockaddr_un server;
	struct msghdr mh;
	struct cmsghdr *cmp;
	union {
		struct cmsghdr cm;
		char ctrl[sizeof(struct cmsghdr) + sizeof(int)];
	} ctrlu;
	struct iovec iov[1];
	char eab[100], fname[100];

	mh.msg_name = NULL;
	mh.msg_namelen = 0;
	mh.msg_iov = iov;
	mh.msg_iovlen = 1;
	mh.msg_control = NULL;
	mh.msg_controllen = 0;
	mh.msg_flags = 0;
	iov[0].iov_base = fname;
	iov[0].iov_len = strlen(fname) + 1;

	server.sun_family = AF_UNIX;
	strcpy(server.sun_path, "sockunix");
	sockd = socket(AF_UNIX, SOCK_STREAM, 0);				//socket anlegen
	rc = connect(sockd, (struct sockaddr*) &server, sizeof(server));	//verbinden
	printf("geben sie den Filenamen ein:\t");
	fgets(eab,sizeof(eab),stdin);						//filenamen einlesen
	eab[strlen(eab)-1]='\0';						//zeichenkettenende anhängen
	iov[0].iov_base = eab;
	iov[0].iov_len = sizeof(eab) + 1;
	rc = sendmsg(sockd, &mh, 0);						//anfrage senden
	if (rc < 0) {
		perror("sendmsg client");
		exit(-1);
	}
	fd=recv_fd(sockd);							//auf antwort warten mittels methode recv_fd warten
	if (fd < 0) {								
		printf("file ex. nicht\n");
		exit(-1);
	}
	while (read(fd, eab, 1) > 0) {						//fileinhalt ausgeben
		printf("%c", eab[0]);
	}
	close(sockd);								//socket schließen

	return 0;
}

int recv_fd(int fd) {								
	int newfd, nr, status;
	//char *ptr;
	char buf[2];
	struct iovec iov[1];
	struct msghdr msg;

	status = -1;
	
	iov[0].iov_base = buf;
	iov[0].iov_len = sizeof(buf);
	msg.msg_iov = iov;
	msg.msg_iovlen = 1;
	msg.msg_name = NULL;
	msg.msg_namelen = 0;
	if (cmptr == NULL && (cmptr = malloc(CONTROLLEN)) == NULL){
		return (-1);
	}
	msg.msg_control = cmptr;
	msg.msg_controllen = CONTROLLEN;
	if ((nr = recvmsg(fd, &msg, 0)) < 0) {				//auf empfang der nachricht warten
		perror("recvmsg error");
		return (-1);
	} 
	if (nr == 0){
		perror("connection closed by server");
		return (-1);
	}
		
        if (msg.msg_controllen ==0){					//leere Nachricht, kein Filedeskriptor -> File nicht vorhanden
        	return -1;		
        }
	

	return *(int *) CMSG_DATA(cmptr);				//ansonsten gib filedeskriptor weiter
}
