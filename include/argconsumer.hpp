#ifndef __LAP_UTILS_ARGS_CONSUMER
#define __LAP_UTILS_ARGS_CONSUMER
#include <map>
#include <set>
#include <string>
#include <functional>
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
    ArgConsumer &add_argument(std::string name, std::function<void(char *)> action, std::string description = "", bool is_flag = false);
    /// @brief Consume the args
    /// @param argc the number of args
    /// @param argv the array of args
    void consume(int argc, char *argv[]) noexcept(false);
};

#endif
