#include "util.h"

#include <stdio.h>

/**
 *  @brief  Clears the stdin buffer
 */
void clear_buffer()
{
    char temp;

    do{
        temp = getchar();
    } while(temp != '\n' && temp != EOF);
}

/**
 *  @brief  Removes newline from string. String will end in the first newline.
 */
void remove_newline(char str[MAX_STRING])
{
    int i;
    for(i = 0; i < MAX_STRING; i++)
    {
        if(str[i] == '\n')
        {
            str[i] = '\0';
            /* return; */ /* Not allowed :/ */
            i = MAX_STRING; /* Alternative to break the loop */
        }
    }
}

/**
 *  @brief  Determines if the buffer is dirty and cleans it
 *
 *  @param  Last string read from the buffer
 *  @param  Max read size
 */
void clear_dirty_buffer(char str[], int max)
{
    int i, dirty_buffer = 1;
    for(i = 0; i < max; i++)
    {
        if(str[i] == '\n') /* checks if \n is in the buffer, if it is then the buffer is clean */
        {
            dirty_buffer = 0; /* \n exists so buffer is clean */
            i = max; /* exit loop */
        }
    }

    if(dirty_buffer == 1) /* only clear buffer when it is dirty */
    {
        clear_buffer();
    }
}

/**
 *  @brief  Reads string from stdin
 */
void read(char str[MAX_STRING])
{
    fgets(str, MAX_STRING, stdin);
    clear_dirty_buffer(str, MAX_STRING);
    remove_newline(str);
}

/**
 *  @brief  Reads int within a certain boundary
 */
void read_int(int *target, int min, int max, char msg[MAX_STRING])
{
    int valid;

    do {
        printf("%s", msg);
        scanf(" %d", target);
        clear_buffer(); /* buffer is always dirty, contains \n */

        if(*target < min || *target > max)
        {
            printf("\nO numero devera estar entre %d e %d\n\n", min, max);
            valid = 0;
        } else {
            valid = 1;
        }
    } while(valid == 0);
}

/**
 *  @brief  Reads int within a certain boundary
 */
void read_double(double *target, double min, double max, char msg[MAX_STRING])
{
    int valid;

    do {
        printf("%s", msg);
        scanf(" %lf", target);
        clear_buffer(); /* buffer is always dirty, contains \n */

        if(*target < min || *target > max)
        {
            printf("\nO numero devera estar entre %g e %g\n\n", min, max);
            valid = 0;
        } else {
            valid = 1;
        }
    } while(valid == 0);
}

/**
 *  @brief  Reads phone from stdin
 *
 *  str     Destination string
 *  msg     String to show before the data input
 *  skip    Should we allow the user to skip by hitting enter?
 */
void read_phone(char str[MAX_PHONE], char msg[MAX_STRING], int skip)
{
    int valid = 0;

    while(valid == 0)
    {
        printf("%s", msg);
        fgets(str, MAX_PHONE + 1, stdin);
        clear_dirty_buffer(str, MAX_PHONE);
        remove_newline(str);

        if(skip == 1 && str[0] == '\0') /* skip the tests if skip is active and the first character is \n */
        {
            valid = 1;
        } else {
            if(strlen(str) != MAX_PHONE){ /* check lenght */
                printf("\nERRO: Numero de telefone invalido (tamanho errado)\n\n");
            } else {
                valid = 1;

                int i;
                for(i = 0; i < MAX_PHONE; i++)
                {
                    if(str[i] < 48 || str[i] > 57) /* check if the characters are numbers (between 48 and 57 in the ascii table) */
                    {
                        printf("\nERRO: Numero de telefone invalido (contem caracteres que nao sao numeros)\n\n");
                        valid = 0;
                        i = MAX_PHONE; /* exit loop */
                    }
                }
            }
        }

    }
}

/**
 *  @brief  Makes first character of string upper case and everything else lower case
 */
void normalize_string(char str[MAX_STRING])
{
    str[0] = toupper(str[0]);

    int i;
    for(i = 1; i <  MAX_STRING; i++)
    {
        str[i] = tolower(str[i]);
    }
}

/**
 *  @brief  Calculate day in given month
 */
void calculate_month_days(int *max_days, int month, int year)
{
    *max_days = 31;

    if(month == 2) /* february */
    {
        if(year % 400 == 0 || (year % 100 != 0 && year % 4 == 0)) /* leap year*/
        {
            *max_days = 29;
        } else {
            *max_days = 28;
        }
    }

    if(month == 4 || month == 6 || month == 9 || month == 11) /* 30 days */
    {
        *max_days = 30;
    }
}

/**
 *  @brief  Compares two t_time structs
 *
 *  @return Status code
 *      -1  Lower
 *      0   Equal
 *      1   Higher
 */
int compare_time(t_time time1, t_time time2)
{
    int status = 0;

    /* year */
    if(time1.year == time2.year) /* same year */
    {

        /* month */
        if(time1.month == time2.month) /* same month */
        {

            /* day */
            if(time1.day == time2.day) /* same day */
            {

                /* hour */
                if(time1.hour == time2.hour) /* same hour */
                {

                    /* min */
                    if(time1.min == time2.min) /* same min */
                    {

                        /* secs */
                        if(time1.secs == time2.secs) /* same secs */
                        {
                            status = 0;
                        } else {
                            if(time1.secs > time2.secs)   /* higher secs */
                            {
                                status = 1;
                            } else {                    /* lower secs */
                                status = -1;
                            }
                        }

                    } else {
                        if(time1.min > time2.min)   /* higher min */
                        {
                            status = 1;
                        } else {                    /* lower min */
                            status = -1;
                        }
                    }

                } else {
                    if(time1.hour > time2.hour)   /* higher hour */
                    {
                        status = 1;
                    } else {                    /* lower hour */
                        status = -1;
                    }
                }

            } else {
                if(time1.day > time2.day)   /* higher day */
                {
                    status = 1;
                } else {                    /* lower day */
                    status = -1;
                }
            }

        } else {
            if(time1.day > time2.day)   /* higher month */
            {
                status = 1;
            } else {                    /* lower month */
                status = -1;
            }
        }

    } else {
        if(time1.year > time2.year) /* higher year */
        {
            status = 1;
        } else {                    /* lower year */
            status = -1;
        }
    }

    return status;
}
