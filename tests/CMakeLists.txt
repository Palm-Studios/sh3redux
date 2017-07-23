#find_package(Freetype REQUIRED)
#find_package(FTGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm REQUIRED)
find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)
find_package(ZLIB REQUIRED)

include_directories("../include")
include_directories(SYSTEM "../third_party/debugbreak")

add_executable("input"
	"input.cpp"
	
	"../source/SH3/system/assert.cpp"
	"../source/SH3/system/config.cpp"
	"../source/SH3/system/glcontext.cpp"
	"../source/SH3/system/glprogram.cpp"
	"../source/SH3/system/input.cpp"
	"../source/SH3/system/log.cpp"
	"../source/SH3/system/window.cpp"
)

target_link_libraries("input"
#	PRIVATE "${FREETYPE_LIBRARIES}"
#	PRIVATE "${FTGL_LIBRARIES}"
	PRIVATE "${GLEW_LIBRARIES}"
	PRIVATE "${GLM_LIBRARIES}"
	PRIVATE "${OPENGL_gl_LIBRARY}"
	PRIVATE "${SDL2_LIBRARIES}"
	PRIVATE "${ZLIB_LIBRARIES}"
)

add_executable("vao"
	"vao.cpp"
	
	"../source/SH3/system/assert.cpp"
	"../source/SH3/system/config.cpp"
	"../source/SH3/system/glcontext.cpp"
	"../source/SH3/system/glprogram.cpp"
	"../source/SH3/system/glbuffer.cpp"
	"../source/SH3/system/glvertarray.cpp"
	"../source/SH3/system/log.cpp"
	"../source/SH3/system/window.cpp"
)