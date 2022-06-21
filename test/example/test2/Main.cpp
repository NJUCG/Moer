// testing example
#define CATCH_CONFIG_MAIN
#include "catch2/catch.hpp"

#include "Other.h"

TEST_CASE( "test multi cpp" )
{
    REQUIRE( test() );
}