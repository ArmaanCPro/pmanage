include_guard(GLOBAL)
include(CheckIPOSupported)

option(ENABLE_IPO "Enable interprocedural optimization (LTO)" OFF)

function(enable_ipo target)
    if (NOT ENABLE_IPO)
        return()
    endif()

    if (NOT TARGET ${target})
        message(FATAL_ERROR "enable_ipo: ${target} is not a target")
    endif()

    check_ipo_supported(RESULT ipo_supported OUTPUT ipo_error)

    if (ipo_supported)
        set_property(TARGET ${target}
            PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE
        )
    else()
        message(WARNING "IPO not supported: ${ipo_error}")
    endif()
endfunction()

