cmake_minimum_required(VERSION 3.17)

project(manazir
        VERSION 1.0
        DESCRIPTION "A physically based ray tracer"
        LANGUAGES CXX
)

include(FetchContent)
FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG 1.0.2
)
FetchContent_MakeAvailable(glm)

set(CMAKE_CXX_STANDARD 20)

add_executable(manazir main.cpp)
target_link_libraries(manazir PRIVATE glm::glm)
