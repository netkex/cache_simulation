#include "Memory.hpp"

#ifndef CACHE_SIM_ARRAY_HPP
#define CACHE_SIM_ARRAY_HPP

template<typename T>
class Array {
public:
    Array(Memory* _memory, std::size_t  _size, std::size_t _offset) :
        memory(_memory), size(_size), offset(_offset) {}
    T read(std::size_t id) {
        auto bytes = memory->read_bytes(offset + id * sofT, sofT, true);
        T ans;
        for (std::size_t i = 0; i < sofT; i++) {
            std::memcpy((std::uint8_t *)&ans + i, &(bytes[i]), 1);
        }
        return ans;
    }
    void write(std::size_t id, T value) {
        std::vector<std::uint8_t> bytes(sofT, 0);
        for (std::size_t i = 0; i < sofT; i++) {
            std::memcpy(&(bytes[i]), (std::uint8_t *)&value + i, 1);
        }
        memory->write_bytes(offset + id * sofT, bytes, true);
    }
    std::size_t begin() {
        return offset;
    }
    std::size_t end() {
        return offset + size * sofT;
    }

private:
    Memory* memory;
    std::size_t  size;
    std::size_t  offset;
    const std::size_t  sofT = sizeof(T);
};

#endif //CACHE_SIM_ARRAY_HPP
