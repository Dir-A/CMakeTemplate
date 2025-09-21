#include <print>
#include <iostream>
#include <fmt/base.h>
#include <Zut/ZxJson.h>

auto main() -> int
{
  try
  {
    const auto json = ZxJson::JObject_t{ { "name", "xiao" }, { "age", 6 }, { "list", ZxJson::JArray_t{ 1, 2, 3, 4, 5 } } };
    const auto json_string = ZxJson::StoreViaMemory(json, true);
    fmt::println("fmt:\n{}", json_string);
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
