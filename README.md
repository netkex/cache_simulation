Симуляция работы кэша
======

Изменения
____

После лекции уточнил формулу, и что при подсчете статистики не нужно считать выгрузку из кэша
за операцию, 
поэтому внес незначительные изменения.

Способ сборки
____

Проект может быть собран или при помощи *cmake*, или командой 
```
g++ -std=c++17 -I include/ src/main.cpp src/f.cpp src/Memory.cpp -O2 -o cache_sim
```