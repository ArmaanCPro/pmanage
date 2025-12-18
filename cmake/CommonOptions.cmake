include_guard(GLOBAL)

# define a common interface for compiler options
add_library(pmanageCommon INTERFACE)

target_compile_features(pmanageCommon INTERFACE cxx_std_23)

# win32 definitions
target_compile_definitions(pmanageCommon INTERFACE
    $<$<PLATFORM_ID:Windows>:NOMINMAX;_CRT_SECURE_NO_WARNINGS;WIN32_LEAN_AND_MEAN;_WIN32_WINNT=0x0A00>
)

# Compiler options
target_compile_options(pmanageCommon INTERFACE

    # MSVC specific options
    $<$<CXX_COMPILER_ID:MSVC>:/MP;/utf-8;/W4;/wd4201;/diagnostics:caret
        /w14165;/w44242;/w44254;/w44263;/w34265;/w44296;/w44365;/w44388;/w44464
        /w14545;/w14546;/w14549;/w14555;/w34619;/w34640;/w24826;/w14905;/w14906
        /w14928;/w45038;/permissive-;/volatile:iso;/Zc:inline;/Zc:preprocessor
        /Zc:enumTypes;/Zc:lambda;/Zc:externConstexpr;/Zc:__cplusplus;/EHsc
    >

    # GCC/Clang specific options
    $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:
        -W;-Wextra;-Wall;-Werror;-Wno-unused-parameter;-pedantic;#-fno-exceptions
        -Wconversion;-Wshadow;-Wformat=2;-fvisibility=hidden
    >
)

set(CMAKE_CXX_SCAN_FOR_MODULES ON)

add_library(pmanage::common ALIAS pmanageCommon)


