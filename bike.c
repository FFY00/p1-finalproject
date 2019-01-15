#include "bike.h"

/**
 *  @brief  Returns explicit string relative to bike status.
 */
void get_status_str(char str[MAX_STRING], int status)
{
    char desc[4][MAX_STRING] = {
        "Disponivel",
        "Indisponivel",
        "Danificada",
        "Desconhecido"
    };

    if(status > 2)  /* Unknown */
    {
        status = 3;
    }

    strncpy(str, desc[status], MAX_STRING);
}

/**
 *  @brief  Reads bike data
 */
void get_bike_data(t_bike *bike)
{
    printf("\nNome da bicicleta: ");
    read(bike->id);

    printf("Modelo da bicicleta: ");
    read(bike->model);

    bike->state = 0; /* We assume the bicycle is available when added */

    printf("Campus onde a bicicleta se encontra: ");
    read(bike->campus);
    normalize_string(bike->campus);
}

/**
 *  @brief  Reads all bike data
 */
void populate_bike_data(t_bike bikes[MAX_BIKES])
{
    printf("\n\tInsira as Bicicletas\n");
    int i;
    for(i = 0; i < MAX_BIKES; i++)
    {
        get_bike_data(&bikes[i]);

        /* Make sure bike doesn't exist already */
        int ii;
        for(ii = 0; ii < i; ii++)
        {
            if(strcmp(bikes[i].id, bikes[ii].id) == 0) /* Bike already exists */
            {
                printf("\nERRO: Essa bicicleta ja existe!\n");
                i--; /* Loop over the same number again */
            }
        }

    }
}

/**
 *  @brief  Loads the bike information
 *
 *  @param  bikes    Bike array
 *
 *  @return State Code:
 *      0   Successful
 *      -1  Couldn't read the file
 *      -2  Couldn't open the file
 *      -3  Couldn't write to the file
 *      -4  Could read the file but the data contained duplicated elements
 */
t_campus* read_bike_data(t_bike bikes[MAX_BIKES], t_campus *campus, int *campus_count)
{
    int status = 0;
    int write_size;

    FILE *file = NULL;

    /* Initialize the file */
    int bike_state = file_initialize(&file, "bike.dat");

    /* Error opening the file */
    if(bike_state == -2)
    {
        printf("\nERRO: Nao foi possivel abrir o ficheiro de dados\n");
        status = -2;
    }

    /* File exists! Try to read the data */
    if(bike_state == 0)
    {

        if(fread(bikes, sizeof(t_bike), MAX_BIKES, file) < MAX_BIKES) /* Read data (populate bikes array) */
        {
            printf("\nERRO: Nao foi possivel ler as bicicletas do ficheiro corretamente\n");
            status = -1;
        } else {

            /* Check for duplicated elements */
            int i, ii;
            for(i = 0; i < MAX_BIKES; i++)
            {
                for(ii = i+1; ii < MAX_BIKES; ii++)
                {
                    if(strcmp(bikes[i].id, bikes[ii].id) == 0) /* Duplicated element! */
                    {
                        printf("ERRRO: Existem elementos duplicados\n");
                        i = MAX_BIKES; /* exit loop */
                        status = -4;
                    }
                }
            }

            /* read campus_count and alocate memory */
            if(status == 0 && fread(campus_count, sizeof(int), 1, file) != 1)
            {
                status = -6;
                *campus_count = 0;
            } else { /* read was successful */
                if(*campus_count < 0) /* make sure the campus count is sane */
                {
                    printf("\nERRO: Quantidade de campus invalida\n");
                    status = -6;
                } else {
                    campus = realloc(campus, sizeof(t_campus) * *campus_count);
                }
            }

            /* read campus */
            int a = fread(campus, sizeof(t_user), *campus_count, file);
            if(status == 0 && a != *campus_count)
            {
                status = -6;
                printf("\nERRO: Falha ao ler os campus do ficheiro\n");
            }

        }
    }

    /* File doesn't exist or failed to read the data */
    if(bike_state != 0 || status == -1 || status == -4)
    {
        /* Get bike information (populate bike array) */
        populate_bike_data(bikes);

        /* Try to write the date to the file if file didn't exist or we previously failed to read the data */
        if(bike_state == 1 || status == 1)
        {
            write_size = fwrite(bikes, sizeof(t_bike), MAX_BIKES, file);

            if(write_size != MAX_BIKES)
            {
                printf("\nERRO: Falha ao escrever para o ficheiro de dados");
                status = -3;
            }
        }
    }

    if(status == -6)
    {
        campus = populate_campus_data(bikes, campus, campus_count);
    }

    fclose(file);

    /* return status; */
    return campus;
}

/**
 *  @brief  Saves the bike information
 *
 *  @param  bikes    Bike array
 *
 *  @return Satus Code:
 *      0   Successful
 *      -2  Couldn't open the file
 *      -3  Couldn't write to the file
 */
int save_bike_data(t_bike bikes[MAX_BIKES], t_campus *campus, int campus_count)
{
    int status = 0;

    FILE *file = NULL;

    /* Initialize the file */
    int bike_state = file_initialize(&file, "bike.dat");

    /* Error opening the file */
    if(bike_state == -1)
    {
        printf("\nERRO: Nao foi possivel abrir o ficheiro de dados\n");
        status = -2;
    } else {
        /* Try to write the date to the file if file didn't exist or if we previously failed to read the data */
        if(status == 0 && (
            fwrite(bikes, sizeof(t_bike), MAX_BIKES, file) != MAX_BIKES || /* save bikes*/
            fwrite(&campus_count, sizeof(int), 1, file) != 1 || /* save campus_count */
            fwrite(campus, sizeof(t_campus), campus_count, file) != campus_count)) /* save campus */
        {
            printf("\nERRO: Falha ao escrever para o ficheiro de dados");
            status = -3;
        }

        fclose(file);
    }

    return status;
}

/**
 *  @brief  Get bike from bike array
 *
 *  @param  bike    Bike pointer to save target bike
 *  @param  bikes   Bike array to search
 *  @param  id      Target bike ID
 *
 *  @return Error code:
 *      0   Successful
 *      -1  Not in array
 */
int get_bike(t_bike *bike, t_bike bikes[MAX_BIKES], char id[MAX_STRING])
{
    int ret = -1;

    int i;
    for(i = 0; i < MAX_BIKES; i++)
    {
        if(strcmp(bikes[i].id, id) == 0) /* Found bike with the specified id*/
        {
            *bike = bikes[i];
            ret = 0; /* return 0 - Success */

            i = MAX_BIKES; /* break loop */
        }
    }

    return ret;
}

/**
 *  @brief  Get bike ID
 *
 *  @param  bike    Bike pointer to save target bike
 *  @param  bikes   Bike array to search
 *  @param  id      Target bike ID
 *
 *  @return Error code:
 *      0   Successful
 *      -1  Not in array
 */
int get_bike_id(int *bike_id, t_bike bikes[MAX_BIKES], char id[MAX_STRING])
{
    int ret = -1;

    int i;
    for(i = 0; i < MAX_BIKES; i++)
    {
        if(strcmp(bikes[i].id, id) == 0) /* Found bike with the specified id*/
        {
            *bike_id = i;
            i = MAX_BIKES; /* break loop */
        }
    }

    return ret;
}
