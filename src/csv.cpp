#include "csv.hpp"
#include <fstream>

template <typename T>
int read_csv(T **&mat, const std::string &path, const char sep, std::function<T(const std::string &)> from_string)
{
    std::ifstream csv_file{path};
    std::string line;
    std::vector<T> values;
    std::string value = "";

    getline(csv_file, line);
    for (auto c : line)
    {
        if (c == sep)
        {
            values.push_back(from_string(value));
            value = "";
        }
        else
        {
            value += c;
        }
    }

    values.push_back(from_string(value));
    int dim = (int)values.size();
    mat = new T *[dim];
    int i;
    for (i = 0; i < dim; i++)
    {
        mat[i] = new T[dim];
    }
    std::copy(values.begin(), values.end(), mat[0]);

    i = 1;
    while (getline(csv_file, line))
    {
        values.clear();
        value = "";
        for (auto c : line)
        {
            if (c == sep)
            {
                values.push_back(from_string(value));
                value = "";
            }
            else
            {
                value += c;
            }
        }
        values.push_back(from_string(value));
        if (values.size() != dim)
        {
            throw std::runtime_error("Expected all rows to be the same size");
        }
        std::copy(values.begin(), values.end(), mat[i++]);
    }
    if (i != dim)
    {
        throw std::runtime_error("Expected same number of rows as columns");
    }
    csv_file.close();
    return dim;
}

template int read_csv(int **&mat, const std::string &path, const char sep, std::function<int(const std::string &)> from_string);
