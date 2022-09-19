
#include <iostream>
#include <iomanip>
#include <map>
#include <string>
#include <set>
#include <random>
#include <fstream>

#include <functional>
template <typename T>
const std::set<T> &empty_set()
{
    static const std::set<T> output;
    return output;
}
template <typename T>
void print(const T **matrix, int dim, int width = -1)
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
void highlighted_print(const T **matrix, int dim, const std::set<std::tuple<int, int>> highlight, int width = -1)
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
void print(const T *array, int dim, int width = -1)
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
void fill_random(T **matrix, int dim, T max_val, T min_val = 0)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<T> distr(min_val, max_val);

    for (int i = 0; i < dim; i++)
        for (int j = 0; j < dim; j++)
            matrix[i][j] = distr(gen);
}

/// @brief Class used to construct an arg consumer. Note that this is just for baking functions directly into the consumer. It is not a parser!
class ArgConsumer
{
    typedef std::map<std::string, std::function<void(char *)>> argmap;
    typedef std::set<std::string> argflags;

private:
    const std::string m_name;
    std::set<std::string> m_flags;
    std::map<std::string, std::string> m_descriptions;
    argmap m_actions;
    int m_max_length = 0;
    bool m_consumed;
    /// @brief Whether the consumer has been used
    /// @return false if the consumer is yet to be used, otherwise throws an exception
    bool is_consumed() noexcept(false)
    {
        if (this->m_consumed)
        {
            throw std::runtime_error("Args have already been consumed");
        }
        return false;
    }

public:
    /// @brief Construct a consumer with the given name
    /// @param name The name of the consumer
    ArgConsumer(std::string name) : m_name(name) {}
    explicit ArgConsumer() : ArgConsumer(nullptr) {}
    /// @brief Add an argument to the consumer
    /// @param name the name of the argument
    /// @param action the action to be performed if the name is present
    /// @param description the (optional) description of the argument
    /// @param is_flag whether the argument is a flag and doesn't require arguments
    /// @return this consumer so it can be used as a builder
    ArgConsumer &add_argument(std::string name, std::function<void(char *)> action, std::string description = "", bool is_flag = false)
    {
        if (is_flag)
        {
            this->m_flags.insert(name);
        }
        if (description != "")
        {
            this->m_descriptions.insert({name, description});
        }
        this->m_max_length = std::max<int>(this->m_max_length, name.length());
        this->m_actions.insert({name, action});
        return *this;
    }
    /// @brief Consume the args
    /// @param argc the number of args
    /// @param argv the array of args
    void consume(int argc, char *argv[]) noexcept(false)
    {
        if (!this->is_consumed())
        {
            this->add_argument("--help", [this](char *value)
                               {
                                int width = 2 + this->m_max_length;
         std::cout << this->m_name << std::endl;

         for (auto it = this->m_descriptions.begin(); it != this->m_descriptions.end(); it++)
         {
           std::cout << std::setw(width) << it->first
                     << " : " << it->second
                     << std::endl;
         }
         std::exit(0); });
        }
        std::function<void(char *)>
            fn = nullptr;
        for (int argi = 0; argi < argc; ++argi)
        {
            if (fn != nullptr)
            {
                fn(argv[argi - 0]);
                fn = nullptr;
            }
            if (this->m_actions.contains(argv[argi]))
            {
                fn = this->m_actions[argv[argi]];
                if (this->m_flags.contains(argv[argi]))
                {
                    fn(nullptr);
                    fn = nullptr;
                }
            }
        }
        if (fn != nullptr)
        {
            fn(nullptr);
            fn = nullptr;
        }
        this->m_consumed = true;
    }
};

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
