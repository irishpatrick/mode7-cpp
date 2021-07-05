#ifndef LINE_HPP
#define LINE_HPP

#include <string>
#include <sstream>

namespace mode7
{

class Line
{
public:

    Line();
    ~Line();

    void create(float, float);
    void create(float, float, float, float);

    float solve(float);
    float solveInv(float);

    inline std::string str()
    {
        std::stringstream ss;
        ss << "(" << x0 << "," << y0 << ") ";
        ss << "(" << x1 << "," << y1 << ") ";
        ss << "y = " << m << " * x + " << b;
        return ss.str();
    }

    float m;
    float b;

    float x0, y0;
    float x1, y1;
};

}

#endif /* LINE_HPP */
