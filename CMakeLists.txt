cmake_minimum_required(VERSION 3.5)

project(App VERSION 1.0 LANGUAGES C CXX)

# Set the few platform-specific options before adding any build targets.
if(EMSCRIPTEN)
    add_link_options(--shell-file ${PROJECT_SOURCE_DIR}/simple_shell.html)
    set(CMAKE_EXECUTABLE_SUFFIX ".html")
elseif(APPLE)
    add_compile_definitions(APPLE=1)
elseif(WIN32)
    add_compile_definitions(WIN32=1)
elseif(UNIX)
    add_compile_definitions(UNIX=1)
endif()

add_subdirectory(src)
