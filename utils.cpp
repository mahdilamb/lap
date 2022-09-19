
#include <iostream>
#include <iomanip>
#include <map>
#include <string>
#include <set>
#include <random>

#include <functional>
template <typename T>
void print(const T **matrix, int dim, int width = 4)
{
    std::cout << "[";
    for (int i = 0; i < dim; i++)
    {
        if (i)
        {
            std::cout << " ";
        }

        std::cout << "[";
        for (int j = 0; j < dim; j++)
            std::cout << std::setw(width) << matrix[i][j] << (j != dim - 1 ? "," : "")
                      << " ";
        std::cout << "]" << (i != dim - 1 ? ",\n" : "");
    }
    std::cout << "]" << std::endl;
}
template <typename T>
void print(const T *array, int dim, int width = 4)
{
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
