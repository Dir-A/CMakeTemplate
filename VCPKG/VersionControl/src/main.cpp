#include <print>
#include <iostream>
#include <fmt/core.h>
#include <sqlite3.h>
#include <boost/thread.hpp>

auto main(void) -> int
{
  try
  {
    boost::thread thread{ [] {
      fmt::print("from boost::thread: fmt_version:{}, boost_version:{}, sqlite_version:{}\n", FMT_VERSION, BOOST_VERSION, sqlite3_libversion_number()); //
    } };
    thread.join();
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
