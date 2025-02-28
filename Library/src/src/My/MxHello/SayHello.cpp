#include "SayHello.h"
#include <print>


namespace My::MxHello
{
    auto SayHello::Say() -> void
    {
        std::print("My::MxHello::SayHello::Say: Hello!");
    }

    void SayC()
    {
        std::print("My::MxHello::SayC: Hello!");
    }
}
