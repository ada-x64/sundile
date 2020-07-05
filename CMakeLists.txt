# MAIN PROJECT CMAKE FILE
#
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
find_package(EnTT CONFIG REQUIRED)
find_package(glew REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(GLUT REQUIRED)
find_package(imgui CONFIG REQUIRED)


#
# directories
target_include_directories(
	sproj
	#PUBLIC ${SRCDIR}/assets
	PUBLIC ${SRCDIR}/components
	PUBLIC ${SRCDIR}/systems
	PUBLIC ${SRCDIR}/sundile
	${ENTT_DIR}
	${GLFW3_DIR}
	${GLM_DIR}
	${GLEW_DIR}
	${IMGUI_DIR}
)

#
# libraries 
target_link_libraries(sproj
	PRIVATE ${ASSIMP_LIBRARIES}

	PRIVATE EnTT
	PRIVATE GLEW::GLEW
	PRIVATE glfw
	PRIVATE glm
	PRIVATE GLUT::GLUT
	PRIVATE imgui::imgui

	PUBLIC Sundile
	INTERFACE SundileComponents
	INTERFACE SundileSystems
)

#
# misc
file(COPY ${SRCDIR}/assets DESTINATION ${CMAKE_BINARY_DIR})