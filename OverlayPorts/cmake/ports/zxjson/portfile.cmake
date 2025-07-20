vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO ZQF-Zut/ZxJson
    REF d02d1de725b1d2629d01dbb163acac3e72dbc767
    SHA512 9135791334f851414125da481ebb6af1c37489463e4593a8b161cdce0cae6f8110bd01cb1f4a7e6df4c382c32ac861cff61f580452dc64ee3297ff117aa74c79
    HEAD_REF master
)

vcpkg_cmake_configure(SOURCE_PATH "${SOURCE_PATH}")
vcpkg_cmake_install()
vcpkg_fixup_pkgconfig()

file(COPY "${CMAKE_CURRENT_LIST_DIR}/usage" DESTINATION "${CURRENT_PACKAGES_DIR}/share/${PORT}")