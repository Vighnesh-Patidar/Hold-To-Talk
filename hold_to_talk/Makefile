# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -std=c++17 -I./include

# Linker flags for external libraries (PortAudio and ncurses)
LDFLAGS = -lportaudio -lncurses -lssl -lcrypto

# Directory paths
SRC_DIR = src
BUILD_DIR = build
BIN_DIR = bin
INCLUDE_DIR = include

# Source and object files
SERVER_SRC = $(SRC_DIR)/main_server.cpp $(SRC_DIR)/audio_handler.cpp $(SRC_DIR)/encryption.cpp
CLIENT_SRC = $(SRC_DIR)/main_client.cpp $(SRC_DIR)/audio_handler.cpp $(SRC_DIR)/encryption.cpp

SERVER_OBJ = $(SERVER_SRC:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)
CLIENT_OBJ = $(CLIENT_SRC:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

# Output executables
SERVER_EXEC = $(BIN_DIR)/server
CLIENT_EXEC = $(BIN_DIR)/client

# Create directories if they don't exist
$(shell mkdir -p $(BUILD_DIR) $(BIN_DIR))

# Default target: build both the server and client
all: $(SERVER_EXEC) $(CLIENT_EXEC)

# Compile and link the server
$(SERVER_EXEC): $(SERVER_OBJ)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^

# Compile and link the client
$(CLIENT_EXEC): $(CLIENT_OBJ)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^

# Compile each source file into an object file
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up build and binary directories
clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

# Phony targets (not actual files)
.PHONY: all clean

