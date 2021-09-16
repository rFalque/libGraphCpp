cmake_minimum_required(VERSION 3.11)

project(graph_tests)

enable_testing()
find_package(GTest REQUIRED)
find_package(Eigen3 REQUIRED)
find_package(yaml-cpp REQUIRED)

include_directories(../include)

add_executable(${PROJECT_NAME}_bin graph_tests.cpp)

set_target_properties(${PROJECT_NAME}_bin PROPERTIES CXX_STANDARD 11 CXX_STANDARD_REQUIRED YES)

include_directories(${PROJECT_NAME}_bin
			${YAML_CPP_INCLUDE_DIR}
			${EIGEN3_INCLUDE_DIR}
      )

target_link_libraries(${PROJECT_NAME}_bin
			${YAML_CPP_LIBRARIES}
			GTest::GTest
      )
