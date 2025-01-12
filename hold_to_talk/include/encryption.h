#ifndef ENCRYPTION_H
#define ENCRYPTION_H

void setupEncryption();
void encryptAudio(unsigned char* audioData, unsigned char* encryptedData, size_t length);
void decryptAudio(unsigned char* encryptedData, unsigned char* decryptedData, size_t length);

#endif

