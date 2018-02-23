find_package(Boost REQUIRED COMPONENTS system filesystem)
find_package(SDL2 REQUIRED)
find_package(ZLIB REQUIRED)

include_directories("../include")
include_directories(SYSTEM "../third_party/debugbreak")
include_directories(SYSTEM "${Boost_INCLUDE_DIRS}")
include_directories(SYSTEM "${SDL2_INCLUDE_DIRS}")
include_directories(SYSTEM "${ZLIB_INCLUDE_DIRS}")

add_executable("fileinfo"
	"fileinfo/info.cpp"
	
	"../source/SH3/arc/mft.cpp"
	"../source/SH3/arc/subarc.cpp"
	"../source/SH3/arc/vfile.cpp"
	
	"../source/SH3/system/assert.cpp"
	"../source/SH3/system/config.cpp"
	"../source/SH3/system/log.cpp"
)

target_link_libraries("fileinfo"
    PRIVATE "${SDL2_LIBRARIES}"
	PRIVATE "${ZLIB_LIBRARIES}"
)

add_executable("arc_dump"
	"dump/dump.cpp"
	
	"../source/SH3/arc/mft.cpp"
	"../source/SH3/arc/subarc.cpp"
	"../source/SH3/arc/vfile.cpp"
	
	"../source/SH3/system/assert.cpp"
	"../source/SH3/system/config.cpp"
	"../source/SH3/system/log.cpp"
)

target_link_libraries("arc_dump"
    PRIVATE "${SDL2_LIBRARIES}"
	PRIVATE "${ZLIB_LIBRARIES}"
)

add_executable("arc_extract"
	"extract/extract.cpp"
)

target_link_libraries("arc_extract"
	PRIVATE "${Boost_LIBRARIES}"
	PRIVATE "${ZLIB_LIBRARIES}"
)