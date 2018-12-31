// filename: lazy_utils.hpp
#ifndef __LAZY_UTILS_HPP__
#define __LAZY_UTILS_HPP__
namespace __utils{

template <typename T> class lazy
{
private:
    bool initialized_;
    T value_;
    std::function<T()> policy_;
    static T default_policy()
    {
        throw std::runtime_error("No lazy initiation policy given.");
    }
public:
    lazy() : policy_(default_policy), initialized_(false) {}
    lazy(std::function<T()> _) : policy_(_), initialized_(false) {}
    lazy(const lazy<T>& _) : policy_(_.policy_), initialized_(false) {}

    lazy<T>& operator=(const lazy<T>& _)
    {
        policy_ = _.policy_;
        initialized_ = false;
        return *this;
    }

    T& get_value()
    {
        if (!initialized_)
        {
            value_ = policy_();
            initialized_ = true;
        }
        return value_;
    }

    T& operator()() { return get_value(); }
};




} // namespace __utils
#endif // __LAZY_UTILS_HPP__
