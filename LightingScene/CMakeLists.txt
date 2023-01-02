cmake_minimum_required(VERSION 3.2)
project(scene-viewer)

set(CMAKE_CXX_STANDARD 14)

if (POLICY CMP0072)
  set(OpenGL_GL_PREFERENCE LEGACY)
endif()

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

include_directories(${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS})

add_executable(scene scene.cpp)
target_link_libraries(scene ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})
target_compile_options(scene PUBLIC -Wno-deprecated-declarations)

# file(COPY 
#     ${CMAKE_CURRENT_SOURCE_DIR}/models
#     DESTINATION 
#     ${CMAKE_CURRENT_BINARY_DIR}/
# )