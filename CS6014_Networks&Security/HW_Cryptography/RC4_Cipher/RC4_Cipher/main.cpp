/**
* @file main.cpp
* @brief Implementation of the R64 Cipher algorithm.
*
* Provides an implementation of the R64 Cipher, including key scheduling
* and pseudo-random generation algorithms to encrypt or decrypt data.
* Created by Corinne Jones on 2/23/24.
*/
#include <iostream>

using namespace std;

/**
 * @class R64Cipher
 * @brief Implements the R64 Cipher algorithm for encryption and decryption.
 *
 * This class encapsulates the functionality of the R64 Cipher algorithm,
 * including initialization with a key and generation of the next byte
 * of the keystream.
 */
struct R64Cipher {
    vector<int> stateArr;
    
    int i, j;
    
    string key;

    /**
     * @brief Constructor that initializes the cipher with a given key.
     *
     * Initializes the state vector S and performs the Key Scheduling Algorithm (KSA)
     * to scramble the state based on the provided key.
     *
     * @param key The encryption key as a string.
     */
    R64Cipher(const string &key) {
        // Initialize member variables
         stateArr.resize(256);
         i = 0; 
         j = 0;
        this->key=key;
        
        // Initialize state vector
        for (int k = 0; k < 256; k++){
            stateArr[k] = k;
        }
        
        // Key Scheduling Algorithm
        for (int k = 0, j = 0; k < 256; ++k) {
            j = (j + stateArr[k] + key[k % key.length()]) % 256;
            swap(stateArr[k], stateArr[j]);
        }
    }
    
    /**
      * @brief Swaps two elements in the state vector.
      *
      * @param a Reference to the first element.
      * @param b Reference to the second element.
      */
    void swap(int &a, int &b) {
        int temp = a;
        a = b;
        b = temp;
    }

    /**
      * @brief Generates and returns the next byte of the keystream.
      *
      * Implements the Pseudo-Random Generation Algorithm (PRGA) to generate
      * the next byte of the keystream based on the current state.
      *
      * @return The next byte of the keystream as an integer.
      */
    int next_byte() {
        // Pseudo-Random Generation Algorithm
        i = (i + 1) % 256;
        j = (j + stateArr[i]) % 256;
        swap(stateArr[i], stateArr[j]);
        return stateArr[(stateArr[i] + stateArr[j]) % 256];
    }
    
    string encryptDecrypt(const string& input) {
        R64Cipher cipher(key);
        string output = input;

        for (int i = 0; i < input.size(); i++) {
            output[i] = input[i] ^ cipher.next_byte();
        }

        return output;
    }
    
};

/**
 * @brief Main function demonstrating the usage of the R64Cipher class.
 *
 * Initializes an R64Cipher with a specified key and generates the next byte
 * of the keystream, displaying it to the console.
 *
 * @return 0 upon successful execution.
 */
int main(int argc, const char * argv[]) {
   
    string key1 = "Key";
    string key2 = "NotKey";
    string salaryMessage = "Your salary is $1000"; //used for part three
    
    R64Cipher cipher1(key1);
    R64Cipher cipher2 (key2);
    R64Cipher cipher3 ("salaryKey"); //used for part three
   
    /**
     * Verify that decrypting a message with a different key than the encryption key does
     * not reveal the plaintext.
     *
     * Also verify that using the same key does reveal the plaintext.
     *
     */
    string originalText = "Hello, world!";
    
    string encrypted1 =  cipher1.encryptDecrypt(originalText);
    
    string decrypted1 = cipher1.encryptDecrypt(encrypted1);
    
    string decrypted2 = cipher2.encryptDecrypt(decrypted1);
    
    cout << "PART ONE" << endl;
    cout << "Message after encryption: " << encrypted1 << endl;
    cout << "Message after decryption with correct key: " << decrypted1 << endl;
    cout << "Message after decryption with wrong key: " << decrypted2 << endl;
    cout << endl;
    
    /**
     * Verify that encrypting 2 messages using the same keystream is insecure. What do
     * you expect to see if you xor the two encrypted messages?
     *
     */
    string message1 = "Message one";
    string message2 = "Message two";

    string encrypted2 = cipher1.encryptDecrypt(message1);
    string encrypted3 = cipher1.encryptDecrypt(message2);

    string xoredMessages;

    for (int i = 0; i < encrypted3.size(); i++) {
        xoredMessages += encrypted2[i] ^ encrypted3[i];
    }

    cout << "PART TWO" << endl;
    cout << "Encrypted 1: " << encrypted1 << endl;
    cout << "Encrypted 2: " << encrypted2 << endl;
    cout << "XORed Message: " << xoredMessages << endl;
    cout << endl;

    /**
     * Modify part of a message using a bit-flipping attack. For example, try sending the
     * message "Your salary is $1000" encrypted with RC4. Modify the ciphertext so that
     * when decrypted is says that your salary is $9999, instead. Hint: This should just
     * require using xor.
     *
     */
    string encryptedSalary = cipher3.encryptDecrypt(salaryMessage);

    for (int i = 0; i < 4; i++) {
        encryptedSalary[salaryMessage.find("1000") + i] ^= "1000"[i] ^ "9999"[i];
    }

    string modifiedSalaryMessage = cipher3.encryptDecrypt(encryptedSalary);

    cout << "PART THREE" << endl;
    cout << "Message before modification: " << salaryMessage << endl;
    cout << "Message after modification: " << modifiedSalaryMessage << endl;

    return 0;
}
