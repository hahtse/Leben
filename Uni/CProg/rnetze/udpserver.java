import java.io.*;
import java.net.*;

public class srvr1 {
 final static int MAX = 100;
 final static int PORT = 5020;
 DatagramSocket server = null;

 void sclose() {
  if (server != null)
   server.close (); 
} 

 void echosrvr () {                           
  String s = "";
  byte f[] = new byte [MAX];
  int port = 0;
  DatagramPacket packet = null;
  try {
   server = new DatagramSocket (PORT);
   InetAddress myaddr = InetAddress.getLocalHost ();
//  InetAddress myaddr = server.getLocalAddress ();  
   s = myaddr.toString ();
   System.out.println ("Local-IP: " + s );
   packet = new DatagramPacket(f, MAX);
   port = server.getLocalPort ();
   System.out.println ("Local-Port: " + port );
   System.out.println ("Warten auf Client");
//   packet.setPort (PORT);
   int l = 1;
   int j = MAX-1;
//   while (l>0) {
    packet.setLength (MAX);
    server.receive (packet);
    l = packet.getLength ();
    System.out.println ("Empfangen: " + l + " Bytes");
    f = packet.getData ();
    s = new String(f ,0 , l);
    System.out.println (s);
    port = packet.getPort ();
    System.out.println ("Client-Port: " + port );
    InetAddress clntaddr = packet.getAddress ();
    s = clntaddr.toString ();
    System.out.println ("Client-IP: " + s );

    System.out.println ("Senden: " + l + " Bytes");
    packet.setLength (l);
    server.send (packet);
//   }
  }
  catch (IOException ioe) {
   System.out.println ("echoclnt: " + ioe);
   System.exit(1);
  }
 }
public static void main (String args[]) {
 srvr1 s = new srvr1();
 s.echosrvr ();
 s.sclose ();
}
}

