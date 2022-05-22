// test testing module
#define CATCH_CONFIG_MAIN
#include "catch2/catch.hpp"

using namespace std;

TEST_CASE( "test catch framework" )
{
    std::string_view str = "Hello";

    REQUIRE( str == "Hello" );
}