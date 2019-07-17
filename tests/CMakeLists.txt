cmake_minimum_required(VERSION 3.1)

project(graph_tests)


configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
                 ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
                 EXCLUDE_FROM_ALL)

# The gtest/gtest_main targets carry header search path
# dependencies automatically when using CMake 2.8.11 or
# later. Otherwise we have to add them here ourselves.
if (CMAKE_VERSION VERSION_LESS 2.8.11)
  include_directories("${gtest_SOURCE_DIR}/include")
endif()

# Now simply link against gtest or gtest_main as needed. Eg
#add_executable(example example.cpp)
#target_link_libraries(example gtest_main)
#add_test(NAME example_test COMMAND example)



add_subdirectory(../external_library ${CMAKE_CURRENT_BINARY_DIR}/external_library)

set(CMAKE_BUILD_TYPE Release)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/../cmake)

# specific version of eigen needed
find_package(Eigen3 3.2.10 EXACT REQUIRED)

include_directories(${EIGEN3_INCLUDE_DIR})

# libigl
option(LIBIGL_WITH_OPENGL            "Use OpenGL"                  ON)
option(LIBIGL_WITH_OPENGL_GLFW       "Use GLFW"                    ON)
#option(LIBIGL_WITH_OPENGL_GLFW_IMGUI "Use ImGui"          ON)
option(LIBIGL_WITH_VIEWER            "Use OpenGL viewer"            ON)
#option(LIBIGL_USE_STATIC_LIBRARY     "Use LibIGL as static library" ON)

find_package(LIBIGL REQUIRED QUIET)
#find_package(OpenGL REQUIRED)

file(GLOB SRCFILES *.cpp)

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
			igl::opengl_glfw
			gtest_main)

add_test(NAME example_test COMMAND example)
