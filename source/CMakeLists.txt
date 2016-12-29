#find_package(Freetype REQUIRED)
#find_package(FTGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm REQUIRED)
find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)
find_package(ZLIB REQUIRED)

include_directories("../include")

add_executable("${CMAKE_PROJECT_NAME}"
	"sh3main.cpp"
	
	"arc/sh3_arc.cpp"
	"arc/sh3_arc_section.cpp"
	
	"system/sh3_config.cpp"
	"system/sh3_glcontext.cpp"
	"system/sh3_log.cpp"
	"system/sh3_window.cpp"
	
	"../${BINARY_RESOURCE_FILE}"
)
target_link_libraries("${CMAKE_PROJECT_NAME}"
#	PRIVATE "${FREETYPE_LIBRARIES}"
#	PRIVATE "${FTGL_LIBRARIES}"
	PRIVATE "${GLEW_LIBRARIES}"
	PRIVATE "${GLM_LIBRARIES}"
	PRIVATE "${OPENGL_gl_LIBRARY}"
	PRIVATE "${SDL2_LIBRARIES}"
	PRIVATE "${ZLIB_LIBRARIES}"
)
