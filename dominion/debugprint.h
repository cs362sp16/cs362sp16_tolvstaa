#ifndef _DEBUGPRINT
#define _DEBUGPRINT

#include <stdio.h>

#ifdef DEBUG
    #define debug_printf(format, ...) fprintf(stderr, format, ##__VA_ARGS__)
#else
    #define debug_printf(format, ...)
#endif

#endif
