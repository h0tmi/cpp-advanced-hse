cmake_minimum_required(VERSION 3.18)

project(vcpkg-deps)

set(CMAKE_CXX_STANDARD 20)

# Find packages installed via `vcpkg`.
find_package(imgui CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(GLEW REQUIRED)

add_executable(main
  src/main.cpp
  src/opengl_shader.cpp
  src/file_manager.cpp

  bindings/imgui_impl_glfw.cpp
  bindings/imgui_impl_opengl3.cpp

  assets/simple-shader.vs
  assets/simple-shader.fs
)

add_custom_command(TARGET main
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/assets/simple-shader.vs ${PROJECT_BINARY_DIR}
  COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/assets/simple-shader.fs ${PROJECT_BINARY_DIR}
)

target_link_libraries(main
  PRIVATE imgui::imgui glfw GLEW::GLEW
)
target_compile_definitions(main PUBLIC IMGUI_IMPL_OPENGL_LOADER_GLEW)
