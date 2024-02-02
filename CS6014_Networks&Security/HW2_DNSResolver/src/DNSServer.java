import javax.xml.crypto.Data;
import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.util.ArrayList;

public class DNSServer {
    public static void main(String[] args) throws IOException {

        int server_port = 8053;
        System.out.println("Listening at " + server_port);

        DatagramSocket socket = new DatagramSocket(server_port);


        for (int count = 1; true; count++) {
            byte[] buffer = new byte[1024];
            DatagramPacket pkt = new DatagramPacket(buffer, buffer.length);
            socket.receive(pkt);
            DNSMessage requestMessage = DNSMessage.decodeMessage(pkt.getData());

            DNSHeader header = requestMessage.getHeader_();
            String headStr = header.toString();

            System.out.println(headStr);

            ArrayList<DNSQuestion> questions = requestMessage.getQuestion_();

            DNSCache cache = new DNSCache();

            for (DNSQuestion question : questions) {
                DNSRecord record = cache.queryRecord(question);
                if (record != null) {
                    //handle the request here
                } else {
                    InetAddress googleDNS = InetAddress.getByName("8.8.8.8");
                    DatagramPacket forwardGooglePkt = new DatagramPacket(buffer, buffer.length, googleDNS, 53);
                    socket.send(forwardGooglePkt);

                    byte[] responseBuffer = new byte[1024]; // Create a separate buffer for receiving the response
                    // Receive response from Google DNS
                    DatagramPacket responseGooglePkt = new DatagramPacket(responseBuffer, responseBuffer.length);
                    socket.receive(responseGooglePkt);
                    
                    // Decode response from Google DNS
                    DNSMessage googleResponse = DNSMessage.decodeMessage(responseGooglePkt.getData());

                    // Add answers to cache
                    for (DNSRecord answer : googleResponse.getAnswer_()) {
                        cache.insertRecord(question, answer);
                    }
                    // Add answers to response
                    googleResponse.getAnswer_().addAll(googleResponse.getAnswer_());

                }
            }





//            System.out.println(count + " Heard from " + pkt.getAddress() + " " + pkt.getPort());
//            for (int i = 0; i < pkt.getLength(); i++) {
//                System.out.printf(" %x", (int) buffer[i] & 0xFF);
//            }
//            System.out.println("\n");
        }

    }


}
