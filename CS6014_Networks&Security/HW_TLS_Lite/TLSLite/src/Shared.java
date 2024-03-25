import javax.crypto.Mac;
import javax.crypto.spec.DHParameterSpec;
import javax.crypto.spec.SecretKeySpec;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
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
import java.util.ArrayList;
import java.util.Arrays;


public class Shared {
    private static final BigInteger g = new BigInteger("2"); // base generator
    private static final BigInteger N = new BigInteger("1"); // prime modulus, replace "..." with a large prime value

    //SESSION KEYS?
    private static byte[] serverEncrypt;
    private static byte[] clientEncrypt;
    static byte[] serverMAC;
    static byte[] clientMAC;
    private static byte[] serverIV;
    private static byte[] clientIV;
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

    public static BigInteger[] generateDHKeyPair() throws InvalidAlgorithmParameterException, NoSuchAlgorithmException {
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

    //TODO: figure out how to encrypt this information
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
    //TODO: Make sure this using the nonce and the master key appropriately
    static void generateSessionKeys(byte[] clientNonce, byte[] masterKey) throws NoSuchAlgorithmException, InvalidKeyException {
        byte[] prk = hkdfExpand(masterKey, "masterKey" + new String(clientNonce, StandardCharsets.UTF_8));
        serverEncrypt = hkdfExpand(prk, "server encrypt");
        clientEncrypt = hkdfExpand(serverEncrypt, "client encrypt");
        serverMAC = hkdfExpand(clientEncrypt, "server MAC");
        clientMAC = hkdfExpand(serverMAC, "client MAC");
        serverIV = hkdfExpand(clientMAC, "server IV");
        clientIV = hkdfExpand(serverIV, "client IV");

        System.out.println("YOU ARE THE WINNER");
    }

    public static byte[] generateMacMessage(ArrayList<byte[]> message, byte[] key) throws NoSuchAlgorithmException, InvalidKeyException, IOException {
        Mac HMAC = Mac.getInstance("HmacSHA256");

        // Generate a new key from macKey and initialize the Mac instance with the new key
        SecretKeySpec secretKeySpec = new SecretKeySpec(key, "HmacSHA256");
        HMAC.init(secretKeySpec);

        // Iterate through the ArrayList, updating the Mac instance with each byte array
        for (byte[] messagePart : message) {
            HMAC.update(messagePart);
        }

        // Compute the MAC
        return HMAC.doFinal();

    }

    public static boolean verifyMessageHistory(byte[] otherHostMacMsg, ArrayList<byte[]> myMsgs, byte[] macKey) throws NoSuchAlgorithmException, IOException, InvalidKeyException {
        byte[] myMacMsg = Shared.generateMacMessage(myMsgs, macKey);
        return Arrays.equals(myMacMsg, otherHostMacMsg);
    }

    public static void encryptMessage(){

    }
    public static void decryptMessage(){

    }



}
