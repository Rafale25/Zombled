cmake_minimum_required(VERSION 3.25.0)

set(PROJECT_NAME "Zombled")
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_SCAN_FOR_MODULES OFF)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

set(LIBS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../libs/)

project(${PROJECT_NAME})

file(GLOB_RECURSE SRC_C CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c)
file(GLOB_RECURSE SRC_CPP CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
    ${LIBS_DIR}/uhcstd/*.cc
)
file(GLOB_RECURSE SRC_HEADERS CONFIGURE_DEPENDS ./src/*.hpp)

add_compile_options(-fsanitize=address -fno-omit-frame-pointer -g)
add_link_options(-fsanitize=address)

add_executable(${PROJECT_NAME} ${SRC_CPP} ${SRC_C} ${SRC_HEADERS})


# target_include_directories(${PROJECT_NAME} PRIVATE ${GLFW_DIR}/include)
add_subdirectory(${LIBS_DIR}/WishEngine ${CMAKE_CURRENT_BINARY_DIR}/WishEngine)

# find_package(Vulkan REQUIRED)

target_include_directories(${PROJECT_NAME}
    PRIVATE
        # ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${LIBS_DIR}/uhcstd/io
        ${LIBS_DIR}/uhcstd/math
        ${LIBS_DIR}/uhcstd/network
        ${LIBS_DIR}/uhcstd/std
)



# target_link_libraries(imgui PUBLIC Vulkan::Vulkan glfw)

target_link_libraries(${PROJECT_NAME} PRIVATE
    WishEngine
)

target_compile_definitions(${PROJECT_NAME} PRIVATE
    ASSETS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/../assets/"
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "./"
)
