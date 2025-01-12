#include "audio_handler.h"
#include <portaudio.h>
#include <iostream>

// Constants for audio capture
#define SAMPLE_RATE (44100)
#define FRAMES_PER_BUFFER (256)
#define NUM_CHANNELS (1)

// PortAudio stream pointer
static PaStream* stream = nullptr;

// Audio capture callback function
static int audioCallback(const void* inputBuffer, void* outputBuffer,
                         unsigned long framesPerBuffer,
                         const PaStreamCallbackTimeInfo* timeInfo,
                         PaStreamCallbackFlags statusFlags, void* userData) {
    // Copy the captured audio from the input buffer to the provided audio buffer
    float* inBuffer = (float*)inputBuffer;
    float* outBuffer = (float*)outputBuffer;
    std::memcpy(outBuffer, inBuffer, framesPerBuffer * sizeof(float));
    return paContinue;
}

// Initialize PortAudio for audio capture
void initAudio() {
    PaError err = Pa_Initialize();
    if (err != paNoError) {
        std::cerr << "PortAudio error: " << Pa_GetErrorText(err) << std::endl;
        exit(1);
    }

    err = Pa_OpenDefaultStream(&stream, NUM_CHANNELS, NUM_CHANNELS, paFloat32, SAMPLE_RATE,
                               FRAMES_PER_BUFFER, audioCallback, nullptr);
    if (err != paNoError) {
        std::cerr << "PortAudio error: " << Pa_GetErrorText(err) << std::endl;
        exit(1);
    }

    err = Pa_StartStream(stream);
    if (err != paNoError) {
        std::cerr << "PortAudio error: " << Pa_GetErrorText(err) << std::endl;
        exit(1);
    }
}

// Capture audio into the provided buffer
void captureAudio(float* audioBuffer, int numFrames) {
    PaError err = Pa_ReadStream(stream, audioBuffer, numFrames);
    if (err != paNoError) {
        std::cerr << "PortAudio error: " << Pa_GetErrorText(err) << std::endl;
        exit(1);
    }
}

// Play audio from the provided buffer
void playAudio(float* audioBuffer, int numFrames) {
    PaError err = Pa_WriteStream(stream, audioBuffer, numFrames);
    if (err != paNoError) {
        std::cerr << "PortAudio error: " << Pa_GetErrorText(err) << std::endl;
        exit(1);
    }
}

// Clean up PortAudio resources
void cleanupAudio() {
    Pa_StopStream(stream);
    Pa_CloseStream(stream);
    Pa_Terminate();
}

