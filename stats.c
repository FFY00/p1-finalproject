#include "stats.h"

/**
 *  @brief  Caluclates average distance per bike
 *
 *  @param  distance    Pointer to save distance
 *  @param  loans       Dynamic array cointaining the loan data
 *  @param  loans_count Loans dynamic array size
 */
void calculate_average_distance(double *distance, t_loan *loans, int loan_count)
{
    *distance = 0;

    int i;
    for(i = 0; i < loan_count; i++)
    {
        if(loans[i].status == 2) /* finallized */
        {
            *distance += loans[i].distance;
        }
    }

    *distance /= loan_count;
}

/**
 *  @brief  Calculates distribution of bike loans per user type
 *
 *  @param  stats       Array top save the stats
 *  @param  loans       Dynamic array cointaining the loan data
 *  @param  loans_count Loans dynamic array size
 *  @param  users       Dynamic array cointaining the user data
 *  @param  users_count Users dynamic array size
 */
void calculate_user_type_loan_stats(float stats[4], t_loan *loans, int loan_count, t_user *users, int user_count)
{
    int i;

    for(i = 0; i < 4; i++) /* reset stats array */
    {
        stats[i] = 0;
    }

    for(i = 0; i < loan_count; i++)
    {
        if(loans[i].status == 2 || loans[i].status == 1) /* finalized or in progress */
        {
            stats[users[loans[i].user_id].type]++; /* increment loan counter for user type */
        }
    }

    for(i = 0; i < 4; i++) /* finalize calculation */
    {
        stats[i] = stats[i] * 100.0 * loan_count;
    }

}

/**
 *  @brief  Gets the campus with higher loan count
 *
 *  @param  campus          Dynamic array to save the result. IMPORTANT: campus array needs to be freed!
 *  @param  campus_count    Campus dynamic array size
 *  @param  loans           Dynamic array cointaining the loan data
 *  @param  loans_count     Loans dynamic array size
 */
char** get_campus_with_higher_loan_count(char **campus, int *campus_count, t_loan *loans, int loan_count)
{
    campus = NULL; /* initialize campus dynamic array */
    *campus_count = 0;

    campus_counter *counter = NULL; /* dynamic array to temporary campus id */
    int counter_count = 0;

    int i, ii, exists, id;
    for(i = 0; i < loan_count; i++)
    {
        if(loans[i].status == 2 || loans[i].status == 1) /* finalized or in progress */
        {
            exists = 0; /* reset flag */

            for(ii = 0; i < counter_count; i++) /* search list for campus */
            {
                if(strcmp(loans[i].destination, counter[ii].id) == 0) /* campus in counter */
                {
                    exists = 1;
                    id = ii;
                }
            }

            if(exists == 0) /* not in counter, we need to add it */
            {
                counter = realloc(counter, sizeof(campus_counter) * ++counter_count);
                id = counter_count-1;
                counter[id].count = 0;
                strncpy(counter[id].id, loans[i].destination, MAX_STRING);
            }

            counter[id].count++; /* increment counter */
        }
    }

    int max = 0;
    for(i = 0; i < counter_count; i++) /* get the higher count */
    {
        if(counter[i].count == max) /* same, add to resultt array */
        {
            campus = realloc(campus, sizeof(char*) * ++(*campus_count)); /* realocate dynamic array */
            campus[*campus_count-1] = malloc(MAX_STRING); /* alocate heap space for string */
            strncpy(campus[*campus_count-1], counter[id].id, MAX_STRING);
        }
        if(counter[i].count > max) /* higher, reset result array */
        {
            max = counter[i].count;
            *campus_count = 1;

            campus = realloc(campus, sizeof(char*) * *campus_count); /* realocate dynamic array */
            campus[0] = malloc(MAX_STRING); /* alocate heap space for string */
            strncpy(campus[0], counter[id].id, MAX_STRING);
        }
    }

    free(counter);

    return campus;
}

