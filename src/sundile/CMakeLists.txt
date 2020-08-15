# SUNDILE - CMAKE
file(GLOB_RECURSE SUNDILE_FILES "*.h" "*.cpp")
add_library(sundile ${SUNDILE_FILES})

message("COMPILING SUNDILE...")
#
# packages & libraries
find_package(assimp REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(GLUT REQUIRED)
find_package(imgui CONFIG REQUIRED)

target_link_libraries(sundile
	PUBLIC ${ASSIMP_LIBRARIES}
	PUBLIC GLEW::GLEW
	PUBLIC glfw
	PUBLIC glm
	PUBLIC GLUT::GLUT
	PUBLIC imgui::imgui
)