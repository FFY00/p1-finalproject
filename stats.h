#ifndef STATS_H_INCLUDED
#define STATS_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "user.h"
#include "structs.h"

/** @struct campus_counter
 *  @brief  Helper struct to assign campus int values
 *
 *  @var    campus_counter::id
 *  Campus ID
 *
 *  @var    camppus_counter::count
 *  Int value to save
 */
typedef struct {
    char id[MAX_STRING];
    int count;
} campus_counter;

void calculate_average_distance(double *distance, t_loan *loans, int loan_count);
void calculate_user_type_loan_stats(float stats[4], t_loan *loans, int loan_count, t_user *users, int user_count);
char** get_campus_with_higher_loan_count(char *campus[MAX_STRING], int *campus_count, t_loan *loans, int loan_count);
void calculate_bike_user_count(int *user_count, t_loan *loans, int loan_count, char bike[MAX_STRING], t_bike *bikes);
void calculate_user_loan_count(int *count, t_loan *loans, int loan_count, int user_id);
void calculate_user_distance(int *distance, t_loan *loans, int loan_count, int user_id);
void calculate_bike_loan_count(int *count, t_loan *loans, int loan_count, int bike_id);
void calculate_bike_distance(int *distance, t_loan *loans, int loan_count, int bike_id);
t_user* sort_users_by_loan(t_user *users, int user_count, t_loan *loans, int loan_count);

#endif /* STATS_H_INCLUDED */
