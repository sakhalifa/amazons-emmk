#ifndef __UTIL_H__
#define __UTIL_H__

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <limits.h>
#include <assert.h>
typedef unsigned int uint;

enum error_type{
    MALLOC_ERR = 1,
    FILE_ERR,
};

void _check_error(void * pointer, enum error_type error_type, uint line , const char * file);

/*
Checks the given pointer to see if the allocation didn't failed. If the pointer is NULL the program will stop with an error.
*/
#define CHECK_MALLOC(ptr) _check_error((ptr), MALLOC_ERR, (__LINE__ - (1)) , __FILE__)

#endif