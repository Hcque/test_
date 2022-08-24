
cmake_minimum_required(VERSION 3.16)

project(
    MyNoise
)

file(GLOB_RECURSE
        NOISEPAGE_SRCS       
        ${PROJECT_SOURCE_DIR}/src/*.cpp
        ${PROJECT_SOURCE_DIR}/src/include/*.h
        )

message("fecthcontent ")
# include(FetchContent)                   # FetchContent_ functions.
message("fecthcontent ")

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 14)

include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.12.1
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# function(add_noisepage_dep NAME GIT_URL GIT_TAG)
#     string(TOLOWER "${NAME}" NAME_LOWER)    # The automatically created variables are lowercase. Footgun!
#     FetchContent_Declare(${NAME})           # Declare the resource to be fetched.
#     if (NOT ${NAME}_POPULATED)
#         # The long form of FetchContent_Populate is used since NoisePage requires control of the source folder name.
#         # This is to enable includes like #include "NAME/their_files.h".
#         message("do")
#         FetchContent_Populate(${NAME}
#                 QUIET                                                   # Don't print verbose output while populating.
#                 SOURCE_DIR ${CMAKE_BINARY_DIR}/_deps/src/${NAME}        # Source files go here.
#                 BINARY_DIR ${CMAKE_BINARY_DIR}/_deps/build/${NAME}      # Build files go here.
#                 SUBBUILD_DIR ${CMAKE_BINARY_DIR}/_deps/sub/${NAME}      # Sub-build files go here.
#                 GIT_REPOSITORY ${GIT_URL}                               # Download from GIT_URL.
#                 GIT_TAG ${GIT_TAG}                                      # Checkout the specific GIT_TAG version.
#                 )

#         # Whenever custom behavior is required, override with NoisePage third_party/deps_override/NAME_CMakeLists.txt.
#         set(DEP_CUSTOM_CMAKELISTS ${PROJECT_SOURCE_DIR}/third_party/deps_override/${NAME}_CMakeLists.txt)
#         if (EXISTS ${DEP_CUSTOM_CMAKELISTS})
#             file(COPY ${DEP_CUSTOM_CMAKELISTS} DESTINATION ${${NAME_LOWER}_SOURCE_DIR})
#             file(RENAME ${${NAME_LOWER}_SOURCE_DIR}/${NAME}_CMakeLists.txt ${${NAME_LOWER}_SOURCE_DIR}/CMakeLists.txt)
#             message(STATUS "Copied ${DEP_CUSTOM_CMAKELISTS} -> ${${NAME_LOWER}_SOURCE_DIR}/CMakeLists.txt.")
#         endif ()
#         unset(DEP_CUSTOM_CMAKELISTS)

#         set(MESSAGE_QUIET ON)                                                           # Silence third party includes.
#         add_subdirectory(${${NAME_LOWER}_SOURCE_DIR} ${${NAME_LOWER}_BINARY_DIR})       # Include the dependency.
#         unset(MESSAGE_QUIET)                                                            # Undo silence.
#     endif ()

#     # Get the date of the last git commit for the GIT_TAG branch.
#     execute_process(COMMAND git log -1 --format=%cd --date=short                        # Get the last commit's date.
#             WORKING_DIRECTORY ${${NAME_LOWER}_SOURCE_DIR}                               # From the specified git dir.
#             OUTPUT_VARIABLE GIT_LAST_COMMIT)                                            # Save to GIT_LAST_COMMIT.
#     string(STRIP "${GIT_LAST_COMMIT}" GIT_LAST_COMMIT)                                  # Remove any trailing newlines.

#     message(STATUS "[FOUND] ${NAME} (${GIT_URL} ${GIT_TAG} ${GIT_LAST_COMMIT})")        # Print a success message.
# endfunction()


# include(GoogleTest)     # We rely on GoogleTest for all our tests.
# add_noisepage_dep(googlebenchmark https://github.com/google/benchmark.git v1.5.2)
# add_noisepage_dep(googletest https://github.com/google/googletest.git release-1.10.0)

# find_package(benchmark REQUIRED)
# add_subdirectory(benchmark)


enable_testing()

add_executable(
  hello_test
  test/mvcc_test.cpp
  test/adobe_test.cpp
)
target_link_libraries(
  hello_test
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(hello_test)



add_library(noisepage_lib OBJECT ${NOISEPAGE_SRCS})

target_compile_options(noisepage_lib PUBLIC 
    "-march=native"
)




