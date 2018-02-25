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

if(WIN32)
    set(BINARY_RESOURCE_FILE "resource.rc")
endif()

add_executable("input"
	"input.cpp"
	
	"../source/SH3/system/assert.cpp"
	"../source/SH3/system/config.cpp"
	"../source/SH3/system/glcontext.cpp"
	"../source/SH3/system/glprogram.cpp"
	"../source/SH3/system/input.cpp"
	"../source/SH3/system/log.cpp"
	"../source/SH3/system/window.cpp"

    "${BINARY_RESOURCE_FILE}"
)

target_link_libraries("input"
	PRIVATE "${GLEW_LIBRARIES}"
	PRIVATE "${OPENGL_LIBRARIES}"
	PRIVATE "${SDL2_LIBRARIES}"
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

    "${BINARY_RESOURCE_FILE}"
)

target_link_libraries("vao"
	PRIVATE "${GLEW_LIBRARIES}"
	PRIVATE "${OPENGL_LIBRARIES}"
	PRIVATE "${SDL2_LIBRARIES}"
)

add_executable("tex"
	"tex.cpp"
	
	"../source/SH3/arc/mft.cpp"
	"../source/SH3/arc/subarc.cpp"
	"../source/SH3/arc/vfile.cpp"
	
	"../source/SH3/graphics/texture.cpp"
	
	"../source/SH3/system/assert.cpp"
	"../source/SH3/system/config.cpp"
	"../source/SH3/system/glcontext.cpp"
	"../source/SH3/system/glprogram.cpp"
	"../source/SH3/system/glbuffer.cpp"
	"../source/SH3/system/glvertarray.cpp"
	"../source/SH3/system/log.cpp"
	"../source/SH3/system/window.cpp"

    "${BINARY_RESOURCE_FILE}"
)

target_link_libraries("tex"
	PRIVATE "${GLEW_LIBRARIES}"
	PRIVATE "${OPENGL_LIBRARIES}"
	PRIVATE "${SDL2_LIBRARIES}"
	PRIVATE "${ZLIB_LIBRARIES}"
)

add_executable("cam"
	"camera.cpp"
	
	"../source/SH3/angle.cpp"
	"../source/SH3/camera/camera.cpp"
	
	"../source/SH3/system/assert.cpp"
	"../source/SH3/system/config.cpp"
	"../source/SH3/system/glcontext.cpp"
	"../source/SH3/system/glprogram.cpp"
	"../source/SH3/system/glbuffer.cpp"
	"../source/SH3/system/glvertarray.cpp"
	"../source/SH3/system/log.cpp"
	"../source/SH3/system/window.cpp"

    "${BINARY_RESOURCE_FILE}"
)

target_link_libraries("cam"
	PRIVATE "${GLEW_LIBRARIES}"
	PRIVATE "${OPENGL_LIBRARIES}"
	PRIVATE "${SDL2_LIBRARIES}"
	PRIVATE "${ZLIB_LIBRARIES}"
)

