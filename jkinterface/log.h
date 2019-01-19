#ifndef LOG_H
#define LOG_H

#define DEBUG_TO_STDERR 0
#include <stdio.h>
#if DEBUG_TO_STDERR
#define LOGLOG(format, ...) \
{ \
fprintf(stderr ,format ,##__VA_ARGS__); \
    fprintf(stderr ,"\n"); \
    }
#else
int jklog(const char* ,...);
#define LOGLOG(format, ...) \
{ \
    jklog(format ,##__VA_ARGS__); \
    jklog("\n"); \
    }
#endif
void log_init();
#endif // LOG_H
