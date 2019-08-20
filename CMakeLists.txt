cmake_minimum_required(VERSION 3.1)

project(libGraphCpp)


### Configure output locations
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

### add subdirectories
add_subdirectory(external_library)
add_subdirectory(external_library/polyscope)

### CMAKE settings
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

### find eigen
find_package(Eigen3 3.2.10 EXACT REQUIRED)
include_directories(${EIGEN3_INCLUDE_DIR})


include_directories(include)

file(GLOB SRCFILES app/*.cpp)

add_executable(${PROJECT_NAME}_bin ${SRCFILES})

set_target_properties(${PROJECT_NAME}_bin PROPERTIES CXX_STANDARD 11 CXX_STANDARD_REQUIRED YES)

include_directories(${PROJECT_NAME}_bin
			${PROJECT_SOURCE_DIR}/external_library/
			${YAML_CPP_INCLUDE_DIR}
			${EIGEN3_INCLUDE_DIR}
			)

target_link_libraries(${PROJECT_NAME}_bin 
			${YAML_CPP_LIBRARIES}
			polyscope
			)
