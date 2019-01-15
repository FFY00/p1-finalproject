#include "loan.h"

/**
 *  @brief  Returns explicit string relative to the loan status
 */
void get_loan_status_str(char str[MAX_STRING], int status)
{
    char desc[][MAX_STRING] = {
        "Em espera",
        "Em progresso",
        "Finalizado",
        "Cancelado",
        "Deconhecido"
    };

    if(status > 3)  /* Unknown */
    {
        status = 4;
    }

    strncpy(str, desc[status], MAX_STRING);
}

/**
 *  @brief  Prompts user for loan request data
 *
 *  @param  loan    Loan pointer to save the data
 *  @param  id      Loan ID
 */
void get_loan_request_data(t_loan *loan, int id, t_bike bikes[MAX_BIKES], int user_count, t_campus *campus, int campus_count)
{
    char temp[MAX_STRING];
    int max_days, valid, i;

    loan->id = id;
    loan->status = 0;

    printf("\n");
    read_int(&loan->user_id, 0, user_count - 1, "ID do Utente: ");

    valid = 0;
    do {
        printf("Campus de Origem: ");
        read(loan->origin);
        normalize_string(loan->origin);

        for(i = 0; i < campus_count; i++) /* iterate over campus array */
        {
            if(strcmp(campus[i].id, loan->origin) == 0) /* origin campus is in array */
            {
                valid = 1;
                i = campus_count; /* exit loop */
            }
        }

        if(valid == 0)
        {
            printf("\nERRO: Campus indesponivel. Os disponiveis sao:\n");
            for(i = 0; i < campus_count; i++)
            {
                printf("\t- %s\n", campus[i].id);
            }
            printf("\n");
        }
    } while(valid == 0);

    valid = 0;
    do {
        printf("Campus de Destino: ");
        read(loan->destination);
        normalize_string(loan->destination);

        for(i = 0; i < campus_count; i++) /* iterate over campus array */
        {
            if(strcmp(campus[i].id, loan->destination) == 0) /* destination campus is in array */
            {
                valid = 1;
                i = campus_count; /* exit loop */
            }
        }

        if(valid == 0)
        {
            printf("\nERRO: Campus indesponivel. Os disponiveis sao:\n");
            for(i = 0; i < campus_count; i++)
            {
                printf("\t- %s\n", campus[i].id);
            }
            printf("\n");
        }
    } while(valid == 0);

    printf("Bicicleta de Preferencia: (Predefinicao=Nenhuma) ");
    read(temp);
    normalize_string(temp);
    if(temp[0] == '\0' || strcmp(temp, "Nenhuma") == 0 || strcmp(temp, "Nenhum") == 0) /* default */
    {
        loan->bike_id = -1;
    } else {
        int i;
        for(i = 0; i < MAX_BIKES; i++)
        {
            if(strcmp(temp, bikes[i].id) == 0)
            {
                loan->bike_id = i;
            }
        }
    }

    printf("\n\tData de pedido\n");
    read_int(&loan->pickup_time.year, 0, INT_MAX, "Ano: ");
    read_int(&loan->pickup_time.month, 1, 12, "Mes: ");

    /* calculate max days */
    calculate_month_days(&max_days, loan->pickup_time.month, loan->pickup_time.year);

    read_int(&loan->pickup_time.day, 1, max_days, "Dia: ");

    read_int(&loan->pickup_time.hour, 0, 23, "Hora: ");
    read_int(&loan->pickup_time.min, 0, 59, "Minuto: ");
    read_int(&loan->pickup_time.secs, 0, 59, "Segundo: ");
}


/**
 *  @brief  Prompts user for loan request data
 *
 *  @param  loan    Loan pointer to save the data
 *  @param  id      Loan ID
 */
