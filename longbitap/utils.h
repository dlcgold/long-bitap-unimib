#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <malloc.h>
#include <math.h>
#include <assert.h>
#include <stdlib.h>
#include <inttypes.h>
#include <stdbool.h>
#include "cmdline.h"

char* substr(const char*, int, int);
uint64_t* bitap(char*, char*, uint64_t, uint64_t);
uint64_t* countfirst(uint64_t*, uint64_t);
void bitapLong(char*, char*);
char* load_file(char*);
void print(uint64_t*, uint64_t);
#endif
