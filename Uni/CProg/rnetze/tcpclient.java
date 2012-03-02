import java.io.*;
import java.net.*;

public class clnt1 {
 final static int MAX = 100;
 Socket client = null;
 OutputStream snd = null;
 InputStream rcv = null;

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

 void echoclnt (String Name) {
  String s = "";
  Integer i = new Integer (MAX-1);
  try {
   client = new Socket (Name, 5020);
   snd = client.getOutputStream();
   rcv = client.getInputStream();
   byte f[] = new byte [MAX];
   int l = 1;
   while (l>0) {
    s = i.toString ();
    System.out.print ("max. " + s + " Zeichen:");
    l = System.in.read (f, 0, MAX-1);
    if (l < 0) return;
//    f[l-1] = 0;
    System.out.println ("Senden: " + l + " Bytes");
    snd.write (f, 0, l);
    l = rcv.read (f, 0, MAX);
    System.out.println ("Empfangen: " + l + " Bytes");
    s = new String(f ,0 , l);
    System.out.println (s);
   }
  }
  catch (IOException ioe) {
   System.out.println ("echoclnt: " + ioe);
   System.exit(1);
  }
 }
public static void main (String args[]) {
 clnt1 c = new clnt1();
 byte f[] = new byte [MAX];
 int l = 0;
 String arg = "";
 if (args.length >0) arg = new String (args[0]);
 else { 
  try {
	System.out.print ("Server: ");
	l = System.in.read (f, 0, MAX);
	if (l < 0) System.exit(0);
//        f[l-2] = 0;
//        System.out.println (f[l-2]);
//        System.out.println (f[l-1]);
//        System.out.println (f[l]);
        arg = new String (f ,0 , l-2);
  }
  catch (IOException ioe) {
   System.out.println ("Eingabe: " + ioe);
  }
 }
 System.out.println ("Echo mit Server: " + arg);
 c.echoclnt (arg);
 c.sclose ();
}
}

