# MAIN PROJECT CMAKE FILE
#
cmake_minimum_required(VERSION 3.1...3.15)
if(${CMAKE_VERSION} VERSION_LESS 3.12)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

message(DEBUG "COMPILING MAIN...")

#
# sundile settings
project (sundile
	VERSION 0.1
	DESCRIPTION "ECS-based game engine"
	LANGUAGES C CXX
)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED on)

#
#
# define project
add_executable (sundile src/main.cpp)

add_subdirectory("./src/sundile/")
target_link_libraries(sundile
	PUBLIC sundile_lib
	)

#
# misc
# file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/src/assets DESTINATION ${CMAKE_BINARY_DIR}) ## Should be replaced with Project System, no need to duplicate entire thing every time
set(ENV{SUNDILE_CWD} ${CMAKE_BINARY_DIR})