void get_loan_request_end_data(t_loan *loan, int id)
{
    int max_days;
    char msg[MAX_STRING];

    loan->status = 2; /* finalized */

    int valid = 0;
    do {
        printf("\n\tData de entrega\n");
        read_int(&loan->return_time.year, 0, INT_MAX, "Ano: ");
        read_int(&loan->return_time.month, 1, 12, "Mes: ");

        /* calculate max days */
        calculate_month_days(&max_days, loan->return_time.month, loan->return_time.year);

        read_int(&loan->return_time.day, 1, max_days, "Dia: ");

        read_int(&loan->return_time.hour, 0, 23, "Hora: ");
        read_int(&loan->return_time.min, 0, 59, "Minuto: ");
        read_int(&loan->return_time.secs, 0, 59, "Segundo: ");

        strncpy(msg, "\nDistancia percorrida (Predefinicao=", MAX_STRING);
        /*strncat(msg, , MAX_STRING - strlen(msg));*/
        strncat(msg, "): ", MAX_STRING - strlen(msg));

        if(compare_time(loan->return_time, loan->pickup_time) < 0)
        {
            printf("\nERRO: A data de devolucao nao pode ser menor que a data inicial\n");
        } else {
            valid = 1;
        }
    } while(valid == 0);
}

/**
 *  @brief  Inserts a loan in loans array
 *
 *  @param  loans       Loans dynamic array
 *  @param  loan_count  Number of elements in loans dynamic array
 */
t_loan* insert_loan(t_loan *loans, int *loan_count, t_bike bikes[MAX_BIKES], int user_count, t_campus *campus, int campus_count) /* double pointer would make more sense */
{
    loans = realloc(loans, sizeof(t_loan) * (*loan_count + 1)); /* increase array size by one */
    get_loan_request_data(&loans[*loan_count], *loan_count, bikes, user_count, campus, campus_count); /* populate the loan data */

    /* Make sure the user hasn't already placed a loan request */
    int i;
    for(i = 0; i < *loan_count; i++)
    {
        if(loans[*loan_count].user_id == loans[i].user_id && loans[i].status != 2) /* user already placed a loan request */
        {
            printf("\nERRO: O utente especificado ja tem um emprestimo em curso\n");
            /* undo */
            loans = realloc(loans, *loan_count);
            (*loan_count)--;
            i = *loan_count; /* exit loop */
        }
    }

    (*loan_count)++; /* update the user counter */

    return loans;
}

void resolve_loan_requests(t_loan *loans, int loan_count, int loan_criteria, t_bike bikes[MAX_BIKES], t_user *users)
{
    int i, ii;

    int assign[MAX_BIKES]; /* array that hods loan array identifier to assign to each bike */

    /* initalize to -1, -1 means no change */
    for(i = 0; i < MAX_BIKES; i++)
    {
        assign[i] = -1;
    }

    /* iterate over loans array and assign the loans to be resolved */
    for(i = 0; i < loan_count; i++)
    {
        if(loans[i].status == 0) /* pending */
        {
            /* iterate over bikes array */
            for(ii = 0; ii < MAX_BIKES; ii++)
            {
                if(
                    (
                        loans[i].bike_id == -1 &&                       /* user doesn't have bike preference */
                        strcmp(bikes[ii].campus, loans[i].origin) == 0  /* right campus */
                    ) || (
                        loans[i].bike_id != -1 &&                       /* use has bike preference */
                        loans[i].bike_id == ii &&                       /* right bike */
                        bikes[ii].state == 0 &&                         /* bike is avaliable */
                        strcmp(bikes[ii].campus, loans[i].origin) == 0  /* right campus */
                    )
                  ) {
                    if(assign[ii] == -1) /* unassigned */
                    {
                        assign[ii] = i;
                        ii = MAX_BIKES; /* exit bike loop */
                    } else { /* bike was already assign */
                        switch(loan_criteria)
                        {
                            case 0: /* by date */
                                if(compare_time(loans[assign[ii]].pickup_time, loans[i].pickup_time) == 1) /* assigned loan has a higher date, this loan wins */
                                {
                                    assign[ii] = i;
                                    ii = MAX_BIKES; /* exit bike loop */
                                }
                                break;

                            case 1: /* by user type */
                                if(users[loans[i].user_id].type > users[loans[assign[ii]].user_id].type) /* this user had a higher type */
                                {
                                    assign[ii] = i;
                                    ii = MAX_BIKES; /* exit bike loop */
                                }
                                break;

                            case 2: /* by distance */
                                break;
                        }
                    }
                }
            }
        }
    }

    /* make the changes */
    for(i = 0; i < MAX_BIKES; i++)
    {
        if(assign[i] != -1)
        {
            printf("A bicicleta '%s' foi emprestada ao utente '%s'\n", bikes[i].id, users[loans[assign[i]].user_id].name);
            loans[assign[i]].status = 1; /* set in progress */
            loans[assign[i]].bike_id = i; /* set bike */
            bikes[i].state = 1; /* set as unavailable */
        }
    }
}

