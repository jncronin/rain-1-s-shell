cmake_minimum_required(VERSION 3.11 FATAL_ERROR)

project(s C)

add_executable(s)

target_sources(s
    PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/s.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/builtins.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/parser.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/tokenizer.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/variables.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/interpreter.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/region.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/stringport.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/util.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/linenoise/linenoise.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/linenoise/encodings/utf8.c"
)

target_include_directories(s
    PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/linenoise"
)

add_executable(glob)

target_sources(glob
    PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/supporting/glob.c"
)

add_executable(redir-box)

target_sources(redir-box
    PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/supporting/redir-box.c"
)

install(TARGETS s glob redir-box)
