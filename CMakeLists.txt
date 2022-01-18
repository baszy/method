cmake_minimum_required(VERSION 3.0)

project(method VERSION 0.0.0 LANGUAGES CXX DESCRIPTION "")

# Third-party libraries
add_subdirectory("lib/")

# Include libraries using pkg-config
find_package(PkgConfig REQUIRED)
pkg_check_modules(GL REQUIRED gl)
pkg_check_modules(SDL2 REQUIRED sdl2 SDL2_image)
pkg_check_modules(X11 REQUIRED x11 xi)
pkg_check_modules(PNG REQUIRED libpng)

# Main library
add_subdirectory("src/")
add_subdirectory("include/")

add_compile_options(-g -Wall -pedantic -fno-omit-frame-pointer)

add_library(method ${METHOD_SOURCES})
set_target_properties(method PROPERTIES LINKER_LANGUAGE CXX CXX_STANDARD 11
    POSITION_INDEPENDENT_CODE ON)

target_include_directories(method PUBLIC ${GL_INCLUDE_DIRS}
    ${SDL2_INCLUDE_DIRS} ${X11_INCLUDE_DIRS} ${PNG_INCLUDE_DIRS}
    ${GLAD_INCLUDE_DIRS} ${METHOD_INCLUDE_DIRS})
target_link_libraries(method PRIVATE ${GL_LIBRARIES} ${SDL2_LIBRARIES}
    ${X11_LIBRARIES} ${PNG_LIBRARIES} ${GLAD_LIBRARIES} ${CMAKE_DL_LIBS} m pthread)
target_compile_options(method PRIVATE ${GL_CFLAGS_OTHER} ${SDL2_CFLAGS_OTHER}
    ${X11_CFLAGS_OTHER})

# Tools
add_subdirectory("tools/")

add_executable(method_builder ${METHOD_BUILDER_SOURCES})
set_target_properties(method_builder PROPERTIES LINKER_LANGUAGE CXX
    CXX_STANDARD 11)

target_include_directories(method_builder PUBLIC "tools/"
    ${SDL_INCLUDE_DIRS} ${SDL_IMAGE_INCLUDE_DIRS})
target_link_libraries(method_builder PRIVATE method
    ${SDL_LIBRARIES} ${SDL_IMAGE_LIBRARIES})
target_compile_options(method_builder PRIVATE ${SDL2_CFLAGS_OTHER})

# Example project
add_subdirectory("example/")

add_executable(method_example ${METHOD_EXAMPLE_SOURCES})
set_target_properties(method_example PROPERTIES LINKER_LANGUAGE CXX
    CXX_STANDARD 11)

target_include_directories(method_example PUBLIC "example/")
target_link_libraries(method_example PRIVATE method)
target_compile_options(method_example PRIVATE)

# Generating documentation using Doxygen
find_package(Doxygen)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-exceptions")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti")

if(DOXYGEN_FOUND)
    set(DOXYGEN_PROJECT_NAME ${PROJECT_NAME})
    set(DOXYGEN_PROJECT_NUMBER ${PROJECT_VERSION})
    set(DOXYGEN_PROJECT_BRIEF ${PROJECT_DESCRIPTION})

    set(DOXYGEN_RECURSIVE YES)
    set(DOXYGEN_FULL_PATH_NAMES YES)

    set(DOXYGEN_EXTRACT_ALL YES)
    set(DOXYGEN_EXTRACT_PRIVATE YES)
    set(DOXYGEN_EXTRACT_STATIC YES)

    set(DOXYGEN_TAB_SIZE 4)
    # set(DOXYGEN_HTML_COLORSTYLE_HUE 20)
    # set(DOXYGEN_HTML_COLORSTYLE_SAT 140)
    # set(DOXYGEN_HTML_COLORSTYLE_GAMMA 80)

    set(DOXYGEN_GENERATE_HTML YES)

    # uncomment for doxygen build
    # doxygen_add_docs(doxygen_documetation ALL ${METHOD_INCLUDE_DIRS}
    #     ${METHOD_SOURCE_DIRS} ${METHOD_DOXYGEN_DOX_DIRS})
else()
    message("Doxygen must be installed to generate documentation")
endif()
