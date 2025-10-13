#include "IObit.h"
#include <sstream>
#include <fstream>
#include <cstdint>

void IOBit::write_uint32(std::ostream& os, uint32_t value) {
    os.put((value >> 24) & 0xFF);
    os.put((value >> 16) & 0xFF);
    os.put((value >> 8) & 0xFF);
    os.put(value & 0xFF);
}

uint32_t IOBit::read_uint32(std::istream& is) {
    uint32_t value = 0;
    value |= (uint32_t)(is.get()) << 24;
    value |= (uint32_t)(is.get()) << 16;
    value |= (uint32_t)(is.get()) << 8;
    value |= (uint32_t)(is.get());
    return value;
}

void IOBit::write_string(std::ostream& os, const std::string& str) {
    uint32_t length = str.length();
    write_byte_aligned(os, length);
    os.write(str.c_str(), length);
}

std::string IOBit::read_string(std::istream& is) {
    uint32_t length = read_byte_aligned(is);
    std::string result(length, 0);
    is.read(&result[0], length);
    return result;
}

void IOBit::write_byte_aligned(std::ostream &os, uint32_t value) {
    if (value < (1 << 6)) {
        // 00xxxxxx
        os.put((uint8_t)(value));
    } else if (value < (1 << 14)) {
        // 01xxxxxx xxxxxxxx
        os.put((uint8_t)(0x40 | (value >> 8)));
        os.put((uint8_t)(value & 0xFF));
    } else if (value < (1 << 22)) {
        // 10xxxxxx xxxxxxxx xxxxxxxx
        os.put((uint8_t)(0x80 | (value >> 16)));
        os.put((uint8_t)((value >> 8) & 0xFF));
        os.put((uint8_t)(value & 0xFF));
    } else {
        // 11xxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
        os.put((uint8_t)(0xC0 | (value >> 24)));
        os.put((uint8_t)((value >> 16) & 0xFF));
        os.put((uint8_t)((value >> 8) & 0xFF));
        os.put((uint8_t)(value & 0xFF));
    }
}

uint32_t IOBit::read_byte_aligned(std::istream &is) {
    uint8_t first_byte = is.get();
    
    if ((first_byte & 0xC0) == 0x00) {
        // 00xxxxxx
        return first_byte & 0x3F;
    } else if ((first_byte & 0xC0) == 0x40) {
        // 01xxxxxx xxxxxxxx
        uint8_t second_byte = is.get();
        return ((first_byte & 0x3F) << 8) | second_byte;
    } else if ((first_byte & 0xC0) == 0x80) {
        // 10xxxxxx xxxxxxxx xxxxxxxx
        uint8_t second_byte = is.get();
        uint8_t third_byte = is.get();
        return ((first_byte & 0x3F) << 16) | (second_byte << 8) | third_byte;
    } else {
        // 11xxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
        uint8_t second_byte = is.get();
        uint8_t third_byte = is.get();
        uint8_t fourth_byte = is.get();
        return ((first_byte & 0x3F) << 24) | (second_byte << 16) | (third_byte << 8) | fourth_byte;
    }
}