include_guard(GLOBAL)

option(ENABLE_ASAN "Enable AddressSanitizer" OFF)
option(ENABLE_UBSAN "Enable UndefinedBehaviorSanitizer" OFF)
option(ENABLE_TSAN "Enable ThreadSanitizer" OFF)

function(enable_sanitizers target)
    if (NOT TARGET ${target})
        message(FATAL_ERROR "enable_sanitizers: ${target} is not a target")
    endif()

    if (MSVC)
        # MSVC ASAN is only supported on newer versions and x64
        if (ENABLE_ASAN)
            target_compile_options(${target} PRIVATE /fsanitize=address)
            target_link_options(${target} PRIVATE /fsanitize=address)
        endif()
        return()
    endif()

    set(sanitize_flags "")
    if (ENABLE_ASAN)
        list(APPEND sanitize_flags address)
    endif()
    if (ENABLE_UBSAN)
        list(APPEND sanitize_flags undefined)
    endif()
    if (ENABLE_TSAN)
        list(APPEND sanitize_flags thread)
    endif()

    if (sanitize_flags)
        list(JOIN sanitize_flags "," sanitize_list)

        target_compile_options(${target} PRIVATE
            -fsanitize=${sanitize_list}
            -fno-omit-frame-pointer
            -g
        )
        target_link_options(${target} PRIVATE
            -fsanitize=${sanitize_list}
        )
    endif()
endfunction()

