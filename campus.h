#ifndef CAMPUS_H_INCLUDED
#define CAMPUS_H_INCLUDED

#include <limits.h>
#include <float.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "user.h"
#include "structs.h"

t_campus* populate_campus_data(t_bike bikes[MAX_BIKES], t_campus *campus, int *campus_count);
void calculate_campus_distance(double *distance, char id1[MAX_STRING], char id2[MAX_STRING], t_campus *campus, int campus_count);

#endif /* CAMPUS_H_INCLUDED */
