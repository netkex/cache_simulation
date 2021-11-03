#include <iostream>
#include "Memory.hpp"
#include "Array.hpp"
#include "f.hpp"

#define L1_way 4
#define L1_line_size 64
#define L1_size (32 * (1 << 10))
#define L2_way 8
#define L2_line_size 64
#define L2_size (1 << 20)

const int size = 1024 * 1024;

int main() {
    RAM ram(10 * size * sizeof (double));
    Cache L2(&ram, L2_size, L2_way, L2_line_size);
    Cache L1(&L2, L1_size, L1_way, L1_line_size);

    Array<double> x(&L1, size, 0);
    Array<double> y(&L1, size, x.end());
    Array<double> z(&L1, size, y.end());
    Array<double> xx(&L1, size, z.end());
    Array<double> yy(&L1, size, xx.end());
    Array<double> zz(&L1, size, yy.end());

    L1.zero_stats(), L2.zero_stats();
    double w = 5;
    f(x, y, z, xx, yy, zz, size, w);

    std::cout << "L1 successful: " <<
        (1 - L1.get_missed_operations() * 1. / L1.get_all_operations()) * 100 << "%" << std::endl;
    std::cout << "L2 successful: " <<
        (1 - L2.get_missed_operations() * 1. / L2.get_all_operations()) * 100 << "%" << std::endl;
    return 0;
}
