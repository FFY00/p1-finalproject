#include "user.h"

/**
 *  @brief  Returns explicit string relative to user.type
 */
void get_user_type_str(char str[MAX_STRING], int status)
{
    char desc[][MAX_STRING] = {
        "Aluno",
        "Docente",
        "Tecnico Administrativo",
        "Convidado",
        "Deconhecido"
    };

    if(status == -1) /* guest */
    {
        status = 3;
    }

    if(status > 3)  /* unknown */
    {
        status = 4;
    }

    strncpy(str, desc[status], MAX_STRING);
}

/**
 *  @brief  Read user type
 */
void read_user_type(int *type, char str[MAX_STRING])
{
    int valid = 0;
    char type_str[MAX_STRING];

    do {
        printf(str);
        read(type_str);

        /* make string all lower case*/
        int i;
        for(i = 0; i < MAX_STRING; i++)
        {
            if(type[i] != '\0')
            {
                type[i] = tolower(type[i]);
            } else {
                i = MAX_STRING; /* exit loop */
            }
        }

        if(strcmp(type_str, "aluno") == 0){
            *type = 0;
            valid = 1;
        }

        if(strcmp(type_str, "docente") == 0){
            *type = 1;
            valid = 1;
        }

        if(strcmp(type_str, "tecnico administrativo") == 0){
            *type = 2;
            valid = 1;
        }

        if(strcmp(type_str, "convidado") == 0){
            *type = -1;
            valid = 1;
        }

        /* no type (to use in change_user_data, means keep the old type)*/
        if(type_str[0] == '\0'){
            *type = -2;
            valid = 1;
        }

        if(valid == 0)
        {
            printf("\nERRO: Tipo de utente invalido. Os tipos desponiveis sao Aluno, Docente, Tecnico Administrativo e Convidado.");
        }

    } while(valid == 0);
}

/**
 *  @brief  Reads user data
 */
void get_user_data(t_user *user, int id)
{
    user->id = id;

    printf("\nNome do utente: ");
    read(user->name);

    read_phone(user->phone, "Numero de telefone: ", 0);

    read_user_type(&user->type, "Tipo de utente: ");
}

/**
 *  @brief  Reads user data
 */
void change_user_data(t_user *user)
{
    char temp[MAX_STRING], phone[MAX_PHONE], type_str[MAX_STRING];
    int type;

    printf("\nNome do utente: (Predefinido=%s) ", user->name);
    read(temp);
    if(temp[0] != '\0') /* string is not empty, update the value */
    {
        strncpy(user->name, temp, MAX_STRING);
    }

    strncpy(temp, "Numero de telefone: (Predifinido=", MAX_STRING);
    strncat(temp, user->phone, MAX_STRING - strlen(temp));
    strncat(temp, ") ", MAX_STRING - strlen(temp));
    read_phone(phone, temp, 1);
    if(phone[0] != '\0') /* string is not empty, update the value */
    {
        strncpy(user->phone, phone, MAX_STRING);
    }

    get_user_type_str(type_str, user->type);
    strncpy(temp, "Tipo de utente: (Predifinido=", MAX_STRING);
    strncat(temp, type_str, MAX_STRING - strlen(temp));
    strncat(temp, ") ", MAX_STRING - strlen(temp));
    read_user_type(&type, temp);
    if(type != -2) /* valid type, update the value */
    {
        user->type = type;
    }
}

/**
 *  @brief  Inserts new user
 */
t_user* insert_user(t_user *users, int *user_count) /* double pointer would make more sense */
{
    users = realloc(users, sizeof(t_user) * (*user_count + 1)); /* increase array size by one */
    get_user_data(&users[*user_count], *user_count); /* populate the user data */

    /* Make sure the user inserted is not duplicated */
    int i;
    for(i = 0; i < *user_count; i++)
    {
        if(strcmp(users[*user_count].name, users[i].name) == 0) /* duplicated user */
        {
            printf("\nERRO: Ja existe um utente com esse nome\n");
            /* undo */
            users = realloc(users, *user_count);
            (*user_count)--;
            i = *user_count; /* exit loop */
        }
    }

    (*user_count)++; /* update the user counter */

    return users;
}

/**
 *  @brief  Loads the user information
 *
 *  @param  users    User array
 *
 *  @return State Code:
 *      0   Successful
 *      -1  Couldn't read the file
 *      -2  Couldn't open the file
 *      -3  Couldn't write to the file
 *      -4  Could read the file but the data contained duplicated elements
 *      -5  Could read the file but the file contained an invalid array size number
 */
