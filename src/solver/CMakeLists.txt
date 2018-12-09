set(HEADERS
        ${HEADERS}
        ${CMAKE_CURRENT_SOURCE_DIR}/Solver.h
        ${CMAKE_CURRENT_SOURCE_DIR}/EigenSolver.h
        ${CMAKE_CURRENT_SOURCE_DIR}/SORSolver.h
        ${CMAKE_CURRENT_SOURCE_DIR}/GaussSolver.h
        PARENT_SCOPE
        )

set(SOURCE
        ${SOURCE}
        ${CMAKE_CURRENT_SOURCE_DIR}/EigenSolver.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/SORSolver.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/GaussSolver.cpp
        PARENT_SCOPE
        )