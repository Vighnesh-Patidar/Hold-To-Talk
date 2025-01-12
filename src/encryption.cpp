#include "encryption.h"
#include <openssl/aes.h>
#include <cstring>

AES_KEY encrypt_key, decrypt_key;
unsigned char key[32] = {0}; // AES key (32 bytes for AES-256)

// Set up AES-256 encryption and decryption keys
void setupEncryption() {
    // Use a fixed key for AES encryption (you can replace this with a secure key exchange)
    unsigned char fixedKey[32] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
                                  0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10,
                                  0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18,
                                  0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x20};
    std::memcpy(key, fixedKey, 32);
    AES_set_encrypt_key(key, 256, &encrypt_key);
    AES_set_decrypt_key(key, 256, &decrypt_key);
}

// Encrypt audio data
void encryptAudio(unsigned char* audioData, unsigned char* encryptedData, size_t length) {
    AES_encrypt(audioData, encryptedData, &encrypt_key);
}

// Decrypt audio data
void decryptAudio(unsigned char* encryptedData, unsigned char* decryptedData, size_t length) {
    AES_decrypt(encryptedData, decryptedData, &decrypt_key);
}

