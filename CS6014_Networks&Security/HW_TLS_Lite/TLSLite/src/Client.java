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

    public byte[] generateNonce(){
        SecureRandom random = new SecureRandom();
        byte[] nonce = new byte[32];
        random.nextBytes(nonce);

        return nonce;
    }

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

    public static void main(String[] args) throws IOException, InvalidAlgorithmParameterException, NoSuchAlgorithmException, ClassNotFoundException, CertificateException, SignatureException, InvalidKeyException, NoSuchProviderException {

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

        //MESSAGE 2: Receive a.) certificate, b.) DHPublicKey c.) signed DHPublicKey
        Certificate serverCertificate = (Certificate)is.readObject();
        client.addMessage(serverCertificate.toString().getBytes());
        BigInteger serverDHPublicKey = (BigInteger)is.readObject();
        client.addMessage(serverDHPublicKey.toByteArray());
        BigInteger serverSignedDHKey = (BigInteger)is.readObject();
        client.addMessage(serverSignedDHKey.toByteArray());
        System.out.println("Client received message 2" + "\n");


        //Message 3: Send a) certificate, b) DHPublicKey, c) signed DHPublicKey
        Certificate clientCertificate = Shared.getHostCertificate("client");
        PublicKey clientRSAPublicKey = clientCertificate.getPublicKey();
        BigInteger clientSignedDHKey = Shared.generateSignedDHKey("client", client.getClientDHPublicKey(), clientRSAPublicKey);

        os.writeObject(clientCertificate);
        client.addMessage(clientCertificate.toString().getBytes());
        os.writeObject(client.getClientDHPublicKey());
        client.addMessage(client.getClientDHPublicKey().toByteArray());
        os.writeObject(clientSignedDHKey);
        client.addMessage(clientSignedDHKey.toByteArray());
        System.out.println("Client sent message 3");

        if(Shared.validateCertificate(serverCertificate)){
            client.initializeMasterKey(Shared.generateMasterKey(client.getClientDHPublicKey(), client.getClientDHPrivateKey()));
            Shared.generateSessionKeys(client.getMasterKey());
        }
        else{
            //this means it did not validate
            //todo: figure out what to do if not valid
        }

        socket.close();

    }
}
