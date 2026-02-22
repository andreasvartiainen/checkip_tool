cmake_minimum_required(VERSION 3.10)

project(checkip)

enable_testing()

# C++ standard to 23 to support std::print/ln()
set(CMAKE_C_STANDARD 23)
# add warnings as errors -Werror not in this because it causes catch2 to fail
# use CMakeSelectBuildType to build debug/release
add_compile_options(-Wall -Wextra -Wconversion -Wpedantic -Wreorder -Wunused -Wshadow -Werror=return-type)

# ---------- how to add libraries -------------

# include(FetchContent)
#
# FetchContent_Declare(
# 	raylib
# 	GIT_REPOSITORY https://github.com/raysan5/raylib
# 	GIT_TAG 5.5
# 	FIND_PACKAGE_ARGS ${RAYLIB_VERSION}
# )
#
# set(BUILD_EXAMPLES OFF CACHE INTERNAL "")
#
# FetchContent_MakeAvailable(raylib)

# find_package(SQLite3 REQUIRED)

# ---------- end -------------

# globbing the sources in src folder to not require manual adding of .cpp files
file(GLOB_RECURSE SOURCES "src/*.c")
# list(APPEND SOURCES "ClayMan/clayman.cpp")

# remove main.cpp because that causes problems in the unit tests
list(FILTER SOURCES EXCLUDE REGEX "main.c" )

set(INCLUDE_DIRS
	${CMAKE_SOURCE_DIR}/include
)

add_executable(${PROJECT_NAME} src/main.c ${SOURCES})
# target for add_test()
add_executable(tests tests/test.c ${SOURCES})
# target_link_libraries(${PROJECT_NAME} raylib sqlite3)
# target_link_libraries(${PROJECT_NAME} -lm)
# the binary file to run for the test
add_test(test tests)

# ---------- way to copy data to the build directory -------------

# set(FILENAME example.input)
#
# # copy stuff automatically
# add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD
# 	COMMAND ${CMAKE_COMMAND} -E copy
# 	${CMAKE_SOURCE_DIR}/${FILENAME} ${CMAKE_BINARY_DIR}/
# )

# ---------- end -------------

include_directories(
	PRIVATE
		${INCLUDE_DIRS}
		# ClayMan
		# ClayMan/include
)

# way to copy the binary to the bin file
install(TARGETS ${PROJECT_NAME} DESTINATION ~/.local/bin)

