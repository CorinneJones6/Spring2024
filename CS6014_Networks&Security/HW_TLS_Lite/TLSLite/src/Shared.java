import javax.crypto.*;
import javax.crypto.spec.IvParameterSpec;
import javax.crypto.spec.SecretKeySpec;
import java.io.*;
import java.math.BigInteger;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.security.*;
import java.security.cert.Certificate;
import java.security.cert.CertificateException;
import java.security.cert.CertificateFactory;
import java.security.spec.InvalidKeySpecException;
import java.security.spec.PKCS8EncodedKeySpec;
import java.util.Arrays;


public class Shared {
    private static final BigInteger g = new BigInteger("2"); // base generator
    private static final BigInteger N = new BigInteger("1"); // prime modulus
    static byte[] serverEncrypt;
    static byte[] clientEncrypt;
    static byte[] serverMAC;
    static byte[] clientMAC;
    static byte[] serverIV;
    static byte[] clientIV;
    static Certificate CACertificate;

    static {
        try {
            CACertificate = Shared.getHostCertificate("CA");
        } catch (CertificateException e) {
            throw new RuntimeException(e);
        }
    }

    public static String bytesToHex(byte[] bytes) {
        StringBuilder hexString = new StringBuilder();
        for (byte b : bytes) {
            String hex = Integer.toHexString(0xff & b);
            if (hex.length() == 1) hexString.append('0');
            hexString.append(hex);
        }
        return hexString.toString();
    }
    public static Certificate getHostCertificate(String caller) throws CertificateException {
        String filepath;
        if(caller.equals("server")){
            filepath = "../TLSDirectory/CASignedServerCertificate.pem";
        }
        else if(caller.equals("client")) {
            filepath = "../TLSDirectory/CASignedClientCertificate.pem";
        }
        else if(caller.equals("CA")){
            filepath = "../TLSDirectory/CACertificate.pem";
        }
        else {
            throw new IllegalArgumentException("invalid name provided");
        }
        try (FileInputStream fileInput = new FileInputStream(filepath)) {
            CertificateFactory certificateFactory = CertificateFactory.getInstance("X.509");
            return certificateFactory.generateCertificate(fileInput);
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
    }

    public static BigInteger[] generateDHKeyPair() {
        BigInteger[] returnArray = new BigInteger[2];

        BigInteger privateKey = generateDHPrivateKey(N.bitLength());

        BigInteger publicKey = generateDHPublicKey(privateKey);

        returnArray[0] = privateKey;
        returnArray[1] = publicKey;

        return returnArray;
    }

    private static BigInteger generateDHPrivateKey(int bitLength) {
        SecureRandom random = new SecureRandom();
        return new BigInteger(bitLength - 1, random); // Ensure the private key is suitably large
    }
    private static BigInteger generateDHPublicKey(BigInteger privateKey){
        return g.modPow(privateKey, N);
    }

    //TODO: figure out how to encrypt this information???
    private static PrivateKey generateRSAPrivateKey(String filepath) throws IOException, NoSuchAlgorithmException, InvalidKeySpecException {
        byte[] keyBytes = Files.readAllBytes(Paths.get(filepath));
        PKCS8EncodedKeySpec spec = new PKCS8EncodedKeySpec(keyBytes);
        KeyFactory kf = KeyFactory.getInstance("RSA");
        return kf.generatePrivate(spec);
    }
    private static byte[] signData(PrivateKey privateKey, byte[] data) throws Exception {
        Signature signature = Signature.getInstance("SHA256withRSA");
        signature.initSign(privateKey);
        signature.update(data);
        return signature.sign();
    }
    public static byte[] generateSignedDHKey(String caller, BigInteger DHPublicKey) throws Exception {
        String filepath;
        if(caller.equals("server")){
            filepath = "../TLSDirectory/serverPrivateKey.der";
        }
        else if(caller.equals("client")){
            filepath = "../TLSDirectory/clientPrivateKey.der";
        }
        else {
            throw new IllegalArgumentException("invalid name provided");
        }
        PrivateKey RSAPrivateKey = generateRSAPrivateKey(filepath);

        byte[] DHPublicKey_Bytes = DHPublicKey.toByteArray();

        return signData(RSAPrivateKey, DHPublicKey_Bytes);
    }

    static boolean validateCertificate(Certificate certificate) {
        try {
            PublicKey CApublicKey = Shared.CACertificate.getPublicKey();
            certificate.verify(CApublicKey);
            return true; // Verification successful
        } catch (CertificateException | NoSuchAlgorithmException | SignatureException | InvalidKeyException | NoSuchProviderException e) {
            // Any exception caught here indicates the certificate could not be verified.
            return false; // Verification failed
        }
    }

    static BigInteger generateMasterKey(BigInteger DHPublicKey, BigInteger DHPrivateKey){
        return DHPublicKey.modPow(DHPrivateKey, N);
    }

    public static byte[] hkdfExpand(byte[] input, String tag) throws NoSuchAlgorithmException, InvalidKeyException {
        Mac mac = Mac.getInstance("HmacSHA256");
        mac.init(new SecretKeySpec(input, "HmacSHA256"));
        mac.update(tag.getBytes(StandardCharsets.UTF_8));
        mac.update((byte) 1);
        byte[] okm = mac.doFinal();
        byte[] result = new byte[16];
        System.arraycopy(okm, 0, result, 0, 16);
        return result;
    }
    //TODO: Make sure this using the nonce and the master key appropriately?
    static void generateSessionKeys(byte[] clientNonce, byte[] masterKey) throws NoSuchAlgorithmException, InvalidKeyException {
        byte[] prk = hkdfExpand(masterKey, "masterKey" + new String(clientNonce, StandardCharsets.UTF_8));
        serverEncrypt = hkdfExpand(prk, "server encrypt");
        clientEncrypt = hkdfExpand(serverEncrypt, "client encrypt");
        serverMAC = hkdfExpand(clientEncrypt, "server MAC");
        clientMAC = hkdfExpand(serverMAC, "client MAC");
        serverIV = hkdfExpand(clientMAC, "server IV");
        clientIV = hkdfExpand(serverIV, "client IV");

//        printKeyInformation();
        System.out.println("Session keys generated");
    }

    public static byte[] generateMacMessage(byte[] message, byte[] key) throws NoSuchAlgorithmException, InvalidKeyException {
        Mac HMAC = Mac.getInstance("HmacSHA256");

        //Generate a new key from macKey and initializes the Mac instance with the new key
        SecretKeySpec secretKeySpec = new SecretKeySpec(key, "HmacSHA256");
        HMAC.init(secretKeySpec);

        HMAC.update(message);
        return HMAC.doFinal();

    }

    public static boolean verifyMessageHistory(byte[] otherHostMacMsg, byte[] thisHostMsg, byte[] macKey) throws NoSuchAlgorithmException, InvalidKeyException {
        byte[] thisHostMacMsg = Shared.generateMacMessage(thisHostMsg, macKey);
        return Arrays.equals(thisHostMacMsg, otherHostMacMsg);
    }

    private static Cipher generateCipher(byte[] key, byte[] iv, int mode) throws NoSuchPaddingException, NoSuchAlgorithmException, InvalidAlgorithmParameterException, InvalidKeyException {
        SecretKeySpec keySpec = new SecretKeySpec(key, "AES");
        IvParameterSpec ivSpec = new IvParameterSpec(iv);

        Cipher encryptCipher = Cipher.getInstance("AES/CBC/PKCS5Padding");
        encryptCipher.init(mode, keySpec, ivSpec);

        return encryptCipher;
    }

    public static byte[] encryptMessage(String message, byte[] MACkey, byte[] encryptKey, byte[] hostIV) throws IOException, NoSuchAlgorithmException, InvalidKeyException, NoSuchPaddingException, InvalidAlgorithmParameterException, IllegalBlockSizeException, BadPaddingException {
        byte[] message_bytes = message.getBytes();
        ByteArrayOutputStream outStream = new ByteArrayOutputStream();
        outStream.write(message_bytes);

        byte[] hmac = Shared.generateMacMessage(message_bytes, MACkey);
        outStream.write(hmac);
        Cipher cipher = generateCipher(encryptKey, hostIV, Cipher.ENCRYPT_MODE);

        return cipher.doFinal(outStream.toByteArray());
    }
    public static String decryptMessage(byte[] encryptedData, byte[]decriptKey, byte[] hostIV, byte[]MACkey) throws InvalidAlgorithmParameterException, NoSuchPaddingException, NoSuchAlgorithmException, InvalidKeyException, IllegalBlockSizeException, BadPaddingException {
        Cipher cipher = generateCipher(decriptKey, hostIV, Cipher.DECRYPT_MODE);
        byte[] decryptedData = cipher.doFinal(encryptedData);

        // Assuming the last part of the decrypted data is the HMAC and the rest is the message
        //todo: check the hmac?

        byte[] messageBytes = Arrays.copyOf(decryptedData, decryptedData.length-32);

        return new String(messageBytes, StandardCharsets.UTF_8);
    }

    public static void printKeyInformation() {
        System.out.println("Server Encrypt: " + bytesToHex(serverEncrypt));
        System.out.println("Client Encrypt: " + bytesToHex(clientEncrypt));
        System.out.println("Server MAC: " + bytesToHex(serverMAC));
        System.out.println("Client MAC: " + bytesToHex(clientMAC));
        System.out.println("Server IV: " + bytesToHex(serverIV));
        System.out.println("Client IV: " + bytesToHex(clientIV));
    }

            /*----------------------------------------------------------------------
                                     SHARED HANDSHAKE METHODS
         ----------------------------------------------------------------------*/

    public static void sendCertificateAndKeys(ObjectOutputStream os, BigInteger dhPublicKey, ByteArrayOutputStream msgHistory) throws Exception {
        //MESSAGE 2: Send a) certificate, b) DHPublicKey, c) signed DHPublicKey
        Certificate serverCertificate = Shared.getHostCertificate("server");
        byte[] serverSignedDHKey_Bytes = Shared.generateSignedDHKey("server", dhPublicKey);

        os.writeObject(serverCertificate);
        msgHistory.write(serverCertificate.toString().getBytes());
        os.writeObject(dhPublicKey);
        msgHistory.write(dhPublicKey.toByteArray());
        os.writeObject(serverSignedDHKey_Bytes);
        msgHistory.write(serverSignedDHKey_Bytes);
        System.out.println("Certificate and Keys sent");
    }

    public static Certificate receiveCertificateAndKeys(ObjectInputStream is, ByteArrayOutputStream msgHistory) throws IOException, ClassNotFoundException {
        //MESSAGE 3: Receive a. certificate, b. DHPublicKey c. signed DHPublicKey
        Certificate clientCertificate = (Certificate)is.readObject();
        msgHistory.write(clientCertificate.toString().getBytes());
        BigInteger clientDHPublicKey = (BigInteger)is.readObject();
        msgHistory.write(clientDHPublicKey.toByteArray());
        byte[] clientSignedDHKey = (byte[])is.readObject();
        //need to encrypt the signed key prior to sending
        msgHistory.write(clientSignedDHKey);
        System.out.println("Certificate and Keys received");
        return clientCertificate;
    }

    public static BigInteger validateAndGenerateMasterandSessionKeys(byte[] nonce, Certificate certificate, BigInteger dhPublicKey, BigInteger dhPrivateKey) throws NoSuchAlgorithmException, InvalidKeyException {
        if(Shared.validateCertificate(certificate)){
            BigInteger masterKey = Shared.generateMasterKey(dhPublicKey, dhPrivateKey);
            Shared.generateSessionKeys(nonce, masterKey.toByteArray());
            return masterKey;
        }
        else{
            //todo: figure out what to do if does not validate
//            is.close();
//            socket.close();
//            ss.close();
            return null;
        }
    }

    public static void generateMacMsg(String caller, ObjectOutputStream os, ByteArrayOutputStream msgHistory) throws NoSuchAlgorithmException, InvalidKeyException, IOException {
        //MESSAGE 4: Send HMAC msg of history so far
        byte[] key;
        if(caller.equals("server")) {
            key = Shared.serverMAC;
        }
        else{
            key = Shared.clientMAC;
        }
            byte[] msg = Shared.generateMacMessage(msgHistory.toByteArray(), key);
            os.writeObject(msg);
            System.out.println("Mac msg sent");

    }

    public static byte[] receiveMacMsg(ObjectInputStream is) throws IOException, ClassNotFoundException {
        byte[] clientHMACMessage = (byte[])is.readObject();
        System.out.println("Mac msg received");

        return clientHMACMessage;
    }


    public static void validateMacMsg(String caller, byte[] msg, ByteArrayOutputStream msgHistory) throws NoSuchAlgorithmException, InvalidKeyException {
        //VERIFY VALID HMAC
        byte[] key;
        if(caller.equals("server")) {
            key = Shared.clientMAC;
        }
        else{
            key = Shared.serverMAC;
        }
        if(Shared.verifyMessageHistory(msg, msgHistory.toByteArray(), key)){
            System.out.println("SERVER HANDSHAKE COMPLETED");
        }
        else {
            System.out.println("HMAC MESSAGES WERE NOT THE SAME");
        }
    }

                /*----------------------------------------------------------------------
                                     SHARED MESSAGE METHODS
         ----------------------------------------------------------------------*/

    public static void sendEncryptedMessage(String caller, ObjectOutputStream os, String msg) throws InvalidAlgorithmParameterException, NoSuchPaddingException, IllegalBlockSizeException, IOException, NoSuchAlgorithmException, BadPaddingException, InvalidKeyException {

        if(caller.equals("server")){
            byte[] message1_encrypted = Shared.encryptMessage(msg, Shared.serverMAC, Shared.serverEncrypt, Shared.serverIV);
            os.writeObject(message1_encrypted);
        }
        else {

            byte[] message1_encrypted = Shared.encryptMessage(msg, Shared.clientMAC, Shared.clientEncrypt, Shared.clientIV);
            os.writeObject(message1_encrypted);
        }

    }
    public static String receiveEncryptedMessage(String caller, ObjectInputStream is) throws IOException, ClassNotFoundException, InvalidAlgorithmParameterException, NoSuchPaddingException, IllegalBlockSizeException, NoSuchAlgorithmException, BadPaddingException, InvalidKeyException {
        if(caller.equals("server")) {
            byte[] receivedMsg_encrypted = (byte[]) is.readObject();
            return Shared.decryptMessage(receivedMsg_encrypted, Shared.clientEncrypt, Shared.clientIV, Shared.clientMAC);
        }
        else {
            byte[] receivedMsg_encrypted = (byte[]) is.readObject();
            return Shared.decryptMessage(receivedMsg_encrypted, Shared.serverEncrypt, Shared.serverIV, Shared.serverMAC);
        }
    }

}
