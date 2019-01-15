#include "bike_history.h"

/**
 *  @brief  Prompt user for bike history info
 *
 *  @param  history     Pointer to save bike history report
 */
void get_bike_history_data(t_bike_history *history, int id, t_bike bikes[MAX_BIKES])
{
    char temp[MAX_STRING];
    int i, max_days;

    history->id = id;

    int valid = 0;
    do {
        printf("\nNome da bicicleta: ");
        read(temp);
        normalize_string(temp);

        for(i = 0; i < MAX_BIKES; i++)
        {
            if(strcmp(bikes[i].id, temp) == 0) /* bikes exists */
            {
                valid = 1;
                i = MAX_BIKES; /* exit loop */
            }
        }

        if(valid == 0)
        {
            printf("\nERRO: Nao existe nenhuma bicicleta com esse nome\n");
        }
    } while(valid == 0);

    for(i = 0; i < MAX_BIKES; i++)
    {
        if(strcmp(temp, bikes[i].id) == 0)
        {
            history->bike_id = i;
            i = MAX_BIKES; /* exit loop */
        }
    }

    bikes[history->bike_id].state = 2; /* broken */

    printf("\n\tData de avaria\n");
    read_int(&history->time.year, 0, INT_MAX, "Ano: ");
    read_int(&history->time.month, 1, 12, "Mes: ");

    /* calculate max days */
    calculate_month_days(&max_days, history->time.month, history->time.year);

    read_int(&history->time.day, 1, max_days, "Dia: ");

    read_int(&history->time.hour, 0, 23, "Hora: ");
    read_int(&history->time.min, 0, 59, "Minuto: ");
    read_int(&history->time.secs, 0, 59, "Segundo: ");

    printf("\nDetalhes da avaria: ");
    read(history->description);
}

/**
 *  @brief  Write new report to log
 *
 *  @param  history     Bike history to write to the log
 *  @param  bikes       Bikes array
 */
int write_new_report_log(t_bike_history history, t_bike bikes[MAX_BIKES], t_loan *loans, int loan_count)
{
    FILE *file;
    double distance;

    int status = log_initialize(&file, "log.txt");

    /* Error opening the file */
    if(status == -1)
    {
        printf("\nERRO: Nao foi possivel abrir o ficheiro de log\n");
    } else {
        calculate_bike_distance(&distance, loans, loan_count, history.bike_id);

        fprintf(file, "(!) Avaria #%d\n", history.id);
        fprintf(file, "\tBicicleta: %s\n", bikes[history.bike_id].id);
        fprintf(file, "\tDistancia total precorrida: %g\n", distance);
        printf("Data: %02d/%02d/%d %02d:%02d:%02d\n", history.time.day, history.time.month,
            history.time.year, history.time.hour, history.time.min, history.time.secs);
        fprintf(file, "\tDescricao: %s\n", history.description);

        fclose(file);
    }

    return status;
}

/**
 *  @brief  Write final report information to log
 *
 *  @param  history     Bike history to write to the log
 *  @param  bikes       Bikes array
 */
int write_finalize_report_log(char bike_id[MAX_STRING])
{
    FILE *file;

    int status = log_initialize(&file, "log.txt");

    /* Error opening the file */
    if(status == -1)
    {
        printf("\nERRO: Nao foi possivel abrir o ficheiro de log\n");
    } else {
        fprintf(file, "(+) A bicicleta '%s' ja se encontra disponivel\n", bike_id);

        fclose(file);
    }

    return status;
}

/**
 *  @brief  Inserts a report in the bike history array
 *
 *  @param  bike_history        Bike history dynamic array
 *  @param  bike_history_count  Number of bike history reports
 *  @param  loans               Loans Dynamic array
 *  @param  loan_count          Loan count
 */
t_bike_history* insert_bike_history_report(t_bike_history *bike_history, int *bike_history_count, t_loan *loans, int loan_count, t_bike bikes[MAX_BIKES])
{
    bike_history = realloc(bike_history, sizeof(t_bike_history) * ++(*bike_history_count)); /* increase array size by one */
    get_bike_history_data(&bike_history[*bike_history_count-1], *bike_history_count-1, bikes); /* populate the bike history data */

    bikes[bike_history[*bike_history_count-1].bike_id].state = 2; /* broken */

    write_new_report_log(bike_history[*bike_history_count-1], bikes, loans, loan_count);

    return bike_history;
}

