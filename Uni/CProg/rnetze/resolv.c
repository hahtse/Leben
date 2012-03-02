#include <stdio.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/nameser.h>
#include <resolv.h>
#include <signal.h>

#define DATALEN 512

extern struct state _res;

void tfunc ()
{
}

main ()
{
 int i, j, hl, rc, qc, ac, p, a;
 int op, class, type, datalen, buflen, msgl;
 u_short qtype, qclass, rtype, rclass, l, l2;
 u_long rttl;
 u_char buf[100], *cp;
 char dn[100], *dname, *z, ping[50];
 struct rrec newrr, *rec;
 union {
	char cdat[4];
	unsigned long ldat;
	struct in_addr adat;
 } dat;
 union {
	u_char data[DATALEN];
	HEADER head;
 } data;

  hl = sizeof (HEADER);
  signal (SIGALRM, tfunc);
  res_init();
  printf ("Inhalt von resolv.conf\n");
  printf ("domain: %s\n", _res.defdname);
 /* printf ("MAXDNSRCH = %d\n", MAXDNSRCH);
  for (i=0; i<MAXDNSRCH; i++)
	printf ("%s\n", _res.dnsrch[i]);
*/
  printf ("%d Name-Server\n", MAXNS);
  for (i=0; i<MAXNS; i++)
  	printf ("%s\n", inet_ntoa(_res.nsaddr_list[i].sin_addr));
/*  printf ("number of addresses: %d \n", _res.ascount);
  printf ("number of name servers: %d \n", _res.nscount);
*/
  /*printf ("%d %s\n", _res.ascount, inet_ntoa(_res.sort_list[0]));*/
 
/*  strcpy (dn, "zeus.rz.htwk-leipzig.de");  */
/*  strcpy (dn, "caramba.cs.tu-berlin.de"); */ 
/*  strcpy (dn, "ftp.cs.tu-berlin.de");  */
/*  strcpy (dn, "moses.imn.htwk-leipzig.de"); */
/*  strcpy (dn, "moses"); */

 do {
  printf ("\nSuche nach IP-Adresse von\n");
  printf ("Hostname: ");
  z = gets (dn); 
  printf ("\n");
 if (z) {
  dname = dn;
  strcpy (data.data, dn);
  datalen = strlen(dn)+1;
  op = QUERY;
  buflen = sizeof(buf);
/*  _res.options |= RES_DEBUG; */
  _res.retrans = 10;
  _res.retry = 2; 
  class = C_IN; 
  type = T_A; 
  rc=res_mkquery (op, dname, class, type, data.data, datalen, &newrr, buf, buflen);

  buflen = rc;
  alarm (10);
  msgl=res_send (buf, buflen, data.data, DATALEN);
  if (msgl>0) {

/*  printf ("\nsend: %d\n", msgl); */
/* for (i=0; i< msgl; i++) {
	for (j=0; j<4; j++) dat.cdat[j] = data.data[i+j];
*/
/*	printf ("%x ", dat.ldat); 
	if (dat.ldat==x) printf ("\n%d %x\n", i, dat.ldat); 
  }
  printf ("\n");
*/
/*  for (i=0; i< msgl;i++) {
	j = data.data[i] & 0xff; printf ("%2x ", j); 
  } 
  printf ("\n");
  for (i=0; i< msgl;i++) if (data.data[i]>31) printf ("%c ", data.data[i]);
  printf ("\n");
*/
/*  printf ("%x \n", x); 
*/
  ac = data.head.ancount;
  qc = data.head.qdcount;
  p = hl;
  printf ("Query\n");
  while (qc) {
    l = data.data[p];
    while (l > 0 && l < 64) {
/*	printf ("%d \n", p); */
	for (i=1; i<=l; i++) putchar (data.data[p+i]);	       	
	p = p + l + 1;
	l = data.data[p];
	if (l) putchar ('.');
    } 
    putchar ('\n');
    p++;
    cp = data.data+p; /* GETSHORT verlangt Pointervariable !! */
/* einfache Zuweisung (auch mit cast) ergibt falsche Werte
   evtl. Probleme mit integralen Grenzen ?? */
    GETSHORT (qtype, cp); 
/* oder: qtype = _getshort(data.data+p);*/
    p = p + sizeof(u_short);
    cp = data.data+p;
    GETSHORT (qclass, cp); 
    p = p + sizeof(u_short);
    cp = data.data+p;
    printf ("QTYPE=%d, QCLASS=%d", qtype, qclass);
    qc--;
    putchar ('\n');
  }
  printf ("\n%d Answers\n", ac);
  a=1;
  while (ac) {
    printf ("%d.\n", a);
    l = data.data[p];
    while (l > 0 && l < 64) {
/*	printf ("%d \n", p); */
	for (i=1; i<=l; i++) putchar (data.data[p+i]);	       	
	p = p + l + 1;
	l = data.data[p];
	if (l) putchar ('.');
	else putchar ('\n');
    }
    if (l>63) {
	rc=dn_expand (data.data, data.data+msgl, cp, buf, buflen);
	printf ("%s\n", buf);
	p += 2;
    }  else p++;
    cp = data.data+p;
    GETSHORT (rtype, cp); 
    p = p + sizeof(u_short);
    cp = data.data+p;
    GETSHORT (rclass, cp); 
    p = p + sizeof(u_short);
    cp = data.data+p;
    GETLONG (rttl, cp);
    p = p + sizeof(u_long);
    printf ("RRTYPE=%d, RRCLASS=%d RRTTL=%d\n", rtype, rclass, rttl);
    cp = data.data+p;
    GETSHORT (l, cp);
    p = p + sizeof(u_short);
    if (rtype == 1) {
    	for (j = 0; j< l; j++) dat.cdat[j] = data.data[p+j];
/*	printf ("IP-Adresse: %x \n", dat.ldat); */
    	printf ("IP-Adresse: %s \n", inet_ntoa (dat.adat));
    	sprintf (buf, "%s", inet_ntoa (dat.adat));
	strcpy (ping, "ping ");
	strcat (ping, buf);
	strcat (ping, " 10");
	printf ("%s\n", ping);
	system (ping);
	p += l;
    } 
    if (rtype == 5) {
	l2 = l;
	l = data.data[p];
	while (l > 0 && l < 64) {
/*		printf ("%d \n", p); */
		for (i=1; i<=l; i++) putchar (data.data[p+i]);	       	
		p = p + l + 1;
		l = data.data[p];
		if (l) putchar ('.');
		else putchar('\n');
    	}
    	if (l>63) {
		rc=dn_expand (data.data, data.data+msgl, cp, buf, buflen);
		printf ("%s\n", buf);
		p += 2;
    	}  else p++;
	l = l2;
    }
    putchar ('\n');
    ac--; a++;
    }
   } else printf ("not found\n");
 } } while (z);
}
