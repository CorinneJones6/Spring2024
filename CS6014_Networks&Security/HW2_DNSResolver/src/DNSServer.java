import java.io.*;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.util.ArrayList;

public class DNSServer {
    private static final int DNS_SERVER_PORT = 8053;
    private static final int MAX_PACKET_SIZE = 1024;
    private static final int GOOGLE_DNS_PORT = 53;
    private static final String GOOGLE_DNS_IP = "8.8.8.8";

    /**
     * The main method of the DNS server.
     *
     * @param args Command-line arguments (not used in this application).
     * @throws IOException If there is an error in network communication.
     */
    public static void main(String[] args) throws IOException {

        DatagramSocket socket = new DatagramSocket(DNS_SERVER_PORT);

        while (true) {
            byte[] buffer = new byte[MAX_PACKET_SIZE];
            DatagramPacket pkt = new DatagramPacket(buffer, buffer.length);
            socket.receive(pkt);

            // Decode the received DNS request into a DNSMessage
            DNSMessage request = DNSMessage.decodeMessage(pkt.getData());
            DNSMessage response = new DNSMessage();

            // Process each DNSQuestion in the request
            for (DNSQuestion question : request.getQuestions_()) {
                // Query the DNSCache for a cached DNSRecord
                DNSRecord record = DNSCache.queryRecord(question);
                if (record!=null) {
                    // If a cached record exists, build a response with the cached record
                    ArrayList<DNSRecord> answerRecords = new ArrayList<>();
                    answerRecords.add(record);
                    response = DNSMessage.buildResponse(request, answerRecords);
                } else {
                    // If no cached record exists, send a request to the Google DNS server
                    byte[] requestBytes = request.toBytes();
                    response = sendRequestToGoogle(socket, requestBytes, question);
                }
            }
            // Convert the response DNSMessage to bytes
            byte[] responseData = response.toBytes();

            // Create a DatagramPacket for the response and send it back to the client
            DatagramPacket responsePacket = new DatagramPacket(responseData, responseData.length, pkt.getAddress(), pkt.getPort());
            socket.send(responsePacket);
        }
    }

    /**
     * Sends a DNS request to Google's DNS server and receives a DNS response.
     *
     * @param socket   The DatagramSocket used for communication.
     * @param buffer   The DNS request message in byte array form.
     * @param question The DNS question to be resolved.
     * @return The DNS response message from Google's DNS server.
     * @throws IOException If there is an error in network communication.
     */
    static DNSMessage sendRequestToGoogle(DatagramSocket socket, byte[] buffer, DNSQuestion question) throws IOException {
        // Get the InetAddress for the Google DNS server
        InetAddress googleDNS = InetAddress.getByName(GOOGLE_DNS_IP);

        // Create a DatagramPacket for the request and send it
        DatagramPacket requestPkt = new DatagramPacket(buffer, buffer.length, googleDNS, GOOGLE_DNS_PORT);
        socket.send(requestPkt);

        // Create a separate buffer for receiving the response
        byte[] responseBuffer = new byte[MAX_PACKET_SIZE];
        DatagramPacket responsePkt = new DatagramPacket(responseBuffer, responseBuffer.length);

        // Receive the response from the Google DNS server
        socket.receive(responsePkt);
        System.out.println("Received: " + responsePkt);

        // Decode the received response into a DNSMessage
        DNSMessage response = DNSMessage.decodeMessage(responsePkt.getData());

        // Insert the received answers into the DNSCache
        for (DNSRecord answer : response.getAnswers_()) {
            DNSCache.insertRecord(question, answer);
        }
        return response;
    }
}
