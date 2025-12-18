#pragma once

#if defined(_WIN32)
    #if defined(PMANAGE_EXPORTS)
        #define PMANAGE_API __declspec(dllexport)
    #else
        #define PMANAGE_API __declspec(dllimport)
    #endif
#else
    #define PMANAGE_API __attribute__((visibility("default")))
#endif

