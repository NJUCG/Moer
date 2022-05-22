// test testing module
#define CATCH_CONFIG_MAIN
#include "catch2/catch.hpp"

#include "../../src/Texture/ProceduralTexture.h"

TEST_CASE("test-texture")
{
    std::string_view str = "Hello";

    Checkerboard2D ckbd;
    TextureCoord2D tc;
    tc.coord = Point2d(0.0, 0.0);
    REQUIRE(ckbd.eval(tc) == 0.0);
}