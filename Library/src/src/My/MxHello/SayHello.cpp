#include "SayHello.h"
#include <print>


namespace My::MxHello
{
    auto SayHello::Say() -> void
    {
        std::print("Hello!");
    }
}
