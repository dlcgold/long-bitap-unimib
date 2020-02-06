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
#include "cmdline.h"

char* substr(const char*, int, int);
uint64_t* bitap(char*, char*);
void bitapLong(char*, char*);
char* read_file(char*);
char* load_file(char*);

#endif

