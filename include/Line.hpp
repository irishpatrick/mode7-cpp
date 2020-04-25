#ifndef LINE_HPP
#define LINE_HPP

class Line
{
public:

    Line();
    ~Line();

    void create(float, float);
    void create(float, float, float, float);

    float solve(float);
    float solveInv(float);

    float m;
    float b;
};

#endif /* LINE_HPP */