#include <print>
#include <iostream>
#include <My/MxHello/SayHello.h>


auto main(void) -> int
{
    try
    {
        My::MxHello::SayC();
        My::MxHello::SayHello{}.Say();
    }
    catch (const std::exception& err)
    {
        std::println(std::cerr, "std::exception: {}", err.what());
    }
}
