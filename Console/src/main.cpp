#include <print>
#include <iostream>


auto main(void) -> int
{
    try
    {
        std::println("Hello!");
    }
    catch (const std::exception& err)
    {
        std::println(std::cerr, "std::exception: {}", err.what());
    }
}

