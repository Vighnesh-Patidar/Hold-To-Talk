#include <iostream>
#include <cstring>
#include <thread>
#include <unistd.h>
#include <arpa/inet.h>
#include <openssl/aes.h>
#include "audio_handler.h"
#include "encryption.h"

#define SERVER_IP "127.0.0.1"  // Replace with the server IP address
#define SERVER_PORT 12345      // Server port

// Function to receive and play audio
void receiveAudio(int serverSocket) {
    unsigned char encryptedAudio[FRAMES_PER_BUFFER * sizeof(float)];
    unsigned char decryptedAudio[FRAMES_PER_BUFFER * sizeof(float)];

    while (true) {
        int bytesReceived = recv(serverSocket, encryptedAudio, sizeof(encryptedAudio), 0);
        if (bytesReceived <= 0) {
            std::cerr << "Server disconnected!" << std::endl;
            break;
        }

        // Decrypt the received audio data
        decryptAudio(encryptedAudio, decryptedAudio, sizeof(encryptedAudio));

        // Play the decrypted audio
        playAudio((float*)decryptedAudio, FRAMES_PER_BUFFER);
    }

    close(serverSocket);
}

// Function to capture and send audio to the server
void sendAudio(int serverSocket) {
    unsigned char audioBuffer[FRAMES_PER_BUFFER * sizeof(float)];
    unsigned char encryptedAudio[FRAMES_PER_BUFFER * sizeof(float)];

    while (true) {
        // Capture audio
        captureAudio((float*)audioBuffer, FRAMES_PER_BUFFER);

        // Encrypt the captured audio
        encryptAudio(audioBuffer, encryptedAudio, sizeof(audioBuffer));

        // Send encrypted audio to server
        send(serverSocket, encryptedAudio, sizeof(encryptedAudio), 0);
    }
}

int main() {
    // Set up AES encryption
    setupEncryption();

    // Initialize audio system
    initAudio();

    // Set up server socket for connection
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        std::cerr << "Socket creation failed!" << std::endl;
        return -1;
    }

    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP);

    // Connect to the server
    if (connect(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        std::cerr << "Connection to server failed!" << std::endl;
        return -1;
    }

    // Start receiving and sending audio in separate threads
    std::thread(receiveAudio, serverSocket).detach();
    sendAudio(serverSocket);

    // Clean up audio system
    cleanupAudio();

    return 0;
}

