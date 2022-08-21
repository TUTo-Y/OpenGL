/*
*   处理不同编译器，开发环境和系统的头文件
*/
#ifndef CONTROL_H_
#define CONTROL_H_

#include <malloc.h>

#ifdef  _WIN32  // windows

//  include
#   include <io.h>
#   include <windows.h>
#   include <stdlib.h.h>
#   include <direct.h>

#else // linux

//  include
#   include <unistd.h>
#   include <stdbool.h>
#   include <stdarg.h>
#   include <string.h>
#   include <sys/stat.h>
#   include <sys/types.h>
// bool
#   define BOOL bool
#   define TRUE true
#   define FALSE false
// mem
#   define _msize malloc_usable_size
// file
#   define MAX_PATH 260
#   define mkdir(x) mkdir(x, S_IRWXU)

#endif

#ifndef _DEBUG
#define _DEBUG
#endif

#endif  // CONTROL_H_