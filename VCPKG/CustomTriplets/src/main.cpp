
#include <print>
#include <iostream>
#include <nfd.hpp>
#include <fmt/base.h>

auto main(void) -> int
{
  try
  {
    NFD_Init();
    {
      nfdu8char_t* open_path;
      const auto result = NFD::PickFolder(open_path);
      if (result == NFD_OKAY)
      {
        NFD_FreePathU8(open_path);
      }
      else if (result == NFD_CANCEL)
      {
        fmt::println("User pressed cancel.");
      }
      else
      {
        fmt::println("Error: {}", NFD_GetError());
      }
    }
    NFD_Quit();

    return 0;
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
