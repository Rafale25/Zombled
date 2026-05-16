cmake_minimum_required(VERSION 3.25.0)

set(PROJECT_NAME "Zombled")
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_SCAN_FOR_MODULES OFF)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

project(${PROJECT_NAME})

file(GLOB_RECURSE SRC_C CONFIGURE_DEPENDS ./src/*.c)
file(GLOB_RECURSE SRC_CPP CONFIGURE_DEPENDS ./src/*.cpp)
file(GLOB_RECURSE SRC_HEADERS CONFIGURE_DEPENDS ./src/*.hpp)

add_executable(${PROJECT_NAME} ${SRC_CPP} ${SRC_C} ${SRC_HEADERS})

set(THIRD_PARTY_DIR ./libs/)

# find_package(Vulkan REQUIRED)

target_include_directories(${PROJECT_NAME}
    PRIVATE
        # ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# target_link_libraries(imgui PUBLIC Vulkan::Vulkan glfw)

target_link_libraries(${PROJECT_NAME} PRIVATE
    # WishEngine
)

target_compile_definitions(${PROJECT_NAME} PRIVATE
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "./"
)
