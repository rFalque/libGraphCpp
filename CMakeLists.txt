cmake_minimum_required(VERSION 3.11)

project(libGraphCpp)

### Configure output locations
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

### CMAKE settings
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)


### add polyscope
message("get polyscope")
include(FetchContent)
FetchContent_Declare(
    polyscope

    GIT_REPOSITORY https://github.com/nmwsharp/polyscope.git
    GIT_TAG        56b82cd
    SOURCE_DIR "${CMAKE_SOURCE_DIR}/polyscope"
)
FetchContent_Populate(polyscope)

add_subdirectory(polyscope)

find_package(Eigen3 REQUIRED)
find_package(yaml-cpp REQUIRED)

SET(INCLUDE_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/include/")
SET(SRC_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/src/")

set(SRCS
    # libGraphCpp
)

set(HEADERS
    # libGraphCpp
    ${INCLUDE_ROOT}/libGraphCpp/graph.hpp
    ${INCLUDE_ROOT}/libGraphCpp/graphOptions.hpp
    ${INCLUDE_ROOT}/libGraphCpp/plotGraph.hpp
    ${INCLUDE_ROOT}/libGraphCpp/polyscopeWrapper.hpp
    ${INCLUDE_ROOT}/libGraphCpp/readGraphOBJ.hpp
    ${INCLUDE_ROOT}/libGraphCpp/writeGraphOBJ.hpp
)

add_library(libGraphCpp ${SRCS} ${HEADERS})
set_target_properties(libGraphCpp PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories(libGraphCpp PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_include_directories(libGraphCpp PRIVATE ${EIGEN3_INCLUDE_DIR})
target_include_directories(libGraphCpp PRIVATE ${YAML_CPP_INCLUDE_DIR})

target_link_libraries(libGraphCpp ${YAML_CPP_LIBRARIES} )
target_link_libraries(libGraphCpp polyscope)


file(GLOB SRCFILES app/*.cpp)

add_executable(${PROJECT_NAME}_bin ${SRCFILES})

set_target_properties(${PROJECT_NAME}_bin PROPERTIES CXX_STANDARD 11 CXX_STANDARD_REQUIRED YES)

include_directories(${PROJECT_NAME}_bin
			${EIGEN3_INCLUDE_DIR}
			)

target_link_libraries(${PROJECT_NAME}_bin 
            libGraphCpp
			polyscope
			yaml-cpp
			)
