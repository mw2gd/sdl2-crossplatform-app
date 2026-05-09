add_executable(app
    main.cpp
)

install(TARGETS app DESTINATION ${PROJECT_SOURCE_DIR}/out/bin)

if(EMSCRIPTEN)
    install(FILES
        "$<TARGET_FILE_DIR:app>/app.js"
        "$<TARGET_FILE_DIR:app>/app.wasm"
        DESTINATION ${PROJECT_SOURCE_DIR}/out/bin)
endif()

# SDL must be added before renderloop because renderloop links against SDL2.
add_subdirectory(${PROJECT_SOURCE_DIR}/ext/SDL2-2.30.2 ${PROJECT_SOURCE_DIR}/ext/SDL2-2.30.2/build)
add_subdirectory(${PROJECT_SOURCE_DIR}/src/renderloop)

target_include_directories(app PRIVATE ${PROJECT_SOURCE_DIR}/src/renderloop)

target_link_libraries(app PRIVATE renderloop)
