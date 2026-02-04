cmake_minimum_required(VERSION 4.1.0)
set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CXX_COMPILER "g++")
project(Galaga VERSION 1.0.0)
cmake_policy(SET CMP0072 NEW)

find_package(OpenGL REQUIRED)

add_executable(main 
src/config.h 
src/main.cpp
dependencies/glad/glad.c
src/shaderClass.cpp
src/VBOClass.cpp
src/EBOClass.cpp
src/VAOClass.cpp
dependencies/stb/stb.cpp
src/TextureUtil.cpp
src/Window.cpp
src/game_objects.cpp
src/Position2D.cpp
src/debug_manager.cpp
src/galaga_objects.cpp
src/path_objects.cpp
src/game_ui.cpp
src/BarManager.cpp

)

target_include_directories(main PRIVATE dependencies headers)


target_link_directories(main PRIVATE lib)

target_link_libraries(main glfw3 OpenGL::GL)




add_executable(stage_editor 
src/config.h 
src/stage_editor.cpp
dependencies/glad/glad.c
src/shaderClass.cpp
src/VBOClass.cpp
src/EBOClass.cpp
src/VAOClass.cpp
dependencies/stb/stb.cpp
src/TextureUtil.cpp
src/Window.cpp
src/game_objects.cpp
src/Position2D.cpp
src/debug_manager.cpp
src/galaga_objects.cpp
src/path_objects.cpp
src/path_developer.cpp
src/game_ui.cpp
src/BarManager.cpp

dependencies/imgui/imgui.cpp
dependencies/imgui/imgui_draw.cpp
dependencies/imgui/imgui_tables.cpp
dependencies/imgui/imgui_widgets.cpp
dependencies/imgui/backends/imgui_impl_glfw.cpp
dependencies/imgui/backends/imgui_impl_opengl3.cpp
)


target_include_directories(stage_editor PRIVATE 
dependencies 
headers
dependencies/imgui
dependencies/imgui/backends)


target_link_directories(stage_editor PRIVATE lib)

target_link_libraries(stage_editor glfw3 OpenGL::GL)