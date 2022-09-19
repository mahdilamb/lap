#ifndef __LAPJV_CSV_HPP
#define __LAPJV_CSV_HPP
#include <string>
#include <functional>

template <typename T>
int read_csv(T **&mat, const std::string &path, const char sep, std::function<T(const std::string &)> from_string);

#endif
