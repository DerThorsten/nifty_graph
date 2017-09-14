cmake_minimum_required(VERSION 3.1)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules)

SET(PROJECT_NAME "nifty_graph")
project(nifty_graph)


SET(MACRO_PREFIX NIFTY_GRAPH)

set (NIFTY_GRAPH_VERSION_MAJOR 0 )
set (NIFTY_GRAPH_VERSION_MINOR 1 )
set (NIFTY_GRAPH_VERSION_PATCH 0 )




set (NIFTY_GRAPH_VERSION_SHORT_STR 
    "${${MACRO_PREFIX}_VERSION_MAJOR}.${${MACRO_PREFIX}_VERSION_MINOR}"
)

set (NIFTY_GRAPH_VERSION_STR 
    "${${MACRO_PREFIX}_VERSION_MAJOR}.${${MACRO_PREFIX}_VERSION_MINOR}.${${MACRO_PREFIX}_VERSION_PATCH}"
)


include(CheckCXXCompilerFlag)


SET(BUILD_TESTS    ON CACHE BOOL "Build the test suite ?"   FORCE)
SET(BUILD_DOCS     ON CACHE BOOL "Build the documentation?" FORCE)
SET(BUILD_EXAMPLES ON CACHE BOOL "Build the examples?"      FORCE)

#-------------------------------------------------------------------------------------------------------------------
# cmake packages
#-------------------------------------------------------------------------------------------------------------------
include(ExternalProject)

#-------------------------------------------------------------------------------------------------------------------
# check for c++ 14 support
#-------------------------------------------------------------------------------------------------------------------

# FIXME c++14 checks are broken???
if (CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    CHECK_CXX_COMPILER_FLAG("-std=c++14" HAS_CPP14_FLAG)
    #CHECK_CXX_COMPILER_FLAG("-std=c++11" HAS_CPP11_FLAG)
    if (HAS_CPP14_FLAG)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
    else()
        message(FATAL_ERROR "Unsupported compiler -- multi-array  requires C++14 support!")
    endif()
endif()


if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
endif()

#-------------------------------------------------------------------------------------------------------------------
# threading package
#-------------------------------------------------------------------------------------------------------------------
find_package(Threads REQUIRED)

#-------------------------------------------------------------------------------------------------------------------
# include include files
#-------------------------------------------------------------------------------------------------------------------
file(GLOB_RECURSE headers include/*.hxx)
include_directories(include)


#-------------------------------------------------------------------------------------------------------------------
# external projects which need to be in the `include tree`
#-------------------------------------------------------------------------------------------------------------------


#---------------------------------------------------
# Boost header Only
#---------------------------------------------------
ExternalProject_Add(
    external_boost_header_only
    PREFIX ${CMAKE_BINARY_DIR}/external_projects/boost
    GIT_REPOSITORY https://github.com/NumScale/boost-header-only
    GIT_TAG 3a7ee7dfafa2a484511289aab5a4f5344bdcf8d7
    TIMEOUT 5
    #UPDATE_COMMAND ${GIT_EXECUTABLE} pull
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
    LOG_DOWNLOAD ON
)
# Expose required variable (DOCTEST_INCLUDE_DIR) to parent scope
ExternalProject_Get_Property(external_boost_header_only source_dir)
set(BOOST_INCLUDE_DIR ${source_dir}/boost-1.64.0/ CACHE INTERNAL "Path to include folder for boost")
include_directories(${BOOST_INCLUDE_DIR})


#---------------------------------------------------
# GSL: Guideline Support Library  
#---------------------------------------------------
ExternalProject_Add(
    external_gsl
    PREFIX ${CMAKE_BINARY_DIR}/external_projects/GSL
    GIT_REPOSITORY https://github.com/Microsoft/GSL
    GIT_TAG b01450878b39307df9abb702f02ce8c1d6c9ee30
    TIMEOUT 5
    #UPDATE_COMMAND ${GIT_EXECUTABLE} pull
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
    LOG_DOWNLOAD ON
)
# Expose required variable (DOCTEST_INCLUDE_DIR) to parent scope
ExternalProject_Get_Property(external_gsl source_dir)
set(GSL_INCLUDE_DIR ${source_dir}/include/ CACHE INTERNAL "Path to include folder for GSL")
include_directories(${GSL_INCLUDE_DIR})

#---------------------------------------------------
# xtensor 
#---------------------------------------------------
ExternalProject_Add(
    external_xtensor
    PREFIX ${CMAKE_BINARY_DIR}/external_projects/xtensor
    GIT_REPOSITORY https://github.com/QuantStack/xtensor
    GIT_TAG 0.10.7
    TIMEOUT 5
    #UPDATE_COMMAND ${GIT_EXECUTABLE} pull
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
    LOG_DOWNLOAD ON
)
# Expose required variable (DOCTEST_INCLUDE_DIR) to parent scope
ExternalProject_Get_Property(external_xtensor source_dir)
set(XTENSOR_INCLUDE_DIR ${source_dir}/include/ CACHE INTERNAL "Path to include folder for xtensor")
include_directories(${XTENSOR_INCLUDE_DIR})

#---------------------------------------------------
# nifty-tools 
#---------------------------------------------------
ExternalProject_Add(
    external_nifty_tools
    PREFIX ${CMAKE_BINARY_DIR}/external_projects/nifty_tools
    GIT_REPOSITORY https://github.com/DerThorsten/nifty_tools
    GIT_TAG master
    TIMEOUT 5
    #UPDATE_COMMAND ${GIT_EXECUTABLE} pull
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
    LOG_DOWNLOAD ON
)
# Expose required variable (DOCTEST_INCLUDE_DIR) to parent scope
ExternalProject_Get_Property(external_nifty_tools source_dir)
set(NIFTY_TOOLS_INCLUDE_DIR ${source_dir}/include/ CACHE INTERNAL "Path to include folder for nifty_tools")
include_directories(${NIFTY_TOOLS_INCLUDE_DIR})



# meta target for all to handle dependencies
add_custom_target(externl_headers)
add_dependencies(externl_headers
    external_boost_header_only
    external_gsl 
    external_xtensor
    external_nifty_tools
)







#-------------------------------------------------------------------------------------------------------------------
# enable `make test` in root dir
#-------------------------------------------------------------------------------------------------------------------
if(BUILD_TESTS)
    enable_testing()
endif()

#------------------------------------------------------------------------------------------------------------------
# src folder
#-------------------------------------------------------------------------------------------------------------------
add_subdirectory(src)

