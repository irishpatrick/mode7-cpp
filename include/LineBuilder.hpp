#ifndef LINEBUILDER_HPP
#define LINEBUILDER_HPP

#include <string>
#include <nlohmann/json.hpp>
#include "Object.hpp"

using json = nlohmann::json;

typedef enum {ACCELERATION, COASTING, BRAKING} zone_t;

class LineBuilder : public Object
{
public:

    LineBuilder();
    ~LineBuilder();

    void save(const std::string&);
    void addPoint(float, float, zone_t);
    void input();

private:

    json m_out;
};

#endif /* LINEBUILDER_HPP */