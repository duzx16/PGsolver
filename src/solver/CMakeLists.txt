set(HEADERS
        ${HEADERS}
        ${CMAKE_CURRENT_SOURCE_DIR}/Solver.h
        ${CMAKE_CURRENT_SOURCE_DIR}/EigenSolver.h
        ${CMAKE_CURRENT_SOURCE_DIR}/JacobiSolver.h
        PARENT_SCOPE
        )

set(SOURCE
        ${SOURCE}
        ${CMAKE_CURRENT_SOURCE_DIR}/EigenSolver.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/JacobiSolver.cpp
        PARENT_SCOPE
        )