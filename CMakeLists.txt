cmake_minimum_required(VERSION 3.1)

project(libGraphCpp)

SET(INCLUDE_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/include/")
SET(SRC_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/src/")

find_package(Eigen3 REQUIRED)
find_package(yaml-cpp REQUIRED)

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
target_include_directories(libGraphCpp PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include/embedded_deformation")
target_include_directories(libGraphCpp PRIVATE ${EIGEN3_INCLUDE_DIR})
target_include_directories(libGraphCpp PRIVATE ${YAML_CPP_INCLUDE_DIR})

target_link_libraries(libGraphCpp ${YAML_CPP_LIBRARIES} )
target_link_libraries(libGraphCpp polyscope)
