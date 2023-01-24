include(FetchContent)

cmake_minimum_required(VERSION 3.24)

project(sol)

set(CMAKE_CXX_STANDARD 20)

FetchContent_Declare(lua
    URL "http://www.lua.org/ftp/lua-5.4.4.tar.gz"
    URL_HASH SHA256=164c7849653b80ae67bec4b7473b884bf5cc8d2dca05653475ec2ed27b9ebf61)

FetchContent_MakeAvailable(lua)

set(lua_source_files
    lapi.c lauxlib.c lbaselib.c lcode.c lcorolib.c lctype.c ldblib.c ldebug.c ldo.c ldump.c lfunc.c lgc.c linit.c
    liolib.c llex.c lmathlib.c lmem.c loadlib.c lobject.c lopcodes.c loslib.c lparser.c lstate.c lstring.c lstrlib.c
    ltable.c ltablib.c ltm.c lua.c lundump.c lutf8lib.c lvm.c lzio.c)

list(TRANSFORM lua_source_files PREPEND "${lua_SOURCE_DIR}/src/")

set_source_files_properties(${lua_source_files} PROPERTIES LANGUAGE CXX)

add_library(lua STATIC ${lua_source_files})

target_include_directories(lua PUBLIC "${lua_SOURCE_DIR}/src")

add_library(lua::lua ALIAS lua)

add_executable(sol)

target_include_directories(sol PRIVATE include)

target_sources(sol PRIVATE
    include/sol/concepts/function.h
    include/sol/concepts/function_pointer.h
    include/sol/concepts/has_unambiguous_const_call_operator.h
    include/sol/concepts/pointer.h
    include/sol/concepts/tuple_like.h
    include/sol/concepts/unwrappable.h
    include/sol/force_inline.h
    include/sol/fxhash.h
    include/sol/guard.h
    include/sol/lbind.h
    include/sol/lbind/convert_argument.h
    include/sol/lbind/convert_result.h
    include/sol/lbind/exports.h
    include/sol/lbind/register.h
    include/sol/lbind/semantic_wrapper.h
    include/sol/lbind/wrapper_function.h
    include/sol/meta/constant.h
    include/sol/meta/constant_pack.h
    include/sol/meta/type.h
    include/sol/meta/type_pack.h
    include/sol/panic.h

    src/main.cpp
)

target_link_libraries(sol PRIVATE
    lua::lua
)
