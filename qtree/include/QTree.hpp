#ifndef QTREE_HPP
#define QTREE_HPP

#include "QTreeMember.hpp"
#include "Bucket.hpp"

template <class T>
class QTree
{
public:
    QTree();
    ~QTree();

    void insert(QTreeMember&);

private:
    Bucket m_root;
};

#endif /* QTREE_HPP */