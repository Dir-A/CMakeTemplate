include(FetchContent)

FetchContent_Declare(
  ZxFile
  GIT_REPOSITORY https://github.com/ZQF-Zut/ZxFile.git
  GIT_TAG 40443973d0a138611e625f9fce8d40aaea250999)
FetchContent_MakeAvailable(ZxFile)

FetchContent_Declare(
  fmt
  GIT_REPOSITORY https://github.com/fmtlib/fmt
  GIT_TAG e69e5f977d458f2650bb346dadf2ad30c5320281)
FetchContent_MakeAvailable(fmt)
