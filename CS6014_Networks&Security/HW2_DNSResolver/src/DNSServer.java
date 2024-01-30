import javax.xml.crypto.Data;
import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;

public class DNSServer {
    public static void main(String[] args) throws IOException {

        int server_port = 5678;
        System.out.println("Listening at " + server_port);

        DatagramSocket socket = new DatagramSocket(server_port);
        byte[] response = new byte[1024];
        DatagramPacket packet = new DatagramPacket(response, response.length);

        while(true){
            System.out.println("\n\nReceived: " + packet.getLength() + " bytes");
            for (int i = 0; i < packet.getLength(); i++) {
                System.out.print(String.format("%s", response[i]) + " ");
            }
            System.out.println("\n");
        }
    }
}
