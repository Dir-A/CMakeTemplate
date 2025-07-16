#include <print>
#include <iostream>
#include <sqlite3.h>


auto main(void) -> int
{
    try
    {
        const auto version = sqlite3_libversion_number();
        std::println("SQLite version: {} ({})", sqlite3_libversion(), version);
    }
    catch (const std::exception& err)
    {
        std::println(std::cerr, "std::exception: {}", err.what());
    }
}

