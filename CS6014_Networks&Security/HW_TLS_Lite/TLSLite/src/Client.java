import javax.crypto.BadPaddingException;
import javax.crypto.IllegalBlockSizeException;
import javax.crypto.NoSuchPaddingException;
import java.io.*;
import java.math.BigInteger;
import java.net.Socket;
import java.security.*;
import java.security.cert.Certificate;

public class Client {
    private BigInteger clientDHPrivateKey;
    private BigInteger clientDHPublicKey;
    private BigInteger masterKey;
    private ByteArrayOutputStream messageHistory = new ByteArrayOutputStream();
    final static String HOST = "localhost";
    final static int PORT_NUM = 8080;

    public void addMessage(final byte[] message) throws IOException {
        messageHistory.write(message);
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

    private ByteArrayOutputStream getMessageHistory(){
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
    private void completeHandShake(ObjectInputStream is, ObjectOutputStream os) throws Exception {
        //Send the nonce
        byte[] nonce = generateNonce();
        String nonceStr = Shared.bytesToHex(nonce);
        os.writeObject(nonce);
        addMessage(nonce);
        System.out.println("Client sent nonce: " + nonceStr + "\n");

        //Generate DH Keys
        BigInteger[] dhKeyPair= Shared.generateDHKeyPair();
        initializeDHPrivateKey(dhKeyPair[0]);
        initializeDHPublicKey(dhKeyPair[1]);

        //Handle Certificate / Key Sharing
        Certificate serverCertificate = Shared.receiveCertificateAndKeys(is, getMessageHistory());
        Shared.sendCertificateAndKeys(os, getClientDHPublicKey(), getMessageHistory());

        //Validate Certificate, Generate Master Key, Generate Session Keys
        BigInteger masterKey = Shared.validateAndGenerateMasterandSessionKeys(nonce, serverCertificate, getClientDHPublicKey(), getClientDHPrivateKey());

        //Store as a member variable
        initializeMasterKey(masterKey);

        //Receive and send Mac Msg
        byte[] hmacMsg = Shared.receiveMacMsg(is);
        Shared.generateMacMsg("client", os, getMessageHistory());
        Shared.validateMacMsg("client", hmacMsg, getMessageHistory());
    }

    private void completeMessages(ObjectOutputStream os, ObjectInputStream is) throws InvalidAlgorithmParameterException, NoSuchPaddingException, IllegalBlockSizeException, IOException, NoSuchAlgorithmException, BadPaddingException, InvalidKeyException, ClassNotFoundException {
        String receivedMsg = Shared.receiveEncryptedMessage("client", is);
        System.out.println(receivedMsg);

        String helloMsg = "Hello from the client!";
        Shared.sendEncryptedMessage("client", os, helloMsg);
    }

    public static void main(String[] args) throws Exception {

        //Establish a connection with the server
        Socket socket = new Socket(HOST, PORT_NUM);
        System.out.println("Connection established with: " + HOST);

        ObjectOutputStream os = new ObjectOutputStream(socket.getOutputStream());
        ObjectInputStream is = new ObjectInputStream(socket.getInputStream());

        Client client = new Client();
        client.completeHandShake(is, os);

        client.completeMessages(os, is);

        socket.close();
    }
}
