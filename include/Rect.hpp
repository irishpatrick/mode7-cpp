#ifndef RECT_HPP
#define RECT_HPP

#include "Line2D.hpp"
#include "gl.hpp"
#include "Util.hpp"

#include <iostream>
#include <vector>

namespace mode7
{
    class Rect
    {
    public:

        /**
         * Order the points like so
         * Must be counterclockwise
         * 
         * pa --ld-- pd
         * |         |
         * la        lc
         * |         |
         * pb --lb-- pc
         */
        inline void connect(glm::vec2 pa, glm::vec2 pb, glm::vec2 pc, glm::vec2 pd)
        {
            p[0] = pa;
            p[1] = pb;
            p[2] = pc;
            p[3] = pd;

            a.fromPoints(pa, pb);
            b.fromPoints(pb, pc);
            c.fromPoints(pc, pd);
            d.fromPoints(pd, pa);
        }

        inline bool checkIntersect(glm::vec2 pt)
        {
            float da = a.distTo(pt);
            float db = b.distTo(pt);
            float dc = c.distTo(pt);
            float dd = d.distTo(pt);

            /*std::cout << "\tpt: " << glm::to_string(pt) << std::endl;
            std::cout << (da >= 0) + (db >= 0) + (dc >= 0) + (dd >= 0) << std::endl;
            for (int i = 0; i < 4; ++i)
            {
                std::cout << "\t" << glm::to_string(p[i]) << std::endl;
            }
            std::cout << "\t" << a.str() << std::endl;
            std::cout << "\t" << b.str() << std::endl;
            std::cout << "\t" << c.str() << std::endl;
            std::cout << "\t" << d.str() << std::endl;
            
            std::cout << "\t" << da << std::endl;
            std::cout << "\t" << db << std::endl;
            std::cout << "\t" << dc << std::endl;
            std::cout << "\t" << dd << std::endl;
            assert(((da >= 0) + (db >= 0) + (dc >= 0) + (dd >= 0)) >= 2);*/

            bool status = da >= 0 && db >= 0 && dc >= 0 && dd >= 0;
           
            // if (status)
            // {
            //     std::cout << "\t" << da << " ";
            //     std::cout << "\t" << db << " ";
            //     std::cout << "\t" << dc << " ";
            //     std::cout << "\t" << dd << std::endl;
            // }

            return status;
        }

        inline bool checkIntersect(Rect& r)
        {
            auto a = getLines();
            auto b = r.getLines();
            return Util::sat(a, b);
        }

        inline std::vector<Line2D*> getLines()
        {
            std::vector<Line2D*> out;
            out.reserve(4);
            out.push_back(&a);
            out.push_back(&b);
            out.push_back(&c);
            out.push_back(&d);
            return out;
        }

        Line2D a;
        Line2D b;
        Line2D c;
        Line2D d;
        glm::vec2 p[4];
    };
}
#endif /* RECT_HPP */
