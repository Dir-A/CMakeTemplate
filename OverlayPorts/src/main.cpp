#include <print>
#include <iostream>
#include <sqlite3.h>
#include <Zut/ZxJson.h>


auto main(void) -> int
{
    try
    {
        const ZxJson::JObject_t obj{ {"name", "sqlite3"}, {"version", sqlite3_libversion_number()} };
        const auto json_dump_str = ZxJson::StoreViaMemory(obj, true);
        std::println("SQLite version: {}", sqlite3_libversion());
        std::println("ZxJson:\n{}", json_dump_str);
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

