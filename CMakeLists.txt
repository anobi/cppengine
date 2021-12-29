cmake_minimum_required(VERSION 3.13)
project(Cppengine)
set(CMAKE_CXX_STANDARD 17)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIRS})

add_executable(${PROJECT_NAME} src/main.cpp)

target_link_libraries(${PROJECT_NAME}
    ${SDL2_LIBRARIES}
    ${GLEW_LIBRARIES}
    GLEW::GLEW
)

target_sources(${PROJECT_NAME} PUBLIC
    external/imgui/imgui.cpp
    external/imgui/imgui_draw.cpp
    external/imgui/backends/imgui_impl_sdl.cpp
    src/main.cpp
    src/game.cpp
    src/display.cpp
    src/renderer.cpp
    src/input.cpp
    src/controls.cpp
    src/world.cpp
    src/entities/entity_transforms.cpp
    src/entities/entity_light_components.cpp
    src/entities/entity_model_components.cpp
    src/entities/entity_manager.cpp
    src/rendering/material_manager.cpp
    src/rendering/model_manager.cpp
    src/loading/model_loader.cpp
    src/shader.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC
    external/glm
    external/stb
    external/imgui
    external/imgui/backends
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/
)
