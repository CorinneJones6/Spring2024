//
//  main.cpp
//  Cryptography
//
//  Created by Corinne Jones on 2/21/24.
//

#include <iostream>

using namespace std;

using Block = array<uint8_t, 8>;

/**
 * Generates a key from a given string by XORing each character of the string with the key array elements in a cyclic manner.
 * @param str The input string to generate the key from.
 * @return Block The generated key as a Block array.
 */
Block generateKey(const string &str){
    Block key = {0, 0, 0, 0, 0, 0, 0, 0};
    
    for(int i=0; i<str.length(); i++){
        key[i % 8] = key [i % 8] ^ str[i];
    }
    return key;
}

/**
 * Fills a vector with values from 0 to 255.
 * @param arr Reference to the vector to be filled.
 */
void fillArray(vector<uint8_t> &arr){
    for(int i=0; i<256; i++){
        arr.push_back(i);
    }
}

/**
 * Applies the Fisher-Yates shuffle algorithm to randomly shuffle the elements of a vector.
 * @param arr Reference to the vector to be shuffled.
 */
void fisherYateShuffle(vector<uint8_t> &arr){
    
    for(int i=arr.size()-1; i>0; i--){
        int j = rand() % i;
        swap(arr[i], arr[j]);
    }
}

/**
 * Initializes substitution tables with randomly shuffled arrays of 0 to 255.
 * @param substTables Reference to a vector of substitution tables to be initialized.
 */
void initializeTables(vector<vector<uint8_t>> &substTables){

    for(int i=0; i<8; i++){
        vector<uint8_t> substTable;
        fillArray(substTable);
        substTables.push_back(substTable);
    }
    
    for(vector<uint8_t> substTable : substTables){
        fisherYateShuffle(substTable);
    }
}

/**
 * Converts a string message into a Block by copying each character into the Block.
 * @param strMsg The input string message.
 * @return Block The converted Block from the input string.
 */
Block convertToBlock(const string strMsg){
    Block blockMsg;
    
    for (int i = 0; i < blockMsg.size(); ++i) {
        blockMsg[i] = strMsg[i];
    }
    return blockMsg;
}

/**
 * XORs each byte of a block with the corresponding byte of a key.
 * @param block The block to be XORed.
 * @param key The key to XOR with.
 */
void xorWithKey(Block &block, const Block key){
    for(int j=0; j<block.size(); j++){
        block[j] = block[j]^key[j];
    }
}

/**
 * Substitutes each byte in a block using the corresponding substitution table.
 * @param block The block to be substituted.
 * @param substTables The substitution tables.
 */
void substByte(Block &block, const vector<vector<uint8_t>> &substTables){
    
    for(int i=0; i<block.size(); i++){
        int index = block[i];
        block[i] = substTables[i][index];
    }
}

/**
 * Rotates the bits of each byte in a block to the left by one position.
 * @param block The block to be rotated left.
 */
void rotateLeft(Block &block) {
    // Store the bit that is rotated out of the leftmost byte
    uint8_t highBit = (block[0] & 0b10000000) >> 7;
    
    for (int i = 0; i < block.size() - 1; ++i) {
        // Shift current byte left by 1 and add the high bit of the next byte
        block[i] = (block[i] << 1) | ((block[i + 1] & 0b10000000) >> 7);
    }
    
    // Handle the last byte separately
    block[block.size() - 1] = (block[block.size() - 1] << 1) | highBit;
}

/**
 * Encrypts a message using a series of cryptographic operations including XOR with a key, byte substitution, and bit rotation.
 * @param origMsg The original message to encrypt.
 * @param substTables The substitution tables used for byte substitution.
 * @param key The key used for XOR operations.
 * @return Block The encrypted message as a Block.
 */
Block encryptMessage(string origMsg, vector<vector<uint8_t>> substTables, Block key){
    
    Block message = convertToBlock(origMsg);
    
    for(int i=0; i<16; i++){
        //xor the current state with the key
        xorWithKey(message, key);
        
        //for each byte in the state, substitute that byte using the appropriate substitution table(byte 0 should use table 0, byte 1 should use table 1, etc)
        substByte(message, substTables);
       
        //rotate the entire state 1 bit to the left (so, after this, byte 0 will contain contributions from bytes 0 and 1, while byte 7 will contain contributions from byte 7 and byte 0
        rotateLeft(message);
    }
    return message;
}

/**
 * Rotates the bits of each byte in a block to the right by one position.
 * @param block The block to be rotated right.
 */
void rotateRight(Block &block) {
    // Temporary variable to store the bit that is rotated out of the rightmost byte
    uint8_t lowBit = block[block.size() - 1] & 0b00000001;
    for (int i = block.size() - 1; i > 0; --i) {
        // Shift current byte right by 1 and add the low bit of the previous byte
        block[i] = (block[i] >> 1) | ((block[i - 1] & 0b00000001) << 7);
    }
    // Handle the first byte separately
    block[0] = (block[0] >> 1) | (lowBit << 7);
}

/**
 * Reverses the byte substitution process for a block using the substitution tables.
 * @param block The block to reverse the substitution for.
 * @param substTables The substitution tables used for reversing.
 */
void reverseSubstByte(Block &block, const vector<vector<uint8_t>> substTables){
    
    for(int i=0; i<block.size(); i++){
        for(uint8_t originalValue = 0; originalValue < 256; originalValue++){
            if (substTables[i][originalValue] == block[i]) {
                block[i] = originalValue;
                // Found the original value, exit the inner loop
                break;
            }
        }
    }
}

/**
 * Converts a Block into a string message by concatenating each byte as a character.
 * @param blockMsg The Block to be converted.
 * @return string The converted string message from the Block.
 */
string convertToMessage(const Block blockMsg){
    string strMsg;
    
    for (int i = 0; i < blockMsg.size(); ++i) {
        strMsg.push_back(blockMsg[i]);
    }
    return strMsg;
}

/**
 * Decrypts an encrypted message using the reverse of the encryption operations including bit rotation, byte substitution reversal, and XOR with a key.
 * @param encryptMsg The encrypted message to decrypt.
 * @param substTables The substitution tables used for byte substitution reversal.
 * @param key The key used for XOR operations.
 * @return string The decrypted message.
 */
string decryptMessage(Block encryptMsg, vector<vector<uint8_t>> substTables, Block key){
    
    for(int i=0; i<16; i++){
        
        rotateRight(encryptMsg);
        
        reverseSubstByte(encryptMsg, substTables);
        
        xorWithKey(encryptMsg, key);
        
    }
    
    string message = convertToMessage(encryptMsg);
    
    return message;
}


int main(int argc, const char * argv[]) {
    
    string password = "Howdy";
    
    Block key = generateKey(password);
    
    vector<vector<uint8_t>> substTables;
    
    initializeTables(substTables);
    
    string origMsg = "Hello";
    
    cout << "Message before encryption: " << origMsg << "\n";
    
    Block encryptedMsg = encryptMessage(origMsg, substTables, key);
    
    string decryptedMsg = decryptMessage(encryptedMsg, substTables, key);
    
    cout << "Message after decryption: " << decryptedMsg << "\n";
    
    return 0;
}
