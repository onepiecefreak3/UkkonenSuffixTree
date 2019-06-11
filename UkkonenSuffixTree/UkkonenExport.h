#pragma once

#if defined(__Win)
    #define DllExport __declspec( dllexport )
#elif defined(__Unix)
    #define DllExport __attribute__((visibility("default")))
#else
    #define DllExport
#endif

#define MAX_CHAR 256
