The package sfml provides CMake targets:

    find_package(SFML COMPONENTS system window graphics CONFIG REQUIRED)
    target_link_libraries(main PRIVATE sfml-system sfml-network sfml-graphics sfml-window)

    # If you want SFML to provide an implementation of main():
    target_link_libraries(main PRIVATE sfml-main)


Applied user-wide integration for this vcpkg root.

CMake projects should use: "-DCMAKE_TOOLCHAIN_FILE=/usr/local/Cellar/vcpkg/2021.05.12/libexec/scripts/buildsystems/vcpkg.cmake"
