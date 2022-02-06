cmake_minimum_required(VERSION 3.2)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VESRION 0.0.${PATCH_VERSION})

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

project(bulk_server VERSION ${PROJECT_VESRION})

configure_file(include/version.h.in include/version.h)

add_executable(bulk_server src/main.cpp)
add_library(async SHARED src/async.cpp )	

set_target_properties(async PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
)

find_package(Threads REQUIRED)
find_package(Boost COMPONENTS system)

target_include_directories(bulk_server
    PRIVATE ${Boost_INCLUDE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_include_directories(async
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(bulk_server PRIVATE ${Boost_LIBRARIES} async)
target_link_libraries(async Threads::Threads)

if(MSVC)
	target_compile_options(bulk_server PRIVATE /MT)
endif()

install(TARGETS async bulk_server RUNTIME DESTINATION bin)
set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT artem-glibkin@yandex.ru)

include(CPack)
