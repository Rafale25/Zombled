file(GLOB_RECURSE UHCSTD_SRC CONFIGURE_DEPENDS *.cc)

add_library(uhcstd STATIC ${UHCSTD_SRC})

target_include_directories(uhcstd PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/io
    ${CMAKE_CURRENT_SOURCE_DIR}/math
    ${CMAKE_CURRENT_SOURCE_DIR}/network
    ${CMAKE_CURRENT_SOURCE_DIR}/std
)
