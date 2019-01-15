#ifndef BIKE_HISTORY_H_INCLUDED
#define BIKE_HISTORY_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "file.h"
#include "util.h"
#include "stats.h"
#include "structs.h"

void get_bike_history_data(t_bike_history *history, int id, t_bike bikes[MAX_BIKES]);
int write_new_report_log(t_bike_history history, t_bike bikes[MAX_BIKES], t_loan *loans, int loan_count);
int write_finalize_report_log(char bike_id[MAX_STRING]);
t_bike_history* insert_bike_history_report(t_bike_history *bike_history, int *bike_history_count, t_loan *loans, int loan_count, t_bike bikes[MAX_BIKES]);
t_bike_history* read_bike_history_data(t_bike_history *bike_history, int *bike_history_count);
int save_bike_history_data(t_bike_history *bike_history, int bike_history_count);

#endif /* BIKE_HISTORY_H_INCLUDED */
