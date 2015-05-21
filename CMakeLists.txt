project(DCSC)

cmake_minimum_required(VERSION 2.8)


find_package(MPI REQUIRED)

# Legg til mpi til include path
include_directories(${MPI_C_INCLUDE_PATH})

# Sett MPI flagg
add_definitions(${MPI_C_COMPILE_FLAGS})

# Sett define for MPI-støtte
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DENABLE_MPI=1")

# Legg til c99
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")

# Liste over kildekode for DCSC-applikasjonen
set(SOURCES DCSC.c
	    graph_utils.c
	    functions_DCSC.c
            )

# Legg til DCSCapp
add_executable(DCSC_exec ${SOURCES})

# Lenk bader mot MPI og lib.m
target_link_libraries(DCSC_exec m)

target_link_libraries(DCSC_exec ${MPI_C_LIBRARIES})
