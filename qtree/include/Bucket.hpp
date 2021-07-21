#ifndef BUCKET_HPP
#define BUCKET_HPP

#include "QTreeMember.hpp"

#include <vector>
#include <memory>

class Bucket
{
public:

private:
    std::vector<std::shared_ptr<Bucket>> m_children;
    std::vector<std::shared_ptr<QTreeMember>> m_members;
};

#endif /* BUCKET_HPP */