#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <cstring>
#include <chrono>
#include <unistd.h>
#include <arpa/inet.h>
#include <openssl/aes.h>
#include "audio_handler.h"
#include "encryption.h"

#define PORT 12345  // Port for the server
#define MAX_CLIENTS 10

std::vector<int> clients; // List of client sockets
std::mutex clientsMutex;  // Mutex for thread-safe access to the clients list

// Function to handle client communication
void handleClient(int clientSocket) {
    std::cout << "Client connected: " << clientSocket << std::endl;

    unsigned char audioBuffer[FRAMES_PER_BUFFER * sizeof(float)];
    unsigned char encryptedAudio[FRAMES_PER_BUFFER * sizeof(float)];

    // Receive and broadcast audio
    while (true) {
        // Capture audio from client
        int bytesReceived = recv(clientSocket, audioBuffer, sizeof(audioBuffer), 0);
        if (bytesReceived <= 0) {
            std::cout << "Client disconnected: " << clientSocket << std::endl;
            break;
        }

        // Encrypt the audio data
        unsigned char encryptedData[sizeof(audioBuffer)];
        encryptAudio(audioBuffer, encryptedData, sizeof(audioBuffer));

        // Broadcast to all clients
        std::lock_guard<std::mutex> lock(clientsMutex);
        for (int client : clients) {
            if (client != clientSocket) {  // Don't send the audio back to the sender
                send(client, encryptedData, sizeof(encryptedData), 0);
            }
        }
    }

    close(clientSocket);  // Close client connection
}

// Function to initialize server and accept connections
void startServer() {
    int serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t addrLen = sizeof(clientAddr);

    // Set up server socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        std::cerr << "Server socket creation failed!" << std::endl;
        return;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);

    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        std::cerr << "Binding failed!" << std::endl;
        return;
    }

    if (listen(serverSocket, MAX_CLIENTS) == -1) {
        std::cerr << "Listening failed!" << std::endl;
        return;
    }

    std::cout << "Server started, waiting for connections..." << std::endl;

    while (true) {
        clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &addrLen);
        if (clientSocket == -1) {
            std::cerr << "Client connection failed!" << std::endl;
            continue;
        }

        // Add client to the list of connected clients
        {
            std::lock_guard<std::mutex> lock(clientsMutex);
            clients.push_back(clientSocket);
        }

        // Handle each client in a separate thread
        std::thread(handleClient, clientSocket).detach();
    }

    close(serverSocket);
}

int main() {
    // Set up AES encryption
    setupEncryption();

    // Initialize audio system
    initAudio();

    // Start server to accept client connections
    startServer();

    // Clean up audio system
    cleanupAudio();

    return 0;
}