/**
 *  @brief  Loads the bike history information
 *
 *  @param  bike_history        Bike history dynamic array
 *  @param  bike_history_count  Bike history report count
 *
 *  @return State Code:
 *      0   Successful
 *      -1  Couldn't read the file
 *      -2  Couldn't open the file
 *      -3  Couldn't write to the file
 *      -4  Could read the file but the data contained duplicated elements
 *      -5  Could read the file but the file contained an invalid array size number
 */
t_bike_history* read_bike_history_data(t_bike_history *bike_history, int *bike_history_count)
{
    int real_bike_history_count = 0, status = 0;

    FILE *file = NULL;

    /* Initialize the file */
    int bike_history_state = file_initialize(&file, "bike_history.dat");

    /* Error opening the file */
    if(bike_history_state == -2)
    {
        printf("\nERRO: Nao foi possivel abrir o ficheiro de dados\n");
        status = -2;
    }

    /* File exists! Try to read the data */
    if(bike_history_state == 0)
    {
        /* Read bike history count */
        if(status == 0 && fread(bike_history_count, sizeof(int), 1, file) != 1)
        {
            status = -1;
        } else { /* read was successful */
            if(*bike_history_count < 0) /* make sure the bike history count is sane */
            {
                printf("\nERRO: Quantidade de avarias invalida\n");
                status = -5;
            } else {
                bike_history = realloc(bike_history, sizeof(t_bike_history) * *bike_history_count);
            }
        }

        /* Read data (populate bike history array) */
        int a = fread(bike_history, sizeof(t_user), *bike_history_count, file);
        if(status == 0 && a != *bike_history_count)
        {
            status = -1;
        }

        /* Failed to read the data */
        if(status == -1)
        {
            printf("\nERRO: Nao foi possivel ler o ficheiro corretamente\n");
        }

        if(status == 0)
        {

            /* Check for duplicated elements */
            int i, ii;
            for(i = 0; i < *bike_history_count; i++)
            {
                for(ii = i+1; ii < *bike_history_count; ii++)
                {
                    if(bike_history[i].id == bike_history[ii].id) /* Duplicated element! */
                    {
                        printf("ERRRO: Existem elementos duplicados\n");
                        i = *bike_history_count; /* exit loop */
                        status = -4;
                    }
                }
            }

        }
    }

    /* print how many reports were read */
    if(status == 0 || status == -2)
    {
        if(*bike_history_count > 0)
        {
            real_bike_history_count = *bike_history_count;
        }

        printf("%d avaria(s) carregada(s).\n", real_bike_history_count);
    }

    fclose(file);

    return bike_history;
}

/**
 *  @brief  Saves the bike history information
 *
 *  @param  bike_history        Bike history dynamic array
 *  @param  bike_history_count  Bike history report count
 *
 *  @return Satus Code:
 *      0   Successful
 *      -2  Couldn't open the file
 *      -3  Couldn't write to the file
 */
int save_bike_history_data(t_bike_history *bike_history, int bike_history_count)
{
    int status = 0;

    FILE *file = NULL;

    /* Initialize the file */
    int bike_history_state = file_initialize(&file, "bike_history.dat");

    /* Error opening the file */
    if(bike_history_state == -1)
    {
        printf("\nERRO: Nao foi possivel abrir o ficheiro de dados\n");
        status = -2;
    } else {
        /* Try to write the date to the file if file didn't exist or if we previously failed to read the data */

        /* save user count */
        if(status == 0 && fwrite(&bike_history_count, sizeof(int), 1, file) != 1)
        {
            status = -3;
        }

        /* save user data */
        if(status == 0 && fwrite(bike_history, sizeof(t_bike_history), bike_history_count, file) != bike_history_count)
        {
            status = -3;
        }

        if(status == -3) /* failed to write to file */
        {
            printf("\nERRO: Falha ao escrever para o ficheiro de dados");
            status = -3;
        }

        fclose(file);
    }

    return status;
}
