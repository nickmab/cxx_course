# Note that headers are optional, and do not affect add_library, but they will not
# show up in IDEs unless they are listed in add_library.

add_subdirectory(graphers)

# CMake 3.12 or later:
file(GLOB_RECURSE IMPL_LIST "./*.cpp")
file(GLOB_RECURSE HEADER_LIST CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/include/advanced_cxx/*.h")

# Make an automatic library - will be static or dynamic based on user setting
add_library(advanced_cxx_lib ${IMPL_LIST} ${HEADER_LIST})

find_package(Protobuf REQUIRED)

message("${Protobuf_LIBRARIES}")
message("${Protobuf_INCLUDE_DIRS}")

if(NOT Protobuf_FOUND)
	message(FATAL_ERROR "Protobuf is required. Could not find it. Fail.")
endif()

# We need this directory, and users of our library will need it too
target_include_directories(advanced_cxx_lib PUBLIC ../include "${Protobuf_INCLUDE_DIRS}")
# target_include_directories(advanced_cxx_lib PUBLIC ../include)

target_link_libraries(advanced_cxx_lib PUBLIC "${Protobuf_LIBRARIES}")

# All users of this library will need at least C++17
target_compile_features(advanced_cxx_lib PUBLIC cxx_std_17)
# target_compile_options(advanced_cxx_lib PUBLIC /MT)

# IDEs should put the headers in a nice place
source_group(TREE "${PROJECT_SOURCE_DIR}/include" PREFIX "Header Files" FILES ${HEADER_LIST})
