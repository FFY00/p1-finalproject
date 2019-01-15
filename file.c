#include "file.h"

/**
 *  @brief  Loads a file, creating it if it doesn't exist
 *
 *  @param  ftpr    File pointer
 *  @param  path    File Path
 *
 *  @return State code:-
 *      0   Was able to read the file
 *      -1   File didn't exist
 *      -2  Can't access file
 */
int file_initialize(FILE **file, char path[])
{
    int state = 0;

    *file = fopen(path, "rb+"); /* Tries to open file in rb+ mode. A file won't be created if it doesn't exist */
    if(*file == NULL)
    {
        *file = fopen(path, "wb"); /* Create file if it doesn't exist */
        state = -1;
    }

    /* Check if the file was created */
    if(*file == NULL)
    {
        state = -2; /* File was not created. Return error. */
    }

    return state;
}

/**
 *  @brief  Loads the log file with stream positioned at the end (append mode), creating it if it doesn't exist
 *
 *  @param  ftpr    File pointer
 *  @param  path    File Path
 *
 *  @return State code:-
 *      0   Was able to read the file
 *      -1  Can't access file
 */
int log_initialize(FILE **log, char path[])
{
    int state = 0;

    *log = fopen(path, "a");

    if(*log == NULL)
    {
        state = -1;
    }

    return state;
}
