# SUNDILE - CMAKE
file(GLOB_RECURSE SUNDILE_FILES "*.h" "*.cpp")
add_library(Sundile ${SUNDILE_FILES})

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
# libraries 
target_link_libraries(
	Sundile
	PRIVATE assimp::assimp #${ASSIMP_LIBRARIES}

	#PRIVATE EnTT
	PRIVATE GLEW::GLEW
	PRIVATE glfw
	PRIVATE glm
	#PRIVATE GLUT::GLUT
	PRIVATE imgui::imgui

	#PUBLIC Sundile
	PUBLIC SundileComponents
	#PUBLIC SundileAssets
	PUBLIC SundileSystems
)