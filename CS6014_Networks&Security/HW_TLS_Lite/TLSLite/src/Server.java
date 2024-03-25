import java.io.*;
import java.math.BigInteger;
import java.net.Socket;
import java.security.cert.Certificate;
import java.net.ServerSocket;
import java.util.ArrayList;

public class Server {
    final static int PORT_NUM = 8080;
    private BigInteger serverDHPrivateKey;
    private BigInteger serverDHPublicKey;
    private BigInteger masterKey;
    private ArrayList<byte[]> messageHistory = new ArrayList<>();
    public void addMessage(final byte[] message){
        messageHistory.add(message);
    }
    public void initializeDHPrivateKey(BigInteger dhPrivateKey){
        this.serverDHPrivateKey = dhPrivateKey;
    }
    public void initializeDHPublicKey(BigInteger dhPublicKey){
        this.serverDHPublicKey = dhPublicKey;
    }
    public void initializeMasterKey(BigInteger masterKey){
        this.masterKey = masterKey;
    }
    public BigInteger getServerDHPublicKey() {
        return serverDHPublicKey;
    }
    private BigInteger getServerDHPrivateKey(){
        return serverDHPrivateKey;
    }
    private BigInteger getMasterKey(){
        return masterKey;
    }
    private ArrayList<byte[]> getMessageHistory(){
        return messageHistory;
    }

    public static void main(String[] args) throws Exception {

        Server server = new Server();
        ServerSocket ss = new ServerSocket(PORT_NUM);
        System.out.println("Server waiting on port number: " + PORT_NUM + "\n");

        Socket socket = ss.accept();
        ObjectInputStream is = new ObjectInputStream(socket.getInputStream());
        System.out.println("Server established connection with: " + socket.getInetAddress().getHostAddress() + "\n");

        //MESSAGE 1: Read nonce
        byte[] nonce = (byte[])is.readObject();
        String nonceStr = Shared.bytesToHex(nonce);
        server.addMessage(nonce);
        System.out.println("Server received nonce: "  + nonceStr + "\n");

        //Generate DH Keys
        BigInteger[] dhKeyPair = Shared.generateDHKeyPair();
        server.initializeDHPrivateKey(dhKeyPair[0]);
        server.initializeDHPublicKey(dhKeyPair[1]);

        ObjectOutputStream os = new ObjectOutputStream(socket.getOutputStream());

        //MESSAGE 2: Send a) certificate, b) DHPublicKey, c) signed DHPublicKey
        Certificate serverCertificate = Shared.getHostCertificate("server");
//        PublicKey serverRSAPublicKey = serverCertificate.getPublicKey();
        byte[] serverSignedDHKey_Bytes = Shared.generateSignedDHKey("server", server.getServerDHPublicKey());

        os.writeObject(serverCertificate);
        server.addMessage(serverCertificate.toString().getBytes());
        os.writeObject(server.getServerDHPublicKey());
        server.addMessage(server.getServerDHPublicKey().toByteArray());
        os.writeObject(serverSignedDHKey_Bytes);
        server.addMessage(serverSignedDHKey_Bytes);
        System.out.println("Server sent message 2\n");

        //MESSAGE 3: Receive a) certificate, b) DHPublicKey c) signed DHPublicKey
        Certificate clientCertificate = (Certificate)is.readObject();
        server.addMessage(clientCertificate.toString().getBytes());
        BigInteger clientDHPublicKey = (BigInteger)is.readObject();
        server.addMessage(clientDHPublicKey.toByteArray());
        byte[] clientSignedDHKey = (byte[])is.readObject();
        //need to encrypt the signed key prior to sending
        server.addMessage(clientSignedDHKey);
        System.out.println("Server received message 3");

        if(Shared.validateCertificate(clientCertificate)){
            server.initializeMasterKey(Shared.generateMasterKey(server.getServerDHPublicKey(), server.getServerDHPrivateKey()));
            Shared.generateSessionKeys(nonce, server.getMasterKey().toByteArray());
        }
        else{
            //todo: figure out what to do if does not validate
            is.close();
            socket.close();
            ss.close();
        }

        //MESSAGE 4: Send HMAC msg of history so far
        byte[] serverHMACMessage = Shared.generateMacMessage(server.getMessageHistory(), Shared.serverMAC);
        os.writeObject(serverHMACMessage);
        server.addMessage(serverHMACMessage);
        System.out.println("Server sent message 4");

        //Message 5: Receive HMAC msg of history so far
        byte[] clientHMACMessage = (byte[])is.readObject();
        server.addMessage(clientHMACMessage);
        System.out.println("Server received message 5");

        //VERIFY VALID HMAC
        if(Shared.verifyMessageHistory(clientHMACMessage, server.getMessageHistory(), Shared.serverMAC)){
            System.out.println("SERVER HANDSHAKE COMPLETED YESSSS QUEEN");
        }
        else {
            System.out.println("NOT THE SAME DUMMY");
        }


        is.close();
        socket.close();
        ss.close();

    }
}
