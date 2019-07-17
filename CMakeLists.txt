cmake_minimum_required(VERSION 3.1)

project(libGraphCpp)

add_subdirectory(external_library)

set(CMAKE_BUILD_TYPE Release)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# specific version of eigen needed
find_package(Eigen3 3.2.10 EXACT REQUIRED)
include_directories(${EIGEN3_INCLUDE_DIR})

# libigl
option(LIBIGL_WITH_OPENGL            "Use OpenGL"         ON)
option(LIBIGL_WITH_OPENGL_GLFW       "Use GLFW"           ON)
#option(LIBIGL_WITH_OPENGL_GLFW_IMGUI "Use ImGui"          ON)
option(LIBIGL_WITH_VIEWER            "Use OpenGL viewer"  ON)
#option(LIBIGL_USE_STATIC_LIBRARY     "Use LibIGL as static library" ON)

find_package(LIBIGL REQUIRED QUIET)
#find_package(OpenGL REQUIRED)

include_directories(include)

file(GLOB SRCFILES app/*.cpp)

add_executable(${PROJECT_NAME}_bin ${SRCFILES})

set_target_properties(${PROJECT_NAME}_bin PROPERTIES CXX_STANDARD 11 CXX_STANDARD_REQUIRED YES)

include_directories(${PROJECT_NAME}_bin
			${PROJECT_SOURCE_DIR}/external_library/
			${YAML_CPP_INCLUDE_DIR}
			${EIGEN3_INCLUDE_DIR})

target_link_libraries(${PROJECT_NAME}_bin 
			${YAML_CPP_LIBRARIES}
			igl::core
			igl::opengl
			igl::opengl_glfw)



set(INCLUDE_INSTALL_ROOT_DIR include)
set(INCLUDE_INSTALL_DIR ${INCLUDE_INSTALL_ROOT_DIR}/libGraphCpp)
set(header_directory "include/libGraphCpp/")

#install(TARGETS libGraphCpp EXPORT libGraphCpp-targets ${_INSTALL_DESTINATIONS})
install(
	DIRECTORY ${header_directory}
	DESTINATION ${INCLUDE_INSTALL_DIR}
	FILES_MATCHING PATTERN "*.hpp"
)