t_user* read_user_data(t_user *users, int *user_count)
{
    int real_user_count = 0, status = 0;

    FILE *file = NULL;

    /* Initialize the file */
    int user_state = file_initialize(&file, "user.dat");

    /* Error opening the file */
    if(user_state == -2)
    {
        printf("\nERRO: Nao foi possivel abrir o ficheiro de dados\n");
        status = -2;
    }

    /* File exists! Try to read the data */
    if(user_state == 0)
    {
        /* Read user count */
        if(status == 0 && fread(user_count, sizeof(int), 1, file) != 1)
        {
            status = -1;
            *user_count = 0;
        } else { /* read was successful */
            if(*user_count < 0) /* make sure the user count is sane */
            {
                printf("\nERRO: Quantidade de utilizadores invalida\n");
                status = -5;
            } else {
                users = realloc(users, sizeof(t_user) * *user_count);
            }
        }

        /* Read data (populate users array) */
        int a = fread(users, sizeof(t_user), *user_count, file);
        if(status == 0 && a != *user_count)
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
            for(i = 0; i < *user_count; i++)
            {
                for(ii = i+1; ii < *user_count; ii++)
                {
                    if(users[i].id == users[ii].id) /* Duplicated element! */
                    {
                        printf("ERRRO: Existem elementos duplicados\n");
                        i = *user_count; /* exit loop */
                        status = -4;
                    }
                }
            }

        }
    }

    /* print how many users were read */
    if(status == 0 || status == -2)
    {
        if(*user_count > 0)
        {
            real_user_count = *user_count;
        }

        printf("%d utilizadore(s) carregado(s).\n", real_user_count);
    }

    fclose(file);

    /* return status; */ /* we are gonna use return for the array instead of a double pointer :/ */
    return users;
}

/**
 *  @brief  Saves the user information
 *
 *  @param  user    User array
 *
 *  @return Satus Code:
 *      0   Successful
 *      -2  Couldn't open the file
 *      -3  Couldn't write to the file
 */
int save_user_data(t_user *users, int user_count)
{
    int status = 0;

    FILE *file = NULL;

    /* Initialize the file */
    int user_state = file_initialize(&file, "user.dat");

    /* Error opening the file */
    if(user_state == -1)
    {
        printf("\nERRO: Nao foi possivel abrir o ficheiro de dados\n");
        status = -2;
    } else {
        /* Try to write the date to the file if file didn't exist or if we previously failed to read the data */

        if(status == 0 && (
            fwrite(&user_count, sizeof(int), 1, file) != 1 || /* save user count */
            fwrite(users, sizeof(t_user), user_count, file) != user_count)) /* save user data */
        {
            printf("\nERRO: Falha ao escrever para o ficheiro de dados");
            status = -3;
        }

        fclose(file);
    }

    return status;
}

/**
 *  @brief  Get user with specified name from user array
 *
 *  @param  user    Bike pointer to save target bike
 *  @param  users   Bike array to search
 *  @param  name    Target user name
 *
 *  @return Error code:
 *      0   Successful
 *      -1  Not in array
 */
int get_user(t_user *user, t_user *users, int user_count, char name[MAX_STRING])
{
    int ret = -1;

    int i;
    for(i = 0; i < user_count; i++)
    {
        if(strcmp(users[i].name, name) == 0) /* Found user with the specified name*/
        {
            *user = users[i];
            ret = 0; /* return 0 - Success */

            i = user_count; /* break loop */
        }
    }

    return ret;
}

/**
 *  @brief  Get user with specified ID from user array
 *
 *  @param  user    Bike pointer to save target bike
 *  @param  users   Bike array to search
 *  @param  name    Target user name
 *
 *  @return Error code:
 *      0   Successful
 *      -1  Not in array
 */
int get_user_from_id(t_user *user, t_user *users, int user_count, int id)
{
    int ret = -1;

    int i;
    for(i = 0; i < user_count; i++)
    {
        if(users[i].id == id) /* Found user with the specified name*/
        {
            *user = users[i];
            ret = 0; /* return 0 - Success */

            i = user_count; /* break loop */
        }
    }

    return ret;
}

/**
 *  @brief  Replaces an user in a array
 *
 *  @param  user    Bike pointer to save target bike
 *  @param  users   Bike array to search
 *  @param  name    Target user name
 *
 *  @return Error code:
 *      0   Successful
 *      -1  Not in array
 */
int set_user(t_user *user, t_user *users, int user_count, char name[MAX_STRING])
{
    int ret = -1;

    int i;
    for(i = 0; i < user_count; i++)
    {
        if(strcmp(users[i].name, name) == 0) /* Found user with the specified name*/
        {
            users[i] = *user;
            ret = 0; /* return 0 - Success */

            i = user_count; /* break loop */
        }
    }

    return ret;
}
