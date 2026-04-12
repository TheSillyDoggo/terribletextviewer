cmake_minimum_required(VERSION 3.11)
set(CMAKE_CXX_STANDARD 20)

project(glfwtest)


find_package(CURL REQUIRED)
include(FindPkgConfig)

find_package(glfw3 CONFIG REQUIRED)
find_package(OpenGL REQUIRED)

include(cmake/CPM.cmake)
include(FetchContent)

FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG docking
)

FetchContent_Declare(
    fmt
    GIT_REPOSITORY https://github.com/fmtlib/fmt.git
    GIT_TAG 12.1.0
)

CPMAddPackage("gh:prevter/tint@1.0.0")

FetchContent_MakeAvailable(imgui)
FetchContent_MakeAvailable(fmt)


file(GLOB SOURCES
    include/src/*.cpp
    src/*.cpp
	src/*/*.cpp
	src/*/*/*.cpp
	src/*/*/*/*.cpp

    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${IMGUI_BACKEND_FILES}
    ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
    ${imgui_SOURCE_DIR}/imgui_demo.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC
    ${imgui_SOURCE_DIR}
    ${imgui_SOURCE_DIR}/backends
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    glfw
    OpenGL::GL
    fmt
    tint::tint
)

if(WIN32)
    target_compile_definitions(${PROJECT_NAME} PRIVATE PLATFORM_WINDOWS)
elseif(APPLE)
    target_compile_definitions(${PROJECT_NAME} PRIVATE PLATFORM_MACOS)
elseif(LINUX)
    target_compile_definitions(${PROJECT_NAME} PRIVATE PLATFORM_LINUX)
endif()

include_directories(src/ src/Engine)