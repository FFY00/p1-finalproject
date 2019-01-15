#ifndef FILE_H_INCLUDED
#define FILE_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>

#include "util.h"

int file_initialize(FILE**, char[]);
int log_initialize(FILE **log, char path[]);

#endif /* FILE_H_INCLUDED */
