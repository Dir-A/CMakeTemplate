set(VCPKG_TARGET_ARCHITECTURE x64)
set(VCPKG_CRT_LINKAGE dynamic)
set(VCPKG_LIBRARY_LINKAGE dynamic)

# link nativefiledialog-extended as static
if (${PORT} MATCHES "nativefiledialog-extended")
    set(VCPKG_LIBRARY_LINKAGE static)
endif ()

# link fmt as dynamic
