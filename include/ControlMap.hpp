#ifndef CONTROLMAP_HPP
#define CONTROLMAP_HPP

#include <string>
#include <vector>

namespace mode7
{

class ControlMap
{
public:
    ControlMap();
    ~ControlMap();

    void open(const std::string&);
    float calculate(float, float);

private:
    int to_index(int, int);

    float m_width;
    float m_height;
    int m_nrows;
    int m_ncols;
    std::vector<float> m_map;
};

}

#endif /* CONTROLMAP_HPP */