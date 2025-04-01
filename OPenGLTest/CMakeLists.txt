cmake_minimum_required(VERSION 3.5)
project(OpenGLTest VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

add_subdirectory(deps/glfw)

add_subdirectory(deps/glm)

file(GLOB ASSETS "./assets")
file(COPY ${ASSETS} DESTINATION ${CMAKE_BINARY_DIR})

set(GLAD_SOURCES
    deps/glad/src/glad.c
)

set(APPLICATION_SOURCES
    src/application/Application.cpp
    src/application/camera/Camera.cpp
    src/application/camera/OrthographicCamera.cpp
    src/application/camera/PerspectiveCamera.cpp
    src/application/camera/CameraControl.cpp
    src/application/camera/TrackBallController.cpp
)

set(GLFRAMEWORK_SOURCES
    src/glframework/shader.cpp
    src/glframework/texture.cpp
)

set(SOURCES
    src/main.cpp
    ${GLAD_SOURCES}
    ${APPLICATION_SOURCES}
    ${GLFRAMEWORK_SOURCES}
)

add_executable(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME}
    PRIVATE deps
    PRIVATE deps/glad/include
    PRIVATE deps/glfw/include
    PRIVATE deps/glm
    PRIVATE deps/stb
)
target_link_directories(${PROJECT_NAME} PRIVATE deps)
target_link_libraries(${PROJECT_NAME}
    glfw
)