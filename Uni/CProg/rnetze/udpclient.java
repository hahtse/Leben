import java.io.*;
import java.net.*;

public class clnt1 {
 final static int MAX = 100;
 final static int PORT = 5020;
 DatagramSocket client = null;

 void sclose() {
   if (client != null)
    client.close (); 
} 

 void echoclnt (String Name) {                           
  String s = "";
  byte f[] = new byte [MAX];
  int port = 0;
  DatagramPacket packet = null;
  try {
   client = new DatagramSocket ();
   port = client.getLocalPort ();
   System.out.println ("Client-Port: " + port );
   InetAddress myaddr = InetAddress.getLocalHost ();
//    InetAddress myaddr = server.getLocalAddress ();  // ?????
   s = myaddr.toString ();
   System.out.println ("Client-IP: " + s );
   InetAddress inetaddr = InetAddress.getByName (Name);
   packet = new DatagramPacket(f, MAX, inetaddr, PORT);
   int l = 1;
   int j = MAX-1;
//   while (l>0) {
    System.out.print ("max. " + j + " Zeichen:");
    l = System.in.read (f, 0, MAX-1);
    if (l < 0) return;
    System.out.println ("Senden: " + l + " Bytes");
    packet.setLength (l);
    client.send (packet);
    packet.setLength (MAX);
    client.receive (packet);
    port = packet.getPort ();
    InetAddress servaddr = packet.getAddress ();
    s = servaddr.toString ();
    System.out.println ("Server-Port: " + port );
    System.out.println ("Server-IP: " + s );
    l = packet.getLength ();
    System.out.println ("Empfangen: " + l + " Bytes");
    f = packet.getData ();
    s = new String(f ,0 , l);
    System.out.println (s);
//   }
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

