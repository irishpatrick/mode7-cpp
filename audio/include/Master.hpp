#ifndef MASTER_HPP
#define MASTER_HPP

#include "Input.hpp"
#include "Output.hpp"

class Master : public Input, Output
{
public:
    Master();
    ~Master();
};

#endif /* MASTER_HPP */