# CMake 3.12 or later:
file(GLOB APP_LIST "./*.cpp")

target_compile_options(advanced_cxx_lib PUBLIC /MT)

foreach(FILE_NAME ${APP_LIST})
    get_filename_component(APP_NAME ${FILE_NAME} NAME_WLE)
	add_executable(${APP_NAME} "${APP_NAME}.cpp")
	target_compile_features(${APP_NAME} PRIVATE cxx_std_17)
	target_link_libraries(${APP_NAME} PRIVATE advanced_cxx_lib)
	if(WIN32)
    	set_target_properties(${APP_NAME} PROPERTIES LINK_FLAGS "/INCREMENTAL:NO")
	endif()
	# target_link_libraries(${APP_NAME} PRIVATE advanced_cxx_lib fmt::fmt)
endforeach()
