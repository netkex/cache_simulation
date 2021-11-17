#include <vector>
#include <stdint.h>
#include <cstddef>

#ifndef CACHE_SIM_MEMORY_HPP
#define CACHE_SIM_MEMORY_HPP


class Memory {
public:
    virtual std::vector<std::uint8_t> read_bytes(std::size_t address, std::size_t num, bool count_op_flag) = 0;
    virtual void write_bytes(std::size_t address, std::vector<std::uint8_t>& bytes, bool count_op_flag) = 0;
};

class RAM : public Memory {
public:
    explicit RAM(std::size_t _size);
    std::vector<std::uint8_t> read_bytes(std::size_t address, std::size_t num, bool count_op_flag) override;
    void write_bytes(std::size_t address, std::vector<std::uint8_t>& bytes, bool count_op_flag) override;

private:
    std::vector<std::size_t> data;
    std::size_t size;
};

class Cache : public Memory {
public:
    Cache (Memory* _parent_memory, std::size_t _size, std::size_t _way, std::size_t _line_size);
    std::vector<std::uint8_t> read_bytes(std::size_t address, std::size_t num, bool count_op_flag) override;
    void write_bytes(std::size_t address, std::vector<std::uint8_t>& bytes, bool count_op_flag) override;

    void zero_stats();
    int get_missed_operations();
    int get_all_operations();

private:
    std::uint8_t& get_byte(std::size_t address, bool count_flag);
    void load_bytes(std::size_t address, bool count_flag);
    std::size_t get_block_num(std::size_t address);
    std::size_t get_tag(std::size_t address);
    std::size_t get_address(std::size_t tag, std::size_t block_num);


    std::size_t line_size, way, size, log_line_size, log_blocks_number;
    std::vector<std::vector<std::vector<std::uint8_t>>> lines;
    std::vector<std::vector<std::size_t>> tags;
    std::vector<std::vector<int>> last_used;
    Memory* parent_memory;
    int T = 0;
    int missed_operations = 0;
    int all_operations = 0;
};
#endif //CACHE_SIM_MEMORY_HPP
