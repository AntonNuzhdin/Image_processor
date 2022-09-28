#pragma once

#include <fstream>

template <typename T>
void ReadNum(std::ifstream& in, T& val) {
    uint8_t buff[sizeof(val)];
    in.read(reinterpret_cast<char*>(buff), sizeof(val));
    for (size_t i = 0; i < sizeof(val); i++) {
        val |= buff[i] << (8 * i);
    }
}

template <typename T>
T ReadNum(std::ifstream& in) {
    T val;
    ReadNum(in, val);
    return val;
}

template <typename T>
void WriteNum(std::ofstream& out, const T& val) {
    for (size_t i = 0; i < sizeof(val); ++i) {
        out.put(val >> (8 * i) & 0xFF);
    }
}
