cmake_minimum_required(VERSION 3.25.0)

set(PROJECT_NAME "Zombled")
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_SCAN_FOR_MODULES OFF)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

project(${PROJECT_NAME})

file(GLOB_RECURSE SRC_C CONFIGURE_DEPENDS ./src/*.c)
file(GLOB_RECURSE SRC_CPP CONFIGURE_DEPENDS
    ./src/*.cpp
    ./libs/uhcstd/*.cc
)
file(GLOB_RECURSE SRC_HEADERS CONFIGURE_DEPENDS ./src/*.hpp)

add_compile_options(-fsanitize=address -fno-omit-frame-pointer -g)
add_link_options(-fsanitize=address)

add_executable(${PROJECT_NAME} ${SRC_CPP} ${SRC_C} ${SRC_HEADERS})

set(LIBS_DIR ./libs/)

# target_include_directories(${PROJECT_NAME} PRIVATE ${GLFW_DIR}/include)
add_subdirectory(${LIBS_DIR}/WishEngine ${CMAKE_CURRENT_BINARY_DIR}/WishEngine)

# find_package(Vulkan REQUIRED)

target_include_directories(${PROJECT_NAME}
    PRIVATE
        # ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/libs/uhcstd/io
        ${CMAKE_CURRENT_SOURCE_DIR}/libs/uhcstd/math
        ${CMAKE_CURRENT_SOURCE_DIR}/libs/uhcstd/network
        ${CMAKE_CURRENT_SOURCE_DIR}/libs/uhcstd/std
)



# target_link_libraries(imgui PUBLIC Vulkan::Vulkan glfw)

target_link_libraries(${PROJECT_NAME} PRIVATE
    WishEngine
)

target_compile_definitions(${PROJECT_NAME} PRIVATE
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "./"
)
