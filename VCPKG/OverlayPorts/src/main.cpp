#include <print>
#include <iostream>
#include <nfd.hpp>

auto main(void) -> int
{
  try
  {
    const auto default_dir = "C:\\";

    NFD_Init();
    {
      // please select a non-default folder
      {
        nfdu8char_t* open_path;
        const auto result = NFD::PickFolder(open_path, default_dir);
        if (result == NFD_OKAY)
        {
          NFD_FreePathU8(open_path);
        }
        else if (result == NFD_CANCEL)
        {
          std::println("User pressed cancel.");
        }
        else
        {
          std::println("Error: {}", NFD_GetError());
        }
      }

      // if you don't use overlay ports to modify the NFD's port, the defaultPath parameter will be ignored.
      // when NFD::PickFolder is called in this case, the default folder will be the one you last selected.
      // if you use overlay ports, then the NFD::PickFolder call will follow the default_dir, which is the C drive.
      {
        nfdu8char_t* open_path;
        const auto result = NFD::PickFolder(open_path, default_dir);
        if (result == NFD_OKAY)
        {
          NFD_FreePathU8(open_path);
        }
        else if (result == NFD_CANCEL)
        {
          std::println("User pressed cancel.");
        }
        else
        {
          std::println("Error: {}", NFD_GetError());
        }
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
