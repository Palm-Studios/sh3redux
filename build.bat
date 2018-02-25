@echo off

set /p lib_path="Enter library path: "

cmake CMakeLists.txt -G "MinGW Makefiles" -DLIB_ROOT_DIR="%lib_path%" -DGLEW_INCLUDE_DIR="%lib_path%" -DSDL2_INCLUDE_DIR="%lib_path%"/SDL2/include/SDL2 -DGLM_INCLUDE_DIR="%lib_path%"/glm -DZLIB_INCLUDE_DIR="%lib_path%"/zlib/include -DOPENGL_INCLUDE_DIR="%lib_path%"/GL