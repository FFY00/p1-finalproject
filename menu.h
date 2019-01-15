#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "util.h"

void main_menu(char *opt, int user_count, int loan_count, double distance, int pending_loan_count, int bike_info);
void bike_menu(char *opt, int bike_info);
void bike_insert_menu(char *opt);
void user_menu(char *opt, int user_count);
void user_insert_menu(char *opt);
void loan_menu(char *opt, int loan_count, int user_count, int bike_info);
void loan_insert_menu(char *opt);
void bike_history_menu(char *opt, int bike_history_count);
void bike_history_insert_menu(char *opt);
void stats_menu(char *opt);

#endif /* MENU_H_INCLUDED */
