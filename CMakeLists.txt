cmake_minimum_required(VERSION 3.8)

project(TTFViewer)

add_subdirectory(SDL EXCLUDE_FROM_ALL)

file(GLOB_RECURSE SOURCES RELATIVE ${CMAKE_SOURCE_DIR} CONFIGURE_DEPENDS "source/*.c")
file(GLOB_RECURSE HEADERS RELATIVE ${CMAKE_SOURCE_DIR} CONFIGURE_DEPENDS "include/*.h")
add_executable(TTFViewer WIN32 ${SOURCES} ${HEADERS})
target_include_directories(TTFViewer PRIVATE "include")

# Adding SDL
target_include_directories(TTFViewer PRIVATE SDL/include)
if(TARGET SDL2::SDL2main)
    target_link_libraries(TTFViewer PRIVATE SDL2::SDL2main)
endif()
target_link_libraries(TTFViewer PRIVATE SDL2::SDL2)