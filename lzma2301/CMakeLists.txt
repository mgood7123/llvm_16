cmake_minimum_required(VERSION 3.5)

project(7z LANGUAGES C)

file(GLOB HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/C/*.h)
file(GLOB SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/C/*.c)

add_library(7z STATIC ${SOURCES})
target_compile_definitions(7z PUBLIC Z7_AFFINITY_DISABLE)
target_include_directories(7z PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/C)

add_library(7z_shared SHARED ${SOURCES})
target_compile_definitions(7z_shared PUBLIC Z7_AFFINITY_DISABLE)
target_include_directories(7z_shared PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/C)

set_target_properties(7z_shared PROPERTIES
    SOVERSION "23"
    VERSION "23.01"
    OUT_NAME "7z"
)

set(INSTALL_BIN_DIR "${CMAKE_INSTALL_PREFIX}/bin" CACHE PATH "Installation directory for executables")
set(INSTALL_LIB_DIR "${CMAKE_INSTALL_PREFIX}/lib" CACHE PATH "Installation directory for libraries")
set(INSTALL_INC_DIR "${CMAKE_INSTALL_PREFIX}/include" CACHE PATH "Installation directory for headers")

install(TARGETS 7z 7z_shared
    RUNTIME DESTINATION "${INSTALL_BIN_DIR}"
    ARCHIVE DESTINATION "${INSTALL_LIB_DIR}"
    LIBRARY DESTINATION "${INSTALL_LIB_DIR}" )

install(FILES ${HEADERS} DESTINATION "${INSTALL_INC_DIR}")

