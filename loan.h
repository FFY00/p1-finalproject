#ifndef LOAN_H_INCLUDED
#define LOAN_H_INCLUDED

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "file.h"
#include "user.h"
#include "util.h"
#include "structs.h"

void get_loan_status_str(char str[MAX_STRING], int status);
void get_loan_request_data(t_loan *loan, int id, t_bike bikes[MAX_BIKES], int user_count, t_campus *campus, int campus_count);
void get_loan_request_end_data(t_loan *loan, int id);
int get_loan(t_loan *loan, t_loan *loans, int loan_count, int id);
t_loan* insert_loan(t_loan *loans, int *loan_count, t_bike bikes[MAX_BIKES], int user_count, t_campus *campus, int campus_count);
void resolve_loan_requests(t_loan *loans, int loan_count, int loan_criteria, t_bike bikes[MAX_BIKES], t_user *users);
void calculate_pending_loans(int *pending_loan_count, t_loan *loans, int loan_count);
void set_loan_criteria(int *opt);
t_loan* read_loan_data(t_loan *loans, int *loan_count);
int save_loan_data(t_loan *loans, int loan_count);

#endif /* LOAN_H_INCLUDED */
