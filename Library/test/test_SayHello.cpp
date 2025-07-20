#define CATCH_CONFIG_MAIN
#include <catch.hpp>
#include <My/MxHello/SayHello.hpp>


TEST_CASE("SayHello", "[hello]") {
    My::MxHello::SayHello{}.Say();
}
