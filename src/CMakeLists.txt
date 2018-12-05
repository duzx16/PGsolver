find_package(Eigen3)
include_directories(${EIGEN3_INCLUDE_DIR})

add_subdirectory(solver)

add_library(${CMAKE_PROJECT_NAME}_lib ${SOURCE} ${HEADERS})
add_executable(${CMAKE_PROJECT_NAME} main.cpp Parser.cpp Elements.cpp PGSolver.cpp Stamping.cpp)
add_executable(Verification Verification.cpp)
target_link_libraries(${CMAKE_PROJECT_NAME} ${CMAKE_PROJECT_NAME}_lib)