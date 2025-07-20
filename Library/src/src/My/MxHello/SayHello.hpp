#pragma once
#ifdef _WIN32
#ifndef ZQF_EXPORT
#define ZQF_EXPORT __declspec(dllexport)
#endif
#else
#define ZQF_EXPORT
#endif


namespace My::MxHello
{
    class ZQF_EXPORT SayHello
    {
    public:
        auto Say() -> void;

    };

    ZQF_EXPORT void SayC();
}
