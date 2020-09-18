# SUNDILE - CMAKE
file(GLOB_RECURSE SUNDILE_FILES "*.h" "*.cpp")
include_directories("headers" "systems" "ext")
add_library(sundile ${SUNDILE_FILES})

message(DEBUG "COMPILING SUNDILE...")
message(DEBUG "CMAKE_MODULE_PATH: ${CMAKE_PREFIX_PATH}")
#
# packages & libraries
find_package(assimp REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(GLUT REQUIRED)
find_package(GLEW REQUIRED)
#find_package(EnTT CONFIG REQUIRED) # vcpkg EnTT is way out of date
set(ASSIMP_LIBS "${ASSIMP_LIBRARIES}")
if (APPLE)
	set(GLUT_LIBS "")
	set (ASSIMP_LIBS "${ASSIMP_LIBRARY_DIRS}/libassimp.a")
	set(IRRXML_LIBS "${ASSIMP_LIBRARY_DIRS}/libIrrXML.a")
	set(ZLIB_LIBS "${ASSIMP_LIBRARY_DIRS}/libz.a")
else()
	set (GLUT_LIBS "GLUT::GLUT")
endif()
find_package(imgui CONFIG REQUIRED)


target_link_libraries(sundile
	#PUBLIC EnTT
	PUBLIC glfw
	PUBLIC glm
	${GLUT_LIBS}
	PUBLIC GLEW::GLEW
	${ASSIMP_LIBS}
	${IRRXML_LIBS}
	${ZLIB_LIBS}
	PUBLIC imgui::imgui
)