#include <array>
#include <vector>
#include <string>
#include <cstdint>
#include <sstream>
#include <iomanip>
#include <iostream>

#include "headers/sha256.h"
#include "headers/constants.h"
#include "headers/bitfunctions.h"

namespace CONST_SHA = shaConstants;

std::vector<uint8_t> padding(std::vector<uint8_t> message) {

    uint64_t message_bits_len = static_cast<uint64_t>(message.size()) * 8;

    message.push_back(0x80);
    while (message.size() % 64 != 56) {
        message.push_back(0x00);
    }

    for (int i = 7; i >= 0; i--) {
        message.push_back(static_cast<uint8_t>((message_bits_len >> (i * 8)) & 0xFF));
    }

    return message;
}

std::string sha256(std::string message) {

    std::vector<uint8_t> message_bytes(message.begin(), message.end());
    message_bytes = padding(message_bytes);

    std::array<uint32_t, 8> hashes = CONST_SHA::H;
    std::array<uint32_t, 64> k = CONST_SHA::K;

    for (int chunk_start = 0; chunk_start < message_bytes.size(); chunk_start += 64) {
        std::vector<uint8_t> chunk(message_bytes.begin() + chunk_start, message_bytes.begin() + chunk_start + 64);
        std::vector<uint32_t> w(64, 0);

        for (int i = 0; i < 16; i++) {
            w[i] = (static_cast<uint32_t>(chunk[i * 4]) << 24) |
            (static_cast<uint32_t>(chunk[i * 4 + 1]) << 16) |
            (static_cast<uint32_t>(chunk[i * 4 + 2]) << 8) |
            (static_cast<uint32_t>(chunk[i * 4 + 3]));
        }

        for (int i = 16; i < 64; i++) {
            w[i] = σ1(w[i - 2]) + w[i - 7] + σ0(w[i - 15]) + w[i - 16];
        }

        uint32_t a = hashes[0], b = hashes[1], c = hashes[2], d = hashes[3];
        uint32_t e = hashes[4], f = hashes[5], g = hashes[6], h = hashes[7];

        for (int i = 0; i < 64; i++) {
            uint32_t T1 = h + Σ1(e) + ch(e, f, g) + k[i] + w[i];
            uint32_t T2 = Σ0(a) + maj(a, b, c);
            h = g;
            g = f;
            f = e;
            e = d + T1;
            d = c;
            c = b;
            b = a;
            a = T1 + T2;
        }

        hashes[0] += a;
        hashes[1] += b;
        hashes[2] += c;
        hashes[3] += d;
        hashes[4] += e;
        hashes[5] += f;
        hashes[6] += g;
        hashes[7] += h;
    }

    std::stringstream ss;
    for (uint32_t h : hashes) {
        ss << std::hex << std::setfill('0') << std::setw(8) << h;
    }

    return ss.str();
}
