cmake_minimum_required(VERSION 3.14)
project(ddl_gen)

if (CMAKE_VERSION VERSION_LESS 3.0.0)
    include(CheckCXXCompilerFlag)
    check_cxx_compiler_flag(-std=c++14 COMPILER_SUPPORTS_CXX14)
    check_cxx_compiler_flag(-std=c++0x COMPILER_SUPPORTS_CXX0X)
    if (COMPILER_SUPPORTS_CXX14)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
    elseif (COMPILER_SUPPORTS_CXX0X)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
    endif ()
else ()
    SET(CMAKE_CXX_STANDARD 14)
    SET(CMAKE_CXX_STANDARD_REQUIRED ON)
endif ()

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

include_directories(include)

add_library(ddl_gen
        ddl_excel_parser.cpp
        ddl_exception.cpp
        ddl_table.cpp
        ddl_table_column.cpp
        ddl_table_sql_generator.cpp include/ddl_gen/ddl_gen.h)

target_link_libraries(ddl_gen ${CONAN_LIBS})

add_executable(ddl_gen_cli main.cpp)
target_link_libraries(ddl_gen_cli ddl_gen ${CONAN_LIBS})

enable_testing()

add_executable(TestAddTest test/test_add.cpp)
target_link_libraries(TestAddTest ${CONAN_LIBS})

add_test(
        NAME TestAddTest
        COMMAND TestAddTest
)
