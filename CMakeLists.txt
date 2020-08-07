# MAIN PROJECT CMAKE FILE
#
## TODO: Looks like assimp isn't loading symbols for irrXMLReader? might need to link it manually.
## This is the same error, but only has *one* of the bugs.
## Look into how assimp actually links. Look for dynamic libraries or anything you might need to manually include.

cmake_minimum_required(VERSION 3.1...3.15)
if(${CMAKE_VERSION} VERSION_LESS 3.12)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

#
# sundile settings
project (sproj
	VERSION 0.1
	DESCRIPTION "Sundile project"
	LANGUAGES C CXX
)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED on)
#set(CMAKE_CXX_FLAGS -v)
set(CMAKE_C_FLAGS -stdlib=libstdc++)
#build_command(-v)

set(CMAKE_C_COMPILER clang)
set(CMAKE_C_COMPILER_TARGET ${triple})
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_COMPILER_TARGET ${triple})

#
# dependencies
set(SRCDIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
add_subdirectory(${SRCDIR}/components)
add_subdirectory(${SRCDIR}/systems)
add_subdirectory(${SRCDIR}/sundile)

#
# define project
add_executable (sproj src/main.cpp)

#
# packages
find_package(assimp REQUIRED)
#find_package(EnTT CONFIG REQUIRED)
find_package(glew REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
#find_package(GLUT REQUIRED)
find_package(imgui CONFIG REQUIRED)


#
# directories

set(ASSIMP_DIR /Users/ada/dev/lib/assimp/)
target_include_directories(
	sproj
	#PUBLIC ${SRCDIR}/assets
	PUBLIC ${SRCDIR}/components
	PUBLIC ${SRCDIR}/systems
	PUBLIC ${SRCDIR}/sundile
	#PRIVATE ${ASSIP_DIR}/
	#${ENTT_DIR}
	#${GLFW3_DIR}
	#${GLM_DIR}
	#${GLEW_DIR}
	#${IMGUI_DIR}
)

#
# libraries

set(VCPKG_INCLUDE_DIR /Users/ada/dev/lib/vcpkg/installed/x64-osx/lib )

target_link_libraries(sproj
	PRIVATE ${ASSIMP_DIR}/bin/libassimp.dylib
	#PRIVATE ${ASSIMP_DIR}/lib/libIrrXML.a
	PRIVATE assimp::assimp #${ASSIMP_LIBRARIES}
	#PRIVATE EnTT::EnTT
	PRIVATE GLEW::GLEW
	PRIVATE glfw
	PRIVATE glm
	#PRIVATE GLUT::GLUT
	PRIVATE imgui::imgui

	PUBLIC Sundile
	INTERFACE SundileComponents
	INTERFACE SundileSystems
)


#
# misc
file(COPY ${SRCDIR}/assets DESTINATION ${CMAKE_BINARY_DIR})