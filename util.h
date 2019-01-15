#ifndef UTIL_H_INCLUDED
#define UTIL_H_INCLUDED

#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include "structs.h"

void clear_buffer();
void remove_newline(char[MAX_STRING]);
void clear_dirty_buffer(char[], int);
void read(char[MAX_STRING]);
void read_int(int *target, int min, int max, char msg[MAX_STRING]);
void read_double(double *target, double min, double max, char msg[MAX_STRING]);
void read_phone(char[MAX_PHONE], char[MAX_STRING], int skip);
void normalize_string(char str[MAX_STRING]);
void calculate_month_days(int *max_days, int month, int year);
int compare_time(t_time time1, t_time time2);

#endif /* UTIL_H_INCLUDED */
