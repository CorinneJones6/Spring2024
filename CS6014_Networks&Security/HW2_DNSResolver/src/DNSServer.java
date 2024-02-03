import java.io.*;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.util.ArrayList;

public class DNSServer {
    private static final int DNS_SERVER_PORT = 8053;
    private static final DNSCache DNS_CACHE = new DNSCache();

    static DNSMessage sendDnsRequestToGoogle(DatagramSocket socket, byte[] buffer, DNSQuestion question) throws IOException {
        InetAddress googleDNS = InetAddress.getByName("8.8.8.8");
        DatagramPacket requestPkt = new DatagramPacket(buffer, buffer.length, googleDNS, 53);
        socket.send(requestPkt);

        byte[] responseBuffer = new byte[1024]; // Create a separate buffer for receiving the response
        DatagramPacket responsePkt = new DatagramPacket(responseBuffer, responseBuffer.length);
        socket.receive(responsePkt);
        System.out.println("Received: " + responsePkt);

        DNSMessage response = DNSMessage.decodeMessage(responsePkt.getData());

        for(DNSRecord answer : response.getAnswer_()){
            DNSCache.insertRecord(question, answer);
        }
        return response;
    }

    public static void main(String[] args) throws IOException {

        DatagramSocket socket = new DatagramSocket(DNS_SERVER_PORT);
//        DatagramSocket googleSocket = new DatagramSocket();

        while (true) {
            byte[] buffer = new byte[1024];
            DatagramPacket pkt = new DatagramPacket(buffer, buffer.length);
            socket.receive(pkt);

            DNSMessage request = DNSMessage.decodeMessage(pkt.getData());
            DNSMessage response = new DNSMessage();

            for (DNSQuestion question : request.getQuestion_()) {
                DNSRecord record = DNSCache.queryRecord(question);
                if (DNS_CACHE.isCached(question)) {
                    ArrayList<DNSRecord> arr = new ArrayList<>();
                    arr.add(record);
                    response = DNSMessage.buildResponse(request, arr);
                    
                } else {
                    response = sendDnsRequestToGoogle(socket, buffer, question);
                }
            }
            byte[] responseData = response.toBytes();
            DatagramPacket responsePacket = new DatagramPacket(responseData, responseData.length, pkt.getAddress(), pkt.getPort());
            socket.send(responsePacket);
        }
    }
}
