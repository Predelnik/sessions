#include "ixm/session.hpp"
#include "impl.hpp"


namespace ixm::session 
{
    // env::variable
    environment::variable::operator std::string_view() const noexcept
    {
        auto val = impl::get_env_var(m_key.c_str());
        
        if (val) {
            return val;
        } else {
            return {};
        }
    }
    
    auto environment::variable::operator=(std::string_view value) -> variable&
    {
        std::string val { value };
        impl::set_env_var(m_key.c_str(), val.c_str());
        return *this;
    }

    auto environment::variable::split() const -> std::pair<path_iterator, path_iterator>
    {
        auto value = this->operator std::string_view();
        return { path_iterator{value}, path_iterator{} };
    }


    // env
    auto environment::operator[] (const std::string& str) const noexcept -> variable
    {
        return operator[](str.c_str());
    }

    auto environment::operator[] (std::string_view str) const -> variable
    {
        return variable{ str };
    }

    auto environment::operator[] (const char*str) const noexcept -> variable
    {
        return variable{ str };
    }

    bool environment::contains(std::string_view key) const noexcept
    {
        auto thingy = std::string(key);
        return impl::get_env_var(thingy.c_str()) != nullptr;
    }

    auto environment::cbegin() const noexcept -> iterator
    {
        return iterator{ m_envp() };
    }

    auto environment::cend() const noexcept -> iterator
    {
        return iterator{ m_envp() + size()};
    }

    auto environment::size() const noexcept -> size_type
    {
        return impl::env_size();
    }

    void environment::internal_erase(const char* k) noexcept
    {
        impl::rm_env_var(k);
    }

    bool environment::internal_find(const char* key, int& offset) const noexcept
    {
        offset = impl::env_find(key);
        return offset != -1;
    }

    char const** environment::m_envp() const noexcept { return impl::envp(); }



    // args
    auto arguments::operator [] (arguments::index_type idx) const noexcept -> value_type
    {
        return impl::argv(idx);
    }

    arguments::value_type arguments::at(arguments::index_type idx) const
    {
        if (idx >= size()) {
            throw std::out_of_range("invalid arguments subscript");
        }

        return impl::argv(idx);
    }

    arguments::size_type arguments::size() const noexcept
    {
        return static_cast<size_type>(argc());
    }

    arguments::iterator arguments::cbegin () const noexcept
    {
        return iterator{argv()};
    }

    arguments::iterator arguments::cend () const noexcept
    {
        return iterator{argv() + argc()};
    }


    const char** arguments::argv() const noexcept
    {
        return impl::argv();
    }

    int arguments::argc() const noexcept
    {
        return impl::argc();
    }
}