/**
 *  @brief  Caluclates the number of users that have used certain bike
 *
 *  @param  count       Pointer to save the final count
 *  @param  loans       Dynamic array cointaining the loan data
 *  @param  loans_count Loans dynamic array size
 *  @param  bike        Target bike name
 */
void calculate_bike_user_count(int *user_count, t_loan *loans, int loan_count, char bike[MAX_STRING], t_bike *bikes)
{
    int *users = NULL; /* dynamic array to save the users who were already added */
    *user_count = 0;

    int i, ii, valid;
    for(i = 0; i < loan_count; i++)
    {
        if(strcmp(bikes[loans[i].bike_id].id, bike) == 0) /* target bike */
        {
            valid = 1;

            for(ii = 0; ii < *user_count; ii++) /* search for user to see if he was already added */
            {
                if(users[ii] == loans[i].user_id) /* user found, he shouldn't be added again */
                {
                    valid = 0;
                }
            }

            if(valid == 1) /* user wasn't already added */
            {
                users = realloc(users, sizeof(int) * ++(*user_count));
                users[*user_count-1] = loans[i].user_id;
            }

        }
    }

    free(users);
}

/**
 *  @brief  Calculates number of loans by specified user
 */
void calculate_user_loan_count(int *count, t_loan *loans, int loan_count, int user_id)
{
    *count = 0;

    int i;
    for(i = 0; i < loan_count; i++)
    {
        if(loans[i].user_id == user_id)
        {
            (*count)++;
        }
    }
}

/**
 *  @brief  Calculates total distance by specified user
 */
void calculate_user_distance(int *distance, t_loan *loans, int loan_count, int user_id)
{
    *distance = 0;

    int i;
    for(i = 0; i < loan_count; i++)
    {
        if(loans[i].user_id == user_id && loans[i].status == 2) /* correct user and finalized */
        {
            *distance += loans[i].distance;
        }
    }
}

/**
 *  @brief  Calculates number of loans with specified bike
 */
void calculate_bike_loan_count(int *count, t_loan *loans, int loan_count, int bike_id)
{
    *count = 0;

    int i;
    for(i = 0; i < loan_count; i++)
    {
        if(loans[i].bike_id == bike_id)
        {
            (*count)++;
        }
    }
}

/**
 *  @brief  Calculates total distance of specified bike
 */
void calculate_bike_distance(int *distance, t_loan *loans, int loan_count, int bike_id)
{
    *distance = 0;

    int i;
    for(i = 0; i < loan_count; i++)
    {
        if(loans[i].bike_id == bike_id && loans[i].status == 2) /* correct bike and finalized */
        {
            *distance += loans[i].distance;
        }
    }
}

/**
 *  @brief  Sorts a user array by number of loans
 *
 *  @return sort    Sorted array. IMPORTANT: Needs to be freed!
 */
t_user* sort_users_by_loan(t_user *users, int user_count, t_loan *loans, int loan_count)
{
    t_user *sort = malloc(sizeof(t_user) * user_count);
    int *count = malloc(sizeof(int) * user_count); /* dynamic array to save number of loans per user */

    int i, ii, temp;
    t_user utemp;

    /* copy users to sort */
    for(i = 0; i < user_count; i++)
    {
        sort[i] = users[i];
    }

    /* count loan number */
    for(i = 0; i < loan_count; i++)
    {
        count[loans[i].user_id]++;
    }

    /* sort (bubble sort) */
    for (i = 0; i < user_count-1; i++)
    {
        for (ii = 0; ii < user_count-i-1; ii++)
        {
            if (count[ii] > count[ii+1])
            {
                /* swap count array */
                temp = count[ii];
                count[ii] = count[ii+1];
                count[ii+1] = temp;
                /* swap sort array */
                utemp = sort[ii];
                sort[ii] = sort[ii+1];
                sort[ii+1] = utemp;
            }
        }
    }

    free(count);

    return sort;
}
