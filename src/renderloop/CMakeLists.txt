add_library(renderloop STATIC)

target_link_libraries(renderloop PUBLIC SDL2::SDL2-static)

target_sources(renderloop PRIVATE
    renderloop.cpp
    renderloop.hpp
)
