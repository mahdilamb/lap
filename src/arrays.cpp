#include "arrays.hpp"

#include <iostream>
#include <iomanip>
#include <map>
#include <string>
#include <random>
#include <functional>

template <typename T>
void print(const T **matrix, int dim, int width)
{
    width = width <= 0 ? 4 : width;
    std::cout << "[";
    for (int i = 0; i < dim; i++)
    {
        if (i)
        {
            std::cout << " ";
        }

        std::cout << "[ ";
        for (int j = 0; j < dim; j++)
            std::cout << std::setw(width) << matrix[i][j] << (j != dim - 1 ? "," : "")
                      << " ";
        std::cout << "]" << (i != dim - 1 ? ",\n" : "");
    }
    std::cout << "]" << std::endl;
}
template <typename T>
void highlighted_print(const T **matrix, int dim, const std::set<std::tuple<int, int>> highlight, int width)
{
    width = width <= 0 ? 4 : width;
    std::cout << "[";
    for (int i = 0; i < dim; i++)
    {
        if (i)
        {
            std::cout << " ";
        }

        std::cout << "[ ";
        for (int j = 0; j < dim; j++)
        {
            bool is_highlighted = highlight.contains({i, j});
            std::cout << (is_highlighted ? "\033[32m" : "") << std::setw(width) << matrix[i][j] << (is_highlighted ? "\033[0m" : "") << (j != dim - 1 ? "," : "") << " ";
        }
        std::cout << "]" << (i != dim - 1 ? ",\n" : "");
    }
    std::cout << "]" << std::endl;
}
template <typename T>
void print(const T *array, int dim, int width)
{
    width = width <= 0 ? 4 : width;
    std::cout << "[";
    for (int i = 0; i < dim; i++)
    {
        std::cout << std::setw(width) << array[i] << (i != dim - 1 ? "," : "")
                  << " ";
    }
    std::cout << "]" << std::endl;
}

template <typename T>
void fill_random(T **matrix, int dim, T max_val, T min_val)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<T> distr(min_val, max_val);

    for (int i = 0; i < dim; i++)
        for (int j = 0; j < dim; j++)
            matrix[i][j] = distr(gen);
}
template void fill_random(int **matrix, int dim, int max_val, int min_val);

template void print(const int *array, int dim, int width);

template void highlighted_print(const int **matrix, int dim, const std::set<std::tuple<int, int>> highlight, int width);

template void print(const int **matrix, int dim, int width);
