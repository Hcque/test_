cmake_minimum_required(VERSION 3.16)
set(TPCCRUNNER_DESCRIPTION "tpcc-runner is a portable TPC-C benchmark framework for various in-memory transaction engines")

project(
  tpcc-runner
  VERSION 1.0.0.0
  DESCRIPTION "${TPCCRUNNER_DESCRIPTION}"
  )

message(STATUS "Welcome to tpcc-runner, Version: ${PROJECT_VERSION}")
set(CMAKE_C_COMPILER "gcc" CACHE STRING "C compiler")
set(CMAKE_CXX_COMPILER "g++" CACHE STRING "C++ compiler")
message(STATUS "C Compiler: ${CMAKE_C_COMPILER} ${CMAKE_C_COMPILER_ID} ${CMAKE_C_COMPILER_VERSION}")
message(STATUS "C++ Compiler: ${CMAKE_CXX_COMPILER} ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "System: ${CMAKE_SYSTEM_NAME} ${CMAKE_SYSTEM_VERSION} ${CMAKE_SYSTEM_PROCESSOR}")


function(print_sys_info QUERY_TARGET)
    cmake_host_system_information(RESULT SYS_INFO QUERY ${QUERY_TARGET})
    message(STATUS "  System ${QUERY_TARGET}: ${SYS_INFO}")
endfunction()
print_sys_info("NUMBER_OF_LOGICAL_CORES;NUMBER_OF_PHYSICAL_CORES")
print_sys_info("HOSTNAME;FQDN")
print_sys_info("AVAILABLE_VIRTUAL_MEMORY;TOTAL_VIRTUAL_MEMORY")
print_sys_info("AVAILABLE_PHYSICAL_MEMORY;TOTAL_PHYSICAL_MEMORY")
print_sys_info("IS_64BIT;HAS_IA64")
print_sys_info("HAS_FPU;HAS_MMX;HAS_MMX_PLUS")
print_sys_info("HAS_SSE;HAS_SSE2;HAS_SSE_FP;HAS_SSE_MMX")
print_sys_info("HAS_AMD_3DNOW;HAS_AMD_3DNOW_PLUS")
print_sys_info("HAS_SERIAL_NUMBER;PROCESSOR_SERIAL_NUMBER")
print_sys_info("PROCESSOR_NAME;PROCESSOR_DESCRIPTION")
print_sys_info("OS_NAME;OS_RELEASE;OS_VERSION;OS_PLATFORM")

############ ov

include(FetchContent)
function(add_dep NAME GIT_URL GIT_TAG)
    string(TOLOWER "${NAME}" NAME_LOWER)
    FetchContent_Declare(${NAME})
    if (NOT ${NAME}_POPULATED)
        FetchContent_Populate(${NAME}
                QUIET
                SOURCE_DIR ${CMAKE_BINARY_DIR}/_deps/src/${NAME}
                BINARY_DIR ${CMAKE_BINARY_DIR}/_deps/build/${NAME}
                SUBBUILD_DIR ${CMAKE_BINARY_DIR}/_deps/sub/${NAME}
                GIT_REPOSITORY ${GIT_URL}
                GIT_TAG ${GIT_TAG}
                )
        
        set(DEP_CUSTOM_CMAKELISTS ${PROJECT_SOURCE_DIR}/third_party/deps_override/${NAME}_CMakeLists.txt)
        if (EXISTS ${DEP_CUSTOM_CMAKELISTS})
            file(COPY ${DEP_CUSTOM_CMAKELISTS} DESTINATION ${${NAME_LOWER}_SOURCE_DIR})
            file(RENAME ${${NAME_LOWER}_SOURCE_DIR}/${NAME}_CMakeLists.txt ${${NAME_LOWER}_SOURCE_DIR}/CMakeLists.txt)
            message(STATUS "Copied ${DEP_CUSTOM_CMAKELISTS} -> ${${NAME_LOWER}_SOURCE_DIR}/CMakeLists.txt.")
        endif ()
        unset(DEP_CUSTOM_CMAKELISTS)

        add_subdirectory(${${NAME_LOWER}_SOURCE_DIR} ${${NAME_LOWER}_BINARY_DIR})
    endif ()

    execute_process(COMMAND git log -1 --format=%cd --date=short
            WORKING_DIRECTORY ${${NAME_LOWER}_SOURCE_DIR}
            OUTPUT_VARIABLE GIT_LAST_COMMIT)
    string(STRIP "${GIT_LAST_COMMIT}" GIT_LAST_COMMIT)

    message(STATUS "[FOUND] ${NAME} (${GIT_URL} ${GIT_TAG} ${GIT_LAST_COMMIT})")
endfunction()


# Set a default build type if none was specified
set(default_build_type "Release")
if(EXISTS "${CMAKE_SOURCE_DIR}/.git")
  set(default_build_type "Debug")
endif()


set(CC_LINK_LIBRARIES "")
set(CC_INCLUDE_DIRECTORIES "")


string(TOLOWER "${CC_ALG}" CC_NAME)
file(GLOB_RECURSE CC_SRCS 
"${PROJECT_SOURCE_DIR}/mvto/*.h"
# "${PROJECT_SOURCE_DIR}/protocols/${BENCH_NAME}_common/*.hpp"
# "${PROJECT_SOURCE_DIR}/protocols/${CC_NAME}/include/*.hpp" 
)

  set(CMAKE_CXX_STANDARD 17)
  list(APPEND CC_LINK_LIBRARIES "masstree")
  add_dep(masstree https://github.com/wattlebirdaz/masstree-beta.git master)
  list(APPEND CC_INCLUDE_DIRECTORIES "${CMAKE_BINARY_DIR}/_deps/src/") # masstree


set(TPCCRUNNER_SRCS "")
set(TPCCRUNNER_COMPILE_DEFINITIONS "")
set(TPCCRUNNER_COMPILE_OPTIONS "")
set(TPCCRUNNER_INCLUDE_DIRECTORIES "")
set(TPCCRUNNER_LINK_LIBRARIES "")
set(TPCCRUNNER_LINK_OPTIONS "")

list(APPEND TPCCRUNNER_SRCS "${CC_SRCS}" )
list(APPEND TPCCRUNNER_COMPILE_DEFINITIONS "${LOG_LEVEL_DEFINITION}")
list(APPEND TPCCRUNNER_COMPILE_OPTIONS "${COMMON_COMPILE_FLAGS}")
list(APPEND TPCCRUNNER_INCLUDE_DIRECTORIES "${PROJECT_SOURCE_DIR}/")
list(APPEND TPCCRUNNER_INCLUDE_DIRECTORIES "${CMAKE_BINARY_DIR}/_deps/src/")

list(APPEND TPCCRUNNER_INCLUDE_DIRECTORIES "${PROJECT_SOURCE_DIR}/src")


list(APPEND TPCCRUNNER_INCLUDE_DIRECTORIES "${CC_INCLUDE_DIRECTORIES}")
list(APPEND TPCCRUNNER_LINK_LIBRARIES "${CC_LINK_LIBRARIES}")

# include_directories(indexes)



# Create an objectlib, which can be used for creating both static and dynamic library
# add_library(tpccrunner_objlib OBJECT "${TPCCRUNNER_SRCS}")


# target_compile_definitions(tpccrunner_objlib PUBLIC ${TPCCRUNNER_COMPILE_DEFINITIONS})
# target_compile_options(tpccrunner_objlib PUBLIC ${TPCCRUNNER_COMPILE_OPTIONS})
# target_compile_options(tpccrunner_objlib PRIVATE ${COMMON_COMPILE_FLAGS})
# target_include_directories(tpccrunner_objlib PUBLIC ${TPCCRUNNER_INCLUDE_DIRECTORIES})
# target_link_libraries(tpccrunner_objlib PRIVATE ${TPCCRUNNER_LINK_LIBRARIES})
# target_link_options(tpccrunner_objlib PUBLIC ${TPCCRUNNER_LINK_OPTIONS})



add_executable(
  hello_test
  src/mvto/main.cpp
)

target_compile_options(hello_test PUBLIC "-pthread")

target_include_directories(hello_test PUBLIC ${TPCCRUNNER_INCLUDE_DIRECTORIES})
target_link_libraries(hello_test PRIVATE ${TPCCRUNNER_LINK_LIBRARIES})
target_link_options(hello_test PUBLIC "-pthread" )






