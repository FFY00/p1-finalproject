#ifndef BIKE_H_INCLUDED
#define BIKE_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "campus.h"
#include "file.h"
#include "util.h"
#include "structs.h"

void get_status_str(char[MAX_STRING], int);
void get_bike_data(t_bike*);
void populate_bike_data(t_bike[MAX_BIKES]);
t_campus* read_bike_data(t_bike[MAX_BIKES], t_campus *campus, int *campus_count);
int save_bike_data(t_bike[MAX_BIKES], t_campus *campus, int campus_count);
int get_bike(t_bike*, t_bike[MAX_BIKES], char[MAX_STRING]);
int get_bike_id(int *bike_id, t_bike bikes[MAX_BIKES], char id[MAX_STRING]);

#endif /* BIKE_H_INCLUDED */
