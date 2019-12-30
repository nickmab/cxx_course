cmake_minimum_required(VERSION 3.16)

# project name and useful settings. Other commands can see results of this.
project(AdvancedCXX
	VERSION 0.1
	DESCRIPTION "Udemy course and example cmake project."
	LANGUAGES CXX)

# only do this if this is the main project (not included through add_subdirectory)
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)

    # Optionally set things like CMAKE_CXX_STANDARD, CMAKE_POSITION_INDEPENDENT_CODE here
    
    # Let's ensure -std=c++xx instead of -std=g++xx
    set(CMAKE_CXX_EXTENSIONS OFF)

    # Let's nicely support folders in IDE's
    set_property(GLOBAL PROPERTY USE_FOLDERS ON)

    # Testing only available if this is the main app
    # Note this needs to be done in the main CMakeLists
    # since it calls enable_testing, which must be in the
    # main CMakeLists.
    include(CTest)

    # Docs only available if this is the main app
    find_package(Doxygen)
    if(Doxygen_FOUND)
	add_subdirectory(docs)
    else()
	message(STATUS "Doxygen not found, not building docs")
    endif()
endif()

# The compiled library code is here
add_subdirectory(src)

# The executable code is here
add_subdirectory(apps)

option(PACKAGE_TESTS "Build the tests" ON)
# Testing only available if this is the main app / current project.
if(PACKAGE_TESTS AND (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME))
    enable_testing()
    include(GoogleTest)
    add_subdirectory(tests)
endif()

