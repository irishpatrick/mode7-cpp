#define CATCH_CONFIG_RUNNER
#include "catch_all.hpp"

#include "Rect.hpp"
#include "gl.hpp"

unsigned int Factorial( unsigned int number ) {
    return number <= 1 ? number : Factorial(number-1)*number;
}

TEST_CASE( "Factorials are computed", "[factorial]" ) {
    REQUIRE( Factorial(1) == 1 );
    REQUIRE( Factorial(2) == 2 );
    REQUIRE( Factorial(3) == 6 );
    REQUIRE( Factorial(10) == 3628800 );
}

TEST_CASE("Test Rect class", "[Rect]")
{
    mode7::Rect r;
    glm::vec2 pt;
    r.connect(
        glm::vec2(0, 0),
        glm::vec2(0, 1),
        glm::vec2(1, 0),
        glm::vec2(1, 1)
    );
    
    pt = glm::vec2(0.5, 0.5);
    REQUIRE(r.checkIntersect(pt) == true);

    pt = glm::vec2(100, 100);
    REQUIRE(r.checkIntersect(pt) == false);
}

int main( int argc, char* argv[] ) {
    // global setup...
    int result = Catch::Session().run( argc, argv );
    // global clean-up...
    return result;
}