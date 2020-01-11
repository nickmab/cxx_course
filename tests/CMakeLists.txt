# Download and unpack googletest at configure time
configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
                 ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
                 EXCLUDE_FROM_ALL)

# CMake 3.12 or later:
file(GLOB_RECURSE APP_LIST "./*.cpp")

target_compile_options(gtest_main PUBLIC /MT)
target_compile_options(gtest PUBLIC /MT)

foreach(FILE_NAME ${APP_LIST})
  get_filename_component(APP_NAME ${FILE_NAME} NAME_WLE)
	add_executable(${APP_NAME} "${FILE_NAME}")
	target_compile_features(${APP_NAME} PRIVATE cxx_std_17)
	# target_compile_options(${APP_NAME} PUBLIC /MT)
	target_link_libraries(${APP_NAME} PRIVATE advanced_cxx_lib gtest_main)
	if(WIN32)
    	set_target_properties(${APP_NAME} PROPERTIES LINK_FLAGS "/INCREMENTAL:NO")
	endif()
	# target_link_libraries(${APP_NAME} PRIVATE advanced_cxx_lib fmt::fmt)
endforeach()


# Now simply link against gtest or gtest_main as needed. Eg
# add_executable(example example.cpp)
# target_link_libraries(example gtest_main)
# add_test(NAME example_test COMMAND example)