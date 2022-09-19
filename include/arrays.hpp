#ifndef __LAP_UTILS_ARRAYS
#define __LAP_UTILS_ARRAYS
#include <set>
#include <tuple>

template <typename T>
void print(const T **matrix, int dim, int width);
template <typename T>
void highlighted_print(const T **matrix, int dim, const std::set<std::tuple<int, int>> highlight, int width);
template <typename T>
void print(const T *array, int dim, int width);

template <typename T>
void fill_random(T **matrix, int dim, T max_val, T min_val);

#endif
