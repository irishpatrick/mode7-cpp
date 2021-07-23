#define CATCH_CONFIG_RUNNER
#include "catch_all.hpp"

#include "Rect.hpp"
#include "Line2D.hpp"
#include "gl.hpp"
#include "geometry.h"

#define EPSILON 0.001

TEST_CASE("Test libgeometry lines", "[libgeometry]")
{
    line l1;
    line l2;

    line_connect(&l1, 0, 0, 10, 10);
    line_connect(&l2, 0, 10, 10, 0);

    REQUIRE(line_slope(&l2) - 1.0 < EPSILON);
    REQUIRE(line_intercept(&l2) - 10.0 < EPSILON);
    REQUIRE(line_intersects(&l1, &l2));

    line_connect(&l2, 1, 0, 11, 10);
    REQUIRE(!line_intersects(&l1, &l2));

    line_connect(&l1, 271.207, 45.227, 301.144, 43.278);
    line_connect(&l2, 301.293, 45.880, 271.295, 46.264);
    REQUIRE(!line_intersects(&l1, &l2));
}

TEST_CASE("Test libgeometry quads", "[libgeometry]")
{
    vec2 a;
    vec2 b;
    vec2 c;
    vec2 d;
    quad q1;
    int result;

    a.x = 0; a.y = 0;
    b.x = 10; b.y = 0;
    c.x = 10; c.y = 10;
    d.x = 0; d.y = 10;
    REQUIRE(quad_connect(&q1, a, b, c, d) == 0);

    a.x = 0; a.y = 0;
    b.x = 10; b.y = 0;
    d.x = 10; d.y = 10;
    c.x = 0; c.y = 10;
    REQUIRE(quad_connect(&q1, a, b, c, d) == 1);

    a.x = 0; a.y = 0;
    b.x = 10; b.y = 10;
    c.x = 10; c.y = 0;
    d.x = 0; d.y = 10;
    REQUIRE(quad_connect(&q1, a, b, c, d) == 2);

    a.x = 0; a.y = 0;
    b.x = 10; b.y = 0;
    d.x = 10; d.y = 10;
    c.x = 0; c.y = 10;
    result = quad_connect(&q1, a, b, c, d);
    REQUIRE(result == 1);
    quad_orient(&q1, result);
    REQUIRE(!(
        line_intersects(q1.l + 0, q1.l + 2) ||
        line_intersects(q1.l + 1, q1.l + 3)
    ));

    a.x = 0; a.y = 0;
    b.x = 10; b.y = 10;
    c.x = 10; c.y = 0;
    d.x = 0; d.y = 10;
    result = quad_connect(&q1, a, b, c, d);
    REQUIRE(result == 2);
    quad_orient(&q1, result);
    REQUIRE(!(
        line_intersects(q1.l + 0, q1.l + 2) ||
        line_intersects(q1.l + 1, q1.l + 3)
    ));

    a.x = 271.207;
    a.y = 45.227;
    b.x = 301.144;
    b.y = 43.278;
    c.x = 301.293;
    c.y = 45.880;
    d.x = 271.295;
    d.y = 46.264;
    result = quad_connect(&q1, a, b, c, d);
    REQUIRE(result == 0);
    result = quad_orient(&q1, result);
    REQUIRE(result == 0);
}

TEST_CASE("Test Line2D class", "[Line2D]")
{
    mode7::Line2D line;
    glm::vec2 pt;

    line.create(glm::vec2(0, 0), glm::vec2(10, 10));

    pt = glm::vec2(5, 5);
    REQUIRE(line.distTo(pt) < EPSILON);

    pt = glm::vec2(5, 5) + glm::normalize(glm::vec2(1, 1));
    REQUIRE(line.distTo(pt) - 1.0 < EPSILON);

    REQUIRE(line.length() - sqrt(10 * 10 + 10 * 10) < EPSILON);
}

TEST_CASE("Test Rect class", "[Rect]")
{
    mode7::Rect r;
    mode7::Rect s;
    glm::vec2 pt;

    r.connect(
        glm::vec2(0, 0),
        glm::vec2(1, 0),
        glm::vec2(1, 1),
        glm::vec2(0, 1)
    );
    
    REQUIRE(r.checkIntersect(glm::vec2(0.5)) == true);

    REQUIRE(r.checkIntersect(glm::vec2(100)) == false);

    s.connect(
        glm::vec2(0.5, 0.5),
        glm::vec2(0.5, 1.5),
        glm::vec2(1.5, 1.5),
        glm::vec2(1.5, 0.5)
    );
    REQUIRE(r.checkIntersect(s) == true);

    s.connect(
        glm::vec2(100, 100),
        glm::vec2(100, 110),
        glm::vec2(110, 110),
        glm::vec2(110, 100)
    );
    REQUIRE(r.checkIntersect(s) == false);
}

int main( int argc, char* argv[] ) {
    // global setup...
    int result = Catch::Session().run( argc, argv );
    // global clean-up...
    return result;
}