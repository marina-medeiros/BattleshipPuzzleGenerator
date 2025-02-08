cmake_minimum_required(VERSION 3.5)
project (Battleship_Puzzle_Generator VERSION 1.0.0 LANGUAGES CXX )

#=== FINDING PACKAGES ===#
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

#=== SETTING VARIABLES ===#
# Appending to existing flags the correct way (two methods)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic ")
# -fstandalone-debug

#=== SETTING VARIABLES ===#
# Application name
set ( APP_NAME "bpg")
# Compiling flags
set( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COMPILE_FLAGS} ${PREPROCESSING_FLAGS}" )
include_directories(src/include)

#=== Main App ===
#Can manually add the sources using the set command as follows:
# src/bpg.cpp
file(GLOB SOURCES "src/*.cpp")
add_executable(${APP_NAME} ${SOURCES})
target_compile_features( ${APP_NAME}  PUBLIC cxx_std_17 )

# # Uncomment this if you need to debug strings with lldb.
# target_compile_options(${APP_NAME} PRIVATE -fstandalone-debug)

find_program(VALGRIND_EXECUTABLE valgrind)

if(VALGRIND_EXECUTABLE)
    add_custom_target(valgrind
        COMMAND "${VALGRIND_EXECUTABLE}" --leak-check=full --show-leak-kinds=all --error-exitcode=1 ${CMAKE_BINARY_DIR}/seu_executavel
        DEPENDS seu_executavel
        COMMENT "Running Valgrind"
    )
endif()
