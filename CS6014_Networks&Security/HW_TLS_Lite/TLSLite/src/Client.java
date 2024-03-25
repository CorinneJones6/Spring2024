import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.math.BigInteger;
import java.net.Socket;
import java.security.*;
import java.security.cert.Certificate;
import java.security.cert.CertificateException;
import java.util.ArrayList;

public class Client {
    private BigInteger clientDHPrivateKey;
    private BigInteger clientDHPublicKey;
    private BigInteger masterKey;
    private ArrayList<byte[]> messageHistory = new ArrayList<>();
    final static String HOST = "localhost";
    final static int PORT_NUM = 8080;

    public void addMessage(final byte[] message){
        messageHistory.add(message);
    }
    public void initializeDHPrivateKey(BigInteger dhPrivateKey){
        this.clientDHPrivateKey = dhPrivateKey;
    }
    public void initializeDHPublicKey(BigInteger dhPublicKey){
        this.clientDHPublicKey = dhPublicKey;
    }
    public void initializeMasterKey(BigInteger masterKey){
        this.masterKey = masterKey;
    }
    public BigInteger getClientDHPublicKey() {
        return clientDHPublicKey;
    }
    private BigInteger getClientDHPrivateKey(){
        return clientDHPrivateKey;
    }
    private BigInteger getMasterKey(){
        return masterKey;
    }

    private ArrayList<byte[]> getMessageHistory(){
        return messageHistory;
    }

    /**
     * Generates a cryptographically strong random nonce.
     *
     * This method creates a 32-byte (256-bit) nonce using a secure random number generator. A nonce is a "number used once"
     * and is critical in cryptographic protocols to ensure that communications are unique and to prevent replay attacks.
     *
     * @return A 32-byte array containing the generated nonce.
     */
    public byte[] generateNonce(){
        SecureRandom random = new SecureRandom();
        byte[] nonce = new byte[32];
        random.nextBytes(nonce); //Fill the byte array with random data

        return nonce;
    }
    public static void main(String[] args) throws Exception {

        //establish a connection with the server
        Socket socket = new Socket(HOST, PORT_NUM);
        System.out.println("Connection established with: " + HOST);
        ObjectOutputStream os = new ObjectOutputStream(socket.getOutputStream());

        //MESSAGE 1: Client send Nonce
        Client client = new Client();
        byte[] nonce = client.generateNonce();
        String nonceStr = Shared.bytesToHex(nonce);
        os.writeObject(nonce);
        client.addMessage(nonce);
        System.out.println("Client sent nonce: " + nonceStr + "\n");

        //Generate DH Keys
        BigInteger[] dhKeyPair= Shared.generateDHKeyPair();
        client.initializeDHPrivateKey(dhKeyPair[0]);
        client.initializeDHPublicKey(dhKeyPair[1]);

        ObjectInputStream is = new ObjectInputStream(socket.getInputStream());

        //MESSAGE 2: Receive a) certificate, b) DHPublicKey c) signed DHPublicKey
        Certificate serverCertificate = (Certificate)is.readObject();
        client.addMessage(serverCertificate.toString().getBytes());
        BigInteger serverDHPublicKey = (BigInteger)is.readObject();
        client.addMessage(serverDHPublicKey.toByteArray());
        byte[] serverSignedDHKey = (byte[]) is.readObject();
        //need to decrypt the signed DH Key
        client.addMessage(serverSignedDHKey);
        System.out.println("Client received message 2" + "\n");


        //Message 3: Send a) certificate, b) DHPublicKey, c) signed DHPublicKey
        Certificate clientCertificate = Shared.getHostCertificate("client");
//        PublicKey clientRSAPublicKey = clientCertificate.getPublicKey();
        byte[] clientSignedDHKey_Byte = Shared.generateSignedDHKey("client", client.getClientDHPublicKey());

        os.writeObject(clientCertificate);
        client.addMessage(clientCertificate.toString().getBytes());
        os.writeObject(client.getClientDHPublicKey());
        client.addMessage(client.getClientDHPublicKey().toByteArray());
        os.writeObject(clientSignedDHKey_Byte);
        client.addMessage(clientSignedDHKey_Byte);
        System.out.println("Client sent message 3");

        if(Shared.validateCertificate(serverCertificate)){
            client.initializeMasterKey(Shared.generateMasterKey(client.getClientDHPublicKey(), client.getClientDHPrivateKey()));
            Shared.generateSessionKeys(nonce, client.getMasterKey().toByteArray());
        }
        else{
            //todo: figure out what to do if does not validate
            socket.close();
        }

        //Message 4: Receive HMAC msg of history so far
        byte[] serverHMACMessage = (byte[])is.readObject();
        client.addMessage(serverHMACMessage);
        System.out.println("Client received message 4");


        //Message 5: Send HMAC msg of history so far
        byte[] clientHMACMessage = Shared.generateMacMessage(client.getMessageHistory(), Shared.clientMAC);
        os.writeObject(clientHMACMessage);
        client.addMessage(clientHMACMessage);
        System.out.println("Client sent message 5");

        //VERIFY VALID HMAC
        if(Shared.verifyMessageHistory(serverHMACMessage, client.getMessageHistory(), Shared.clientMAC)){
            System.out.println("CLIENT HANDSHAKE COMPLETED YESSSS QUEEN");
        }
        else {
            System.out.println("NOT THE SAME DUMMY");
        }

        socket.close();

    }
}
