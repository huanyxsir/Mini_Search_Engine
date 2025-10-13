#pragma once
#include <sstream>
#include <fstream>
#include <cstdint>


namespace IOBit {
    void write_uint32(std::ostream& os, uint32_t value);
    uint32_t read_uint32(std::istream& is);
    void write_string(std::ostream& os, const std::string& str);
    std::string read_string(std::istream& is);
    void write_byte_aligned(std::ostream &os, uint32_t value);
    uint32_t read_byte_aligned(std::istream &is);
}