/**
 *  @brief  Calculates number of pending loan requests
 */
void calculate_pending_loans(int *pending_loan_count, t_loan *loans, int loan_count)
{
    *pending_loan_count = 0;

    int i;
    for(i = 0; i < loan_count; i++)
    {
        if(loans[i].status == 0)
        {
            (*pending_loan_count)++;
        }
    }
}

/**
 *  @brief  Shows the loan criteria menu and gets the desired action
 *
 *  @param  Option pointer
 */
void set_loan_criteria(int *opt)
{
    int valid = 0;
    do {
        printf("\n\tCriterio de selecao para a lista de espera\n");
        printf("\t(0) Data\n");
        printf("\t(1) Tipo de utilizador\n");
        printf("\t(2) Distancia\n");
        printf("Selecione a opcao: ");

        scanf(" %d", opt);
        clear_buffer();

        if (*opt >= 0 || *opt <= 2)
        {
            valid = 1;
        }

    } while(valid != 1);
}

/**
 *  @brief  Loads the loan information
 *
 *  @param  loanss    Loan array
 *
 *  @return State Code:
 *      0   Successful
 *      -1  Couldn't read the file
 *      -2  Couldn't open the file
 *      -3  Couldn't write to the file
 *      -4  Could read the file but the data contained duplicated elements
 *      -5  Could read the file but the file contained an invalid array size number
 */
t_loan* read_loan_data(t_loan *loans, int *loan_count)
{
    int real_loan_count = 0, status = 0;

    FILE *file = NULL;

    /* Initialize the file */
    int loan_state = file_initialize(&file, "loan.dat");

    /* Error opening the file */
    if(loan_state == -2)
    {
        printf("\nERRO: Nao foi possivel abrir o ficheiro de dados\n");
        status = -2;
    }

    /* File exists! Try to read the data */
    if(loan_state == 0)
    {
        /* Read user count */
        if(status == 0 && fread(loan_count, sizeof(int), 1, file) != 1)
        {
            status = -1;
            *loan_count = 0;
        } else { /* read was successful */
            if(*loan_count < 0) /* make sure the user count is sane */
            {
                printf("\nERRO: Quantidade de emprestimos invalida\n");
                status = -5;
            } else {
                loans = realloc(loans, sizeof(t_loan) * *loan_count); /* alocate memory for */
            }
        }

        /* Read data (populate users array) */
        int a = fread(loans, sizeof(t_loan), *loan_count, file);
        if(status == 0 && a != *loan_count)
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
            for(i = 0; i < *loan_count; i++)
            {
                for(ii = i+1; ii < *loan_count; ii++)
                {
                    if(loans[i].id == loans[ii].id) /* Duplicated element! */
                    {
                        printf("ERRRO: Existem elementos duplicados\n");
                        i = *loan_count; /* exit loop */
                        status = -4;
                    }
                }
            }

        }
    }

    /* print how many users were read */
    if(status == 0 || status == -2)
    {
        if(*loan_count > 0)
        {
            real_loan_count = *loan_count;
        }

        printf("%d emprestimo(s) carregado(s).\n", real_loan_count);
    }

    fclose(file);

    return loans;
}

/**
 *  @brief  Saves the loan information
 *
 *  @param  loans   Loans array
 *
 *  @return Satus Code:
 *      0   Successful
 *      -2  Couldn't open the file
 *      -3  Couldn't write to the file
 */
int save_loan_data(t_loan *loans, int loan_count)
{
    int status = 0;

    FILE *file = NULL;

    /* Initialize the file */
    int loan_state = file_initialize(&file, "loan.dat");

    /* Error opening the file */
    if(loan_state == -1)
    {
        printf("\nERRO: Nao foi possivel abrir o ficheiro de dados\n");
        status = -2;
    } else {
        /* Try to write the date to the file if file didn't exist or if we previously failed to read the data */

        /* save user count */
        if(status == 0 && fwrite(&loan_count, sizeof(int), 1, file) != 1)
        {
            status = -3;
        }

        /* save user data */
        if(status == 0 && fwrite(loans, sizeof(t_loan), loan_count, file) != loan_count)
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
