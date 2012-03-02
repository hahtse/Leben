#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <bits/socket.h>
#include <fcntl.h>


#define CONTROLLEN  CMSG_LEN(sizeof(int))
static struct cmsghdr   *cmptr = NULL;
int send_fd(int fd, int fd_to_send);


int main(void) {

	int fd, sockd, rc, *p;
	int sockd2;
	struct sockaddr_un server;
	struct msghdr mh = { 0 };
	struct cmsghdr *cmp = NULL;

	struct iovec iov[1];
	char eab[100], fname[100];

	mh.msg_name = NULL;
	mh.msg_namelen = 0;
	mh.msg_iov = iov;
	mh.msg_iovlen = 1;
	mh.msg_flags = 0;						
	server.sun_family = AF_UNIX;
	strcpy(server.sun_path, "sockunix");
	sockd = socket(AF_UNIX, SOCK_STREAM, 0);			//Socket anlegen
	unlink(server.sun_path);

	rc = bind(sockd, (struct sockaddr*) &server, sizeof(server));	//...und zuweisen
	if (rc < 0) {
		perror("bind");
		exit(-1);
	}
	rc = listen(sockd, 5);						//auf client warten
	if (rc < 0) {
		perror("listen");
		exit(-1);
	}
	sockd2 = accept(sockd, 0, 0);					//...und den client annehmen

	iov[0].iov_base = eab;						//IO-Vektor initialisieren
	iov[0].iov_len = sizeof(eab);
	rc = recvmsg(sockd2, &mh, 0);					//fileanforderung vom client empfangen
	if (rc < 0) {
		perror("recvmsg server");
		exit(-1);
	}
	strcpy(fname, eab);
	fd = open(fname, O_RDONLY);					//entsprechendes file öffnen
	if (fd < 0) {
		perror("open");
	}
	if (send_fd(sockd2, fd) < 0) {					//filedescriptor übermitteln mit methode send_fd()
		perror("send_fd");
		exit(-1);
	}
	close(sockd2);							//sockets schließen
	close(sockd);
	unlink(server.sun_path);
	return 0;
}

int send_fd(int fd, int fd_to_send) {
	struct iovec iov[1];
	struct msghdr msg;
	char buf[2]; 							// send_fd()/recv_fd() 2-byte protokoll

	iov[0].iov_base = buf;
	iov[0].iov_len = 2;
	msg.msg_iov = iov;
	msg.msg_iovlen = 1;
	msg.msg_name = NULL;
	msg.msg_namelen = 0;
	if (fd_to_send < 0) {						//wenn das file nicht vorhanden ist...
		msg.msg_control = NULL;
		msg.msg_controllen = 0;
		buf[1] = -fd_to_send; 					//nicht null --> error
		if (buf[1] == 0)
			buf[1] = 1;					//zu große negativwerte machen das protokoll kaputt (-256 z.B.)
	} else {							//wenn das file vorhanden ist...
		if (cmptr == NULL && (cmptr = malloc(CONTROLLEN)) == NULL)
			return (-1);					//wenn der messageheader nicht belegt ist -> Fehler
		cmptr->cmsg_level = SOL_SOCKET;
		cmptr->cmsg_type = SCM_RIGHTS;
		cmptr->cmsg_len = CONTROLLEN;
		msg.msg_control = cmptr;
		msg.msg_controllen = CONTROLLEN;
		*(int *) CMSG_DATA(cmptr) = fd_to_send; 		//zu sendender fd wird in sendedaten geschrieben
		buf[1] = 0; 						//wenn der status 0 ist, ist alles in ordnung
	}
	buf[0] = 0; 							//null byte flag für recv_fd()
	if (sendmsg(fd, &msg, 0) != 2)					//...und nachricht senden.
		return (-1);
	return (0);
}

