#ifndef LOG_H
#define LOG_H

#define DEBUGDEBUG 1
#include <stdio.h>
#if DEBUGDEBUG
#define LOGLOG(format, ...) \
{ \
fprintf(stderr ,format ,##__VA_ARGS__); \
    fprintf(stderr ,"\n"); \
    }
#else
#define LOGLOG(format, ...)
#endif
#endif // LOG_H
