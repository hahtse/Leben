import java.io.*;
import java.net.*;

public class srvr1 {
 final static int MAX = 100;
 final static int PORT = 5020;
 ServerSocket server = null;
 Socket conn = null;
 OutputStream snd = null;
 InputStream rcv = null;

 void ini () {
  try {
   server = new ServerSocket(PORT);
  }
  catch (Exception e) {
   System.out.println ("init: " + e);
   System.exit(1);  
  }
  try {
   conn = server.accept();
   System.out.println ("accept: " + conn);
  }
  catch (Exception e) {
   System.out.println ("accept: " + e);
   System.exit(1);  
  }
 }
 void sclose() {
  try {
   if (snd != null)
    snd.close (); 
   if (rcv != null)
    rcv.close (); 
  }
  catch (IOException e) {
   System.out.println ("Close: " + e);
   System.exit (2);
  }
}

 void echoserv () {
  String s = "";
  Integer i = new Integer (MAX-1);
  try {
   snd = conn.getOutputStream();
   rcv = conn.getInputStream();
   byte f[] = new byte [MAX];
   int l = 1;
   while (l>0) {
    l = rcv.read (f, 0, MAX);
    if (l > 0) {
     System.out.println ("Empfangen: " + l + " Bytes");
     s = new String(f ,0 , l);
     System.out.println (s);
     snd.write (f, 0, l);
     }
   }
  }
  catch (IOException ioe) {
   System.out.println ("echoserv: " + ioe);
   System.exit(1);
  }
 }
public static void main (String args[]) {
 srvr1 serv = new srvr1();
 byte f[] = new byte [MAX];
// int l = 0;
// String arg = "";
// if (args.length >0) arg = new String (args[0]);
// else { 
//  try {
//        System.out.print ("Server: ");
//        l = System.in.read (f, 0, MAX);
//        if (l < 0) System.exit(0);
//        f[l-2] = 0;
//        System.out.println (f[l-2]);
//        System.out.println (f[l-1]);
//        System.out.println (f[l]);
//        arg = new String (f ,0 , l-2);
//  }
//  catch (IOException ioe) {
//   System.out.println ("Eingabe: " + ioe);
//  }
// }
// System.out.println ("Echo mit Server: " + arg);
 serv.ini();
 serv.echoserv ();
 serv.sclose ();
}
}

