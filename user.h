#ifndef USER_H_INCLUDED
#define USER_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "file.h"
#include "util.h"
#include "structs.h"

void get_user_type_str(char str[MAX_STRING], int status);
void get_user_data(t_user *user, int id);
void change_user_data(t_user *user);
t_user* insert_user(t_user *users, int *user_count);
t_user* read_user_data(t_user *user, int *user_count);
int save_user_data(t_user *user, int user_count);
int get_user(t_user *user, t_user *users, int user_count, char name[MAX_STRING]);
int get_user_from_id(t_user *user, t_user *users, int user_count, int id);
int set_user(t_user *user, t_user *users, int user_count, char name[MAX_STRING]);
int get_last_bike(char bike[MAX_STRING], t_loan *loans, int loan_count, t_bike bikes[MAX_BIKES], int user);

#endif /* USER_H_INCLUDED */
