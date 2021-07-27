#ifndef INPUT_HPP
#define INPUT_HPP

#include <vector>
#include <memory>

#define INPUT_UNLIMITED_SOURCES -1

class Output;

class Input
{
public:
    Input() :
        maxSources(-1)
    {

    }

    virtual ~Input()
    {

    }

    inline void setMaxSources(int amt)
    {
        maxSources = amt;
        if (amt >= 0)
        {
            sources.reserve(amt);
        }
    }

    virtual inline int plug(std::shared_ptr<Output> out)
    {
        if (maxSources >= 0 && sources.size() >= maxSources)
        {
            return 1;
        }

        sources.push_back(out);

        return 0;
    }

protected:
    int maxSources;
    std::vector<std::shared_ptr<Output>> sources;
};

#endif /* INPUT_HPP */
