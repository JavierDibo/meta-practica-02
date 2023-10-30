cmake_minimum_required(VERSION 3.26)
project(meta_practica_02)

set(CMAKE_CXX_STANDARD 17)

add_executable(meta_practica_02 main.cpp
        LectorDatosCiudades.cpp
        LectorDatosCiudades.h
        LectorDatosCiudades.h
        globals.h
        globals.cpp
        Reloj.cpp
        Reloj.h
        LectorParametros.cpp
        LectorParametros.h
        Logger.cpp
        Logger.h
)

find_package(OpenMP)
if (OPENMP_FOUND)
    target_link_libraries(meta_practica_02 PUBLIC OpenMP::OpenMP_CXX)
endif ()
