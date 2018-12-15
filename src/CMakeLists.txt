cmake_minimum_required(VERSION 3.13)
find_package(Eigen3)
include_directories(${EIGEN3_INCLUDE_DIR})

add_subdirectory(solver)

add_library(${CMAKE_PROJECT_NAME}_lib ${SOURCE} ${HEADERS})

if (USE_OPENMP)
    add_definitions(-DUSE_OPENMP)
    find_package(OpenMP REQUIRED)
    message(${OpenMP_FOUND})
    target_link_libraries(${CMAKE_PROJECT_NAME}_lib OpenMP::OpenMP_CXX)
endif ()

add_executable(${CMAKE_PROJECT_NAME} main.cpp Parser.cpp Elements.cpp PGSolver.cpp Stamping.cpp)
add_executable(Verification Verification.cpp)

target_link_libraries(${CMAKE_PROJECT_NAME} ${CMAKE_PROJECT_NAME}_lib)