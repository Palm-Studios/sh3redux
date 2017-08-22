find_package(Boost REQUIRED)
#find_package(Freetype REQUIRED)
#find_package(FTGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm REQUIRED)
find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)
find_package(ZLIB REQUIRED)

include_directories("../include")
include_directories(SYSTEM "../third_party/debugbreak")
include_directories(SYSTEM "${Boost_INCLUDE_DIRS}")
include_directories(SYSTEM "${GLEW_INCLUDE_DIRS}")
include_directories(SYSTEM "${GLM_INCLUDE_DIRS}")
include_directories(SYSTEM "${OPENGL_INCLUDE_DIR}")
include_directories(SYSTEM "${SDL2_INCLUDE_DIRS}")
include_directories(SYSTEM "${ZLIB_INCLUDE_DIRS}")

add_executable("${CMAKE_PROJECT_NAME}"
	"main.cpp"
	
	"SH3/arc/file.cpp"
	"SH3/arc/section.cpp"
	"SH3/arc/types.cpp"
	"SH3/arc/vfile.cpp"
	
	"SH3/graphics/texture.cpp"
	"SH3/graphics/msbmp.cpp"
	"SH3/graphics/quad.cpp"
	
	"SH3/system/assert.cpp"
	"SH3/system/config.cpp"
	"SH3/system/glcontext.cpp"
	"SH3/system/glprogram.cpp"
	"SH3/system/glbuffer.cpp"
	"SH3/system/glvertarray.cpp"
	"SH3/system/input.cpp"
	"SH3/system/log.cpp"
	"SH3/system/window.cpp"

	
	"../${BINARY_RESOURCE_FILE}"
)

target_link_libraries("${CMAKE_PROJECT_NAME}"
#	PRIVATE FREETYPE
#	PRIVATE FTGL
	PRIVATE "${GLEW_LIBRARIES}"
#	PRIVATE glm
	PRIVATE "${OPENGL_LIBRARIES}"
	PRIVATE "${SDL2_LIBRARIES}"
	PRIVATE "${ZLIB_LIBRARIES}"
)
