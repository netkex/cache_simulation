#include "f.hpp"
#include <cstddef>

void f(Array<double>& x,
       Array<double>& y,
       Array<double>& z,
       Array<double>& xx,
       Array<double>& yy,
       Array<double>& zz,
       std::size_t size,
       double w) {
    for (std::size_t i = 0; i < size; i++) {
        x.write(i, xx.read(i) * w + x.read(i));
    }
    for (std::size_t i = 0; i < size; i++) {
        y.write(i, yy.read(i) * w + y.read(i));
    }
    for (std::size_t i = 0; i < size; i++) {
        z.write(i, zz.read(i) + w * z.read(i));
    }
}