cmake_minimum_required(VERSION 2.6)

add_subdirectory(libmtkconfigutils)
add_subdirectory(libmtkcutils)
add_subdirectory(libmtknetutils)
add_subdirectory(libmtkrilutils)
add_subdirectory(libmtksysutils)
add_subdirectory(libmtktinyxml)
add_subdirectory(libmtkutils)
install(DIRECTORY include/ DESTINATION include/core/utils
        FILES_MATCHING PATTERN "*.h")

