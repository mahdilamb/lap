#include "argconsumer.hpp"
#include <iostream>
#include <iomanip>
ArgConsumer &ArgConsumer::add_argument(std::string name, std::function<void(char *)> action, std::string description, bool is_flag)
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
void ArgConsumer::consume(int argc, char *argv[]) noexcept(false)
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
