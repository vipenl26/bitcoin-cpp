//
//  sha256.h
//  bitcoin-cpp
//
//  Created by Vipen Loka on 10/11/24.
//


#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>

class SHA256 {
public:
    SHA256() { reset(); }

    SHA256& update(const uint8_t* data, size_t length) {
        for (size_t i = 0; i < length; ++i) {
            dataBuffer[dataLength++] = data[i];
            if (dataLength == BLOCK_SIZE) {
                processBlock();
                bitLength += BLOCK_SIZE * 8;
                dataLength = 0;
            }
        }
        return *this;
    }

    SHA256& update(const std::string& data) {
        return update(reinterpret_cast<const uint8_t*>(data.c_str()), data.size());
    }

    std::string final() {
        bitLength += dataLength * 8;
        dataBuffer[dataLength++] = 0x80;

        if (dataLength > 56) {
            while (dataLength < BLOCK_SIZE) {
                dataBuffer[dataLength++] = 0x00;
            }
            processBlock();
            dataLength = 0;
        }

        while (dataLength < 56) {
            dataBuffer[dataLength++] = 0x00;
        }

        for (int i = 7; i >= 0; --i) {
            dataBuffer[dataLength++] = static_cast<uint8_t>((bitLength >> (i * 8)) & 0xFF);
        }
        processBlock();

        std::ostringstream oss;
        for (uint32_t word : state) {
            oss << std::hex << std::setw(8) << std::setfill('0') << word;
        }

        reset();
        return oss.str();
    }

private:
    static constexpr size_t BLOCK_SIZE = 64;
    static constexpr uint32_t K[64] = {
        0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
        0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
        0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
        0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
        0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
        0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
        0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
        0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
    };

    uint32_t state[8];
    uint8_t dataBuffer[BLOCK_SIZE];
    size_t dataLength;
    uint64_t bitLength;

    void reset() {
        state[0] = 0x6a09e667; state[1] = 0xbb67ae85; state[2] = 0x3c6ef372; state[3] = 0xa54ff53a;
        state[4] = 0x510e527f; state[5] = 0x9b05688c; state[6] = 0x1f83d9ab; state[7] = 0x5be0cd19;
        dataLength = 0;
        bitLength = 0;
    }

    void processBlock() {
        uint32_t M[64];
        for (size_t i = 0; i < 16; ++i) {
            M[i] = (dataBuffer[i * 4] << 24) | (dataBuffer[i * 4 + 1] << 16) |
                   (dataBuffer[i * 4 + 2] << 8) | dataBuffer[i * 4 + 3];
        }
        for (size_t i = 16; i < 64; ++i) {
            M[i] = sigma1(M[i - 2]) + M[i - 7] + sigma0(M[i - 15]) + M[i - 16];
        }

        uint32_t a = state[0], b = state[1], c = state[2], d = state[3];
        uint32_t e = state[4], f = state[5], g = state[6], h = state[7];

        for (size_t i = 0; i < 64; ++i) {
            uint32_t T1 = h + Sigma1(e) + ch(e, f, g) + K[i] + M[i];
            uint32_t T2 = Sigma0(a) + maj(a, b, c);
            h = g; g = f; f = e; e = d + T1;
            d = c; c = b; b = a; a = T1 + T2;
        }

        state[0] += a; state[1] += b; state[2] += c; state[3] += d;
        state[4] += e; state[5] += f; state[6] += g; state[7] += h;
    }

    static uint32_t rotr(uint32_t x, uint32_t n) { return (x >> n) | (x << (32 - n)); }
    static uint32_t ch(uint32_t x, uint32_t y, uint32_t z) { return (x & y) ^ (~x & z); }
    static uint32_t maj(uint32_t x, uint32_t y, uint32_t z) { return (x & y) ^ (x & z) ^ (y & z); }
    static uint32_t Sigma0(uint32_t x) { return rotr(x, 2) ^ rotr(x, 13) ^ rotr(x, 22); }
    static uint32_t Sigma1(uint32_t x) { return rotr(x, 6) ^ rotr(x, 11) ^ rotr(x, 25); }
    static uint32_t sigma0(uint32_t x) { return rotr(x, 7) ^ rotr(x, 18) ^ (x >> 3); }
    static uint32_t sigma1(uint32_t x) { return rotr(x, 17) ^ rotr(x, 19) ^ (x >> 10); }
}sha256;


static string compute_sha256(string input) {
    return sha256.update(input).final();
}
