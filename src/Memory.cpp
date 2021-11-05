#include "Memory.hpp"


RAM::RAM(std::size_t _size): size(_size) {
    data = std::vector<std::size_t> (size + 1, 0);
}

std::vector<std::uint8_t> RAM::read_bytes(std::size_t address, std::size_t num) {
    return {&data[address], &data[address + num]};
}

void RAM::write_bytes(std::size_t address, std::vector<std::uint8_t> &bytes) {
    for (std::size_t i = 0; i < bytes.size(); i++) {
        data[address + i] = bytes[i];
    }
}


Cache::Cache(Memory *_parent_memory, std::size_t _size, std::size_t _way, std::size_t _line_size) :
    line_size(_line_size), way(_way), size(_size), parent_memory(_parent_memory)
{
    std::size_t blocks_number = size / (way * line_size);
    lines = std::vector<std::vector<std::vector<std::uint8_t>>> (blocks_number,
                                                                 std::vector<std::vector<std::uint8_t>> (way,
                                                                         std::vector<std::uint8_t> (line_size, 0)));
    tags = std::vector<std::vector<std::size_t>> (blocks_number, std::vector<std::size_t> (way, 0));
    last_used = std::vector<std::vector<int>> (blocks_number,
            std::vector<int> (way, -1)); // -1 shows that cache was not used yet
    log_line_size = __builtin_ctz(line_size);
    log_blocks_number = __builtin_ctz(blocks_number);
}

std::vector<std::uint8_t> Cache::read_bytes(std::size_t address, std::size_t num) {
    all_operations++; // based on fact that query always fits in one cache line
    std::vector<std::uint8_t> ans(num, 0);
    for (std::size_t i = 0; i < num; i++) {
        ans[i] = get_byte(address + i);
    }
    return ans;
}

void Cache::write_bytes(std::size_t address, std::vector<std::uint8_t> &bytes) {
    all_operations++; // based on fact that query always fits in one cache line
    for (std::size_t i = 0; i < bytes.size(); i++) {
        get_byte(address + i) = bytes[i];
    }
}

void Cache::zero_stats() {
    missed_operations = all_operations = 0;
}

int Cache::get_missed_operations() { return missed_operations; }

int Cache::get_all_operations() { return all_operations; }

std::uint8_t& Cache::get_byte(std::size_t address) {
    auto block_num = get_block_num(address);
    auto address_tag = get_tag(address);
    for (std::size_t j = 0; j < way; j++) {
        if (tags[block_num][j] == address_tag && last_used[block_num][j] != -1) {
            last_used[block_num][j] = T++;
            return lines[block_num][j][address % (1 << log_line_size)];
        }
    }
    load_bytes(address >> log_line_size << log_line_size);
    return get_byte(address);
}

void Cache::load_bytes(std::size_t address) {
    missed_operations++;
    auto block_num = get_block_num(address);
    auto bytes = parent_memory->read_bytes(address, line_size);
    for (std::size_t j = 0; j < way; j++) {
        if (last_used[block_num][j] == -1) {
            tags[block_num][j] = get_tag(address);
            lines[block_num][j] = bytes;
            last_used[block_num][j] = T++;
            return;
        }
    }
    std::size_t latest_line = 0;
    for (std::size_t j = 0; j < way; j++) {
        if (last_used[block_num][j] < last_used[block_num][latest_line])
            latest_line = j;
    }
    parent_memory->write_bytes(get_address(tags[block_num][latest_line], block_num),
                               lines[block_num][latest_line]); // write changed data back in memory
    tags[block_num][latest_line] = get_tag(address);
    lines[block_num][latest_line] = bytes;
    last_used[block_num][latest_line] = T++;
}

std::size_t Cache::get_block_num(std::size_t address) {
    std::size_t  cut_address = address >> log_line_size;
    return cut_address ^ (cut_address >> log_blocks_number << log_blocks_number);
}

std::size_t Cache::get_tag(std::size_t address) {
    return address >> (log_blocks_number + log_line_size);
}

std::size_t Cache::get_address(std::size_t tag, std::size_t block_num) {
    return (tag << (log_line_size + log_blocks_number)) + (block_num  << log_line_size);
}

