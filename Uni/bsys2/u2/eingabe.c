#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/stat.h>
#include<stropts.h>
#include<unistd.h>


struct strbuf ctl,data;
/* Komponenten: int maxlen; int len; char *buf; */

int main(){
  	                                               
  	int mypid;                                                  
	int rc, flags;
  	char name[]="50441";                                  
  	char cbuf[100], dbuf[100];                                  
  	FILE *fp;                                                  
  	int fd;   
  	                                                 
/*lege Pipe an oder oeffne bestehende Pipe */                            
	rc=mkfifo(name,0644);                                       
	if (rc<0){
		printf("Pipe existiert schon!\n");
  	}else{
    		printf("Benannte Pipe %s erfolgreich angelegt!\n", name);
	}
  	fp=fopen(name,"w");                                          
  	if(fp==0){                                 
  		perror("Fehler beim Zugriff auf Pipe!");
    		exit(-1);
  	}
   	fd=fileno(fp);                                          
/*Anmeldung senden*/
	mypid=getpid();
	mypid=-1*mypid; 
 	flags=0;                                                    
  	data.len=-1;                                                
  	data.buf=dbuf;
  	sprintf(cbuf,"%d",mypid);;//Status "neuer Sendeprozess"
  	ctl.len=strlen(cbuf)+1;
  	ctl.buf=cbuf;                     
  	rc=putmsg(fd, &ctl, &data, flags);
  	if(rc<0){                                              
    		perror("Fehler beim Senden der Anmeldeachricht: putmsg");
  	}else{
    		printf("Anmeldung!\n");
  	}
  	
/*Nachrichten senden*/	
  	mypid=getpid();                                              
 	sprintf(cbuf,"%d",mypid);                                   
  	
	printf("\nEingabe:\n");
 	while((fgets(dbuf, sizeof(dbuf), stdin))!=NULL){             
     	ctl.len=strlen(cbuf)+1;                                    
    		ctl.buf=cbuf;                                              
    		data.len=strlen(dbuf)+1;                                   
    		flags=0;
    		data.buf=dbuf;                                             
    		rc=putmsg(fd, &ctl, &data, flags);                         
    		if(rc<0){                                                   
      		perror("Fehler beim Senden der Nachricht: putmsg");
    		}
    		printf("\nEingabe:\n");
  	}

  	sprintf(dbuf,"%d",mypid);                                    /* Prozess-ID in lokalen Daten-Buffer kopieren */
  	flags=0;
  	data.len=strlen(dbuf)+1;                                     /* Nachrichtenlaenge ermitteln */
  	data.buf=dbuf;                                               /* lokalen Daten-Buffer in Daten-Sende-Buffer */
  	sprintf(cbuf,"%d",0);                                        /* Status "Sendeprozess abgeschlossen" */
  	ctl.len=1;
  	ctl.buf=cbuf;
  	printf("buf:%s\n",ctl.buf);
  	rc=putmsg(fd, &ctl, &data, flags);                           /* "Abmeldenachricht" senden */
  	if(rc<0){                                                     /* Fehlerausgabe */
    		perror("Fehler beim Senden der Abmeldenachricht: msgsnd");
  	}else{
    		printf("Abmeldung!\n");  
	}
}
