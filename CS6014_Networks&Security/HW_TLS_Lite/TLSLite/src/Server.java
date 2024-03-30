import java.io.*;
import java.math.BigInteger;
import java.net.Socket;
import java.security.cert.Certificate;
import java.net.ServerSocket;
/**
 * Handles server-side operations for a TLS-light handshake and secure messaging with a client.
 * Manages secure session setup, including key exchanges, certificate validation, and encrypted
 * communication, adhering to a simplified TLS handshake protocol for cryptographic security.
 *
 * Created by Corinne Jones in 3/2024
 *
 */
public class Server {
    final int PORT_NUM = 8080;
    private BigInteger serverDHPrivateKey;
    private BigInteger serverDHPublicKey;
    private BigInteger masterKey;
    private ByteArrayOutputStream msgHistoryStream = new ByteArrayOutputStream();
    private void writeMessage(final byte[] message) throws IOException {
        msgHistoryStream.write(message);
    }
    private void setDHPrivateKey(BigInteger dhPrivateKey){
        this.serverDHPrivateKey = dhPrivateKey;
    }
    private void setDHPublicKey(BigInteger dhPublicKey){
        this.serverDHPublicKey = dhPublicKey;
    }
    private void setMasterKey(BigInteger masterKey){
        this.masterKey = masterKey;
    }
    private BigInteger getServerDHPublicKey() {
        return serverDHPublicKey;
    }
    private BigInteger getServerDHPrivateKey(){
        return serverDHPrivateKey;
    }
    private BigInteger getMasterKey(){
        return masterKey;
    }
    private ByteArrayOutputStream getMsgHistoryStream(){
        return msgHistoryStream;
    }

    private byte[] retrieveNonce(ObjectInputStream is) throws IOException, ClassNotFoundException {
        byte[] nonce = (byte[]) is.readObject();
        String nonceStr = Shared.bytesToHex(nonce);
        writeMessage(nonce);
        System.out.println("Server received nonce: "  + nonceStr + "\n");
        return nonce;
    }
    private void completeHandShake(ObjectInputStream is, ObjectOutputStream os) {
        try {
            byte[] nonce = retrieveNonce(is);

            //Generate DH Keys
            BigInteger[] dhKeyPair = Shared.generateDHKeyPair();
            setDHPrivateKey(dhKeyPair[0]);
            setDHPublicKey(dhKeyPair[1]);

            Shared.sendCertificateAndKeys(os, getServerDHPublicKey(), getMsgHistoryStream());

            Certificate certificate = Shared.receiveCertificateAndKeys(is, getMsgHistoryStream());

            BigInteger masterKey = Shared.validateAndGenerateMasterandSessionKeys(nonce, certificate, getServerDHPublicKey(), getServerDHPrivateKey());

            setMasterKey(masterKey);

            Shared.generateMacMsg("server", os, getMsgHistoryStream());

            byte[] hmacMsg = Shared.receiveMacMsg(is);

            Shared.validateMacMsg("server", hmacMsg, getMsgHistoryStream());
        }
        catch (IOException | ClassNotFoundException e){
            throw new RuntimeException("Failed to complete the handshake: " + e.getMessage(), e);
        }

    }
    private void completeMessages(ObjectOutputStream os, ObjectInputStream is) {
        String helloMsg = "Hello from the server!";
        Shared.sendEncryptedMessage("server", os, helloMsg);

        String receivedMsg = Shared.receiveEncryptedMessage("server", is);
        System.out.println(receivedMsg);
    }
    public static void main(String[] args) throws Exception {

        Server server = new Server();
        ServerSocket ss = new ServerSocket(server.PORT_NUM);
        System.out.println("Server waiting on port number: " + server.PORT_NUM + "\n");

        Socket socket = ss.accept();
        ObjectInputStream is = new ObjectInputStream(socket.getInputStream());
        System.out.println("Server established connection with: " + socket.getInetAddress().getHostAddress() + "\n");

        ObjectOutputStream os = new ObjectOutputStream(socket.getOutputStream());

        server.completeHandShake(is, os);
        server.completeMessages(os, is );

        is.close();
        socket.close();
        ss.close();

    }
}
