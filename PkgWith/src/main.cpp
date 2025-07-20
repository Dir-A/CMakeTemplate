#include <print>
#include <iostream>
#include <fmt/printf.h>
#include <Zut/ZxFile.h>


auto main(void) -> int
{
    try
    {
        fmt::println("Hello!");
        ZxFile{ "Hello.txt", ZxFile::OpenMod::WriteForce }.Write(std::span{ std::string_view{ "Hello!" } });
    }
    catch (const std::exception& err)
    {
        std::println(std::cerr, "std::exception: {}", err.what());
    }
    catch (...)
    {
        std::println(std::cerr, "unknown exception!");
    }
}
