#include <filesystem>
#include "AppMain.hpp"

NO_STACK_PROTECTOR
int main(int argc, char* argv[])
{
  CefSettings settings;
  CefString(&settings.browser_subprocess_path) = (std::filesystem::current_path() / CEF_RUNTIME_DIR / CEF_RUNTIME_NAME).c_str();
  return AppMain(CefMainArgs{ argc, argv }, settings);
}
