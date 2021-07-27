#ifndef OUTPUT_HPP
#define OUTPUT_HPP

#include <vector>
#include <memory>

#define OUTPUT_UNLIMITED_SINKS -1

class Input;

class Output
{
public:
    Output() :
        maxSinks(-1)
    {

    }
    
    virtual ~Output()
    {

    }

    inline void setMaxSinks(int amt)
    {
        maxSinks = amt;
        if (amt >= 0)
        {
            sinks.reserve(amt);
        }
    }

    virtual inline int plug(std::shared_ptr<Input> in)
    {
        if (maxSinks >= 0 && sinks.size() >= maxSinks)
        {
            return 1;
        }

        sinks.push_back(in);

        return 0;
    }

protected:
    int maxSinks;
    std::vector<std::shared_ptr<Input>> sinks;
};

#endif /* OUTPUT_HPP */
