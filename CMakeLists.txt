# MAIN PROJECT CMAKE FILE
#

cmake_minimum_required(VERSION 3.1...3.15)
if(${CMAKE_VERSION} VERSION_LESS 3.12)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

message("COMPILING MAIN...")

#
# sundile settings
project (sproj
	VERSION 0.1
	DESCRIPTION "Sundile project"
	LANGUAGES C CXX
)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED on)

#
# define project
add_executable (sproj src/main.cpp)

add_subdirectory("./src/sundile/")
target_link_libraries(sproj
	PUBLIC sundile
	)

#
# misc
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/src/assets DESTINATION ${CMAKE_BINARY_DIR})