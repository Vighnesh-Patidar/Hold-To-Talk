
# Hold-to-Talk Audio Communication System

## Overview
This project provides a real-time audio communication system where users can participate in a voice call by connecting to a server. The server acts as a central hub that allows clients to listen and transmit audio to each other. The communication is encrypted using AES-256 to ensure privacy, and the system is designed to work over local networks.

## Features

- **Real-Time Audio Streaming**: Audio from all clients is captured and broadcasted to other clients with minimal latency.
- **AES-256 Encryption**: Audio data is encrypted to prevent unauthorized access during transmission.
- **Client-Server Architecture**: One machine acts as the server, while others act as clients that connect to the server and participate in the audio call.
- **Log Management**: The server logs the connection and disconnection times, along with the user ID of each participant.
- **Multi-Client Support**: Multiple clients can join the call, and their audio will be broadcasted to all other participants.

## Requirements

### Software
- **C++ Compiler**: A C++ compiler (e.g., GCC or Clang) to build the application.
- **PortAudio**: A library for cross-platform audio input and output. It is used to capture and play audio.
- **OpenSSL**: A library for AES-256 encryption, used for encrypting and decrypting the audio data.
  
### Hardware
- **Microphone**: A microphone for capturing audio from the client side.
- **Speakers/Headphones**: Speakers or headphones for playback of the received audio.

## Installation

### 1. Clone the repository
Clone the project repository to your local machine:
```bash
git clone https://github.com/your-repository/hold-to-talk.git
cd hold-to-talk
```

### 2. Install dependencies
Make sure you have PortAudio and OpenSSL installed. You can install them using package managers like `apt` on Linux or `brew` on macOS.

For Linux:
```bash
sudo apt-get install libportaudio-dev libssl-dev
```

For macOS:
```bash
brew install portaudio openssl
```

### 3. Build the Project
Use the provided `Makefile` to compile both the server and client applications:
```bash
make
```

## Usage

### Running the Server

1. On the machine that will act as the server, navigate to the directory containing the compiled `server` executable.
2. Run the server with the following command:
   ```bash
   ./server
   ```
   The server will start and listen for incoming connections on port `12345`. The server will log connection times and disconnection times for each client.

3. **Note down the IP address** of the server machine (e.g., `192.168.x.x`). This IP address will be required for clients to connect.

### Running the Client

1. On the client machines, modify the `SERVER_IP` in `main_client.cpp` to the IP address of the server machine:
   ```cpp
   #define SERVER_IP "192.168.x.x"  // Replace with the server's IP address
   ```

2. Compile the client with:
   ```bash
   make
   ```

3. Run the client application with:
   ```bash
   ./client
   ```

4. Multiple clients can run simultaneously. Each client will connect to the server, send audio, and listen to the audio from other clients.

### Disconnecting
- When a client disconnects, the server will log the disconnect time and the client’s ID, and the server will stop receiving audio from that client.

## File Structure

```plaintext
├── Makefile                  # Build instructions for server and client
├── main_server.cpp           # Server-side code for handling connections and broadcasting audio
├── main_client.cpp           # Client-side code for capturing and playing audio
├── audio_handler.cpp         # Functions to capture and play audio using PortAudio
├── audio_handler.h           # Header file for audio capture/playback functions
├── encryption.cpp            # AES-256 encryption functions for securing audio data
├── encryption.h              # Header file for encryption functions
└── README.md                 # Documentation for the project
```

## Networking Details

### Server:
- The server listens for incoming connections on **PORT 12345** (configurable in the code).
- The server can accept up to **10 clients** simultaneously.
- The server logs the **connection time** and **disconnection time** for each client.

### Client:
- Clients connect to the server using a TCP socket.
- Clients send captured audio data to the server and receive broadcasted audio data from the server.

### Port and IP Configuration:
- **Port**: `12345` (configurable in the source code).
- **IP Address**: Must be provided manually in the client application (via the `SERVER_IP` variable).

**Important**: Ensure that the server’s port is open and accessible from the client’s network. For local networks, ensure the server and clients are on the same network. For public IPs, ensure proper **port forwarding** and firewall configuration.

## Encryption Overview

### AES-256 Encryption:
- **AES-256** is used to encrypt audio data before transmission and decrypt it upon receipt. This ensures that even if the data is intercepted, it cannot be read without the correct decryption key.
  
- The server and client use the same **encryption key** (`encryptionKey`), which is currently hardcoded in the source code.

  - **Encryption Process**: 
    - Audio data is captured from the microphone as raw bytes.
    - This data is encrypted using AES-256 in **ECB mode**.
    - The encrypted audio is then transmitted to the server or clients.
  
  - **Decryption Process**:
    - Upon receiving the encrypted data, the client or server decrypts it using the same key, and the audio data is played back.

### Key Setup:
The encryption key is defined as:
```cpp
unsigned char encryptionKey[32] = {0x00};  // AES-256 key
```
For better security, you should change this key to a randomly generated 256-bit key.

## Client-Server Communication

The communication is built using **TCP sockets**, and data transmission works as follows:
- **Client to Server**:
  - The client captures audio data, encrypts it using AES-256, and sends it to the server.
- **Server to Clients**:
  - The server receives the encrypted audio from each client and broadcasts it to all other clients after encryption.
- **Real-time Audio**:
  - Audio is played with low latency. The server continuously streams encrypted audio data to clients.

## Licensing

This project is licensed under the MIT License. See the LICENSE file for more details.
