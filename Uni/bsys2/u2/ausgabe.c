#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/stat.h>
#include<stropts.h>
#include<unistd.h>
#include<signal.h>
#include<sys/types.h>

int beenden=0;
struct strbuf ctl,data;
/* Komponenten: int maxlen; int len; char *buf; */

void signalBehandlung(int i){	
	signal(2, SIG_IGN); 
    	beenden=1;
}


int main(){
  	int rc, flags=0, sw=1, sender=0, pid;
  	char name[]="50441", fname[6];                               
  	char cbuf[100], dbuf[100];                                   
  	FILE *fp, *fo;                                               
  	int fd;  
  	   
//Signalbehandlung anmelden
   	signal(2, signalBehandlung);
   	
//lege Pipe an oder oeffne bestehende Pipe 
     rc=mkfifo(name,0644);                                        
 	if (rc<0){
    		printf("Pipe existiert bereits!\n");
  	}
  	else{
    		printf("Benannte Pipe %s erfolgreich angelegt!\n", name);
	}
  	fp=fopen(name,"r");// Pipe lesend oeffnen 
  	if(fp==0){                                                  
    		perror("Fehler beim Zugriff auf Pipe");
    		exit(-1);
  	}
  	fd=fileno(fp);// Dateinummer ermitteln
 	
//maximale Daten/Kontrollinfo -Laenge setzen 
 	ctl.maxlen=sizeof(cbuf);                                     
  	ctl.buf=cbuf;
  	data.maxlen=sizeof(dbuf);                                  
  	data.buf=dbuf;
  
  	do{
  	// Nachrichtenempfang 
    		rc=getmsg(fd, &ctl, &data, &flags);  
    		if(rc<0){
    		}else{               
    		sw=atoi(ctl.buf);                                         
    		
    		if(sw<0){//Sender meldet sich an
    			if(beenden==0){                                           
        			sender++;                                       
        			printf("Neuer Sender!\n");
        		}else{
        			
        			kill(sw*-1, SIGKILL);
        			printf("Sender abgelehnt und beendet!\n");
        		}
    		
    		}else{
    			if(sw==0){//Sender meldet sich ab 
    				
    				sender--;                                              
        			sprintf(fname, "%s", data.buf);                        
        			fo=fopen(fname,"r");      
        		       
        			
        			if(fo==NULL){
        			
        			}else{
        				printf("\nVon Sender %s\n", fname);
     			  	while(fread(&dbuf, sizeof(dbuf), 1, fo)>0){
          				printf("%s",dbuf);    
          			}    
          		                
        				fclose(fo);          
        				remove(fname);
        			} 
    			}else{
    				sprintf(fname, "%s", ctl.buf);  
        			fo=fopen(fname,"a");  
        			sprintf(dbuf, "%s", data.buf);
        			fwrite(dbuf, sizeof(dbuf), 1, fo); 
        			fclose(fo); 
    			}
    		}
    		sprintf(cbuf,"%d",0);
    		ctl.buf=cbuf;
  		}
  	}while(sender>0);                                            // empfange bis kein Sender mehr vorhanden 
  	fclose(fp);
  	unlink(name);                                               // schliesse und entferne Pipe 
}
