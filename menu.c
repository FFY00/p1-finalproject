#include "menu.h"

/**
 *  @brief  Shows the menu and gets the desired action
 *
 *  @param  Option pointer
 */
void main_menu(char *opt, int user_count, int loan_count, double distance, int pending_loan_count, int bike_info)
{
    /* check if the variables are initialized */
    if(user_count < 0)
    {
        user_count = 0;
    }

    if(loan_count < 0){
        loan_count = 0;
    }

    if(pending_loan_count < 0){
        pending_loan_count = 0;
    }

    int valid = 0;
    do {
        printf("\n\t\t\tGestao de Bicicletas\n");
        printf("Utentes: %d\t\t\t", user_count);
        printf("Distancia Percorrida: %g\n", distance);
        printf("Emprestimos: %d\t\t\t", loan_count);
        printf("Lista de Espera: %d\n", pending_loan_count);
        printf("\t(B)icicletas\n");
        printf("\t(U)tentes\n");
        printf("\t(E)mprestimos\n");

        if(bike_info == 1)
        {
            printf("\t(A)varias\n");
        }

        printf("\tEs(T)atisticas\n");
        printf("\t(S)air\n");
        printf("Selecione a opcao: ");

        scanf(" %c", opt);
        clear_buffer();

        *opt = tolower(*opt);

        if (*opt == 'b' || *opt == 'u' || *opt == 'e' || *opt == 't' ||
            *opt == 's')
        {
            valid = 1;
        }

        if (*opt == 'a' && bike_info == 1)
        {
            valid = 1;
        }

    } while(valid != 1);
}

    /* Bike */

/**
 *  @brief  Shows the bike menu and gets the desired action
 *
 *  @param  Option pointer
 */
void bike_menu(char *opt, int bike_info)
{
    int valid = 0;
    do {
        printf("\n\t\t\tGestao de Bicicletas - Bicicletas\n");
        printf("\t(I)nserir\n");

        if(bike_info == 1)
        {
            printf("\t(C)onsultar\n");
            printf("\t(L)istar\n");
            printf("\t(G)uardar\n");
        }

        printf("\t(V)oltar\n");
        printf("\t(S)air\n");
        printf("Selecione a opcao: ");

        scanf(" %c", opt);
        clear_buffer();

        *opt = tolower(*opt);

        if (*opt == 'i' ||
            *opt == 'v' || *opt == 's')
        {
            valid = 1;
        }

        if(bike_info == 1 && (*opt == 'c' || *opt == 'l' || *opt == 'g'))
        {
            valid = 1;
        }

    } while(valid != 1);
}

/**
 *  @brief  Shows bike insert the menu and gets the desired action
 *
 *  @param  Option pointer
 */
void bike_insert_menu(char *opt)
{
    int valid = 0;
    do {
        printf("\n\t\t\tGestao de Bicicletas - Inserir Bicicletas\n");
        printf("\t(F)icheiro (Ler dados do ficheiro 'bike.dat')\n");
        printf("\t(M)anualmente (Inserir dados manualmente)\n");
        printf("\t(V)oltar\n");
        printf("\t(S)air\n");
        printf("Selecione a opcao: ");

        scanf(" %c", opt);
        clear_buffer();

        *opt = tolower(*opt);

        if (*opt == 'f' || *opt == 'm' ||
            *opt == 'v' || *opt == 's')
        {
            valid = 1;
        }

    } while(valid != 1);
}

    /* User */

/**
 *  @brief  Shows the user menu and gets the desired action
 *
 *  @param  Option pointer
 */
void user_menu(char *opt, int user_count)
{
    int valid = 0;
    do {
        printf("\n\t\t\tGestao de Bicicletas - Utentes\n");
        printf("\t(I)nserir\n");

        if(user_count > 0)
        {
            printf("\t(A)lterar\n");
            printf("\t(C)onsultar\n");
            printf("\t(L)istar\n");
            printf("\t(G)uardar\n");
        }

        printf("\t(V)oltar\n");
        printf("\t(S)air\n");
        printf("Selecione a opcao: ");

        scanf(" %c", opt);
        clear_buffer();

        *opt = tolower(*opt);

        if (*opt == 'i' ||
            *opt == 'v' || *opt == 's')
        {
            valid = 1;
        }

        if(user_count > 0 && (*opt == 'a' || *opt == 'c' ||
                                *opt == 'l' || *opt == 'g'))
        {
            valid = 1;
        }

    } while(valid != 1);
}

/**
 *  @brief  Shows the user insert menu and gets the desired action
 *
 *  @param  Option pointer
 */
void user_insert_menu(char *opt)
{
    int valid = 0;
    do {
        printf("\n\t\t\tGestao de Bicicletas - Inserir Utente\n");
        printf("\t(F)icheiro (Ler dados do ficheiro 'user.dat')\n");
        printf("\t(M)anualmente (Inserir dados manualmente)\n");
        printf("\t(V)oltar\n");
        printf("\t(S)air\n");
        printf("Selecione a opcao: ");

        scanf(" %c", opt);
        clear_buffer();

        *opt = tolower(*opt);

        if (*opt == 'f' || *opt == 'm' ||
            *opt == 'v' || *opt == 's')
        {
            valid = 1;
        }

    } while(valid != 1);
}

    /* Loan */

/**
 *  @brief  Shows the loan menu and gets the desired action
 *
 *  @param  Option pointer
 */
void loan_menu(char *opt, int loan_count, int user_count, int bike_info)
{
    int valid = 0;
    do {
        printf("\n\t\t\tGestao de Bicicletas - Emprestimos\n");

        if(user_count > 0 && bike_info == 1)
        {
            printf("\t(P)edido\n");
        }

        if(loan_count > 0)
        {
            printf("\t(D)evolucao\n");
            printf("\t(L)ista de Espera\n");
            printf("\tLista de (E)mprestimos\n");
            printf("\t(A)lterar Emprestimo\n");
            printf("\tCa(N)celar Emprestimo\n");
            printf("\t(G)uardar\n");
        }

        if(user_count > 0 && bike_info == 1)
        {
            printf("\t(H)istorico\n");
        }

        printf("\tCri(T)erio de selecao\n");
        printf("\t(V)oltar\n");
        printf("\t(S)air\n");
        printf("Selecione a opcao: ");

        scanf(" %c", opt);
        clear_buffer();

        *opt = tolower(*opt);

        if (*opt == 't' ||
            *opt == 'v' || *opt == 's')
        {
            valid = 1;
        }

        if(user_count > 0 && bike_info == 1 && (*opt == 'p' || *opt == 'h'))
        {
            valid = 1;
        }

        if(loan_count > 0 && (
            *opt == 'd' || *opt == 'l' || *opt == 'e' ||
            *opt == 'g' || *opt == 'a' || *opt == 'n'))
        {
            valid = 1;
        }

    } while(valid != 1);
}

/**
 *  @brief  Shows the loan insert menu and gets the desired action
 *
 *  @param  Option pointer
 */
void loan_insert_menu(char *opt)
{
    int valid = 0;
    do {
        printf("\n\t\t\tGestao de Bicicletas - Inserir Emprestimo\n");
        printf("\t(F)icheiro (Ler dados do ficheiro 'loan.dat')\n");
        printf("\t(M)anualmente (Inserir dados manualmente)\n");
        printf("\t(V)oltar\n");
        printf("\t(S)air\n");
        printf("Selecione a opcao: ");

        scanf(" %c", opt);
        clear_buffer();

        *opt = tolower(*opt);

        if (*opt == 'f' || *opt == 'm' ||
            *opt == 'v' || *opt == 's')
        {
            valid = 1;
        }

    } while(valid != 1);
}

    /* Bike History */

/**
 *  @brief  Shows the bike history menu and gets the desired action
 *
 *  @param  Option pointer
 */
void bike_history_menu(char *opt, int bike_history_count)
{
    int valid = 0;
    do {
        printf("\n\t\t\tGestao de Bicicletas - Avarias\n");
        printf("\t(R)eportar\n");

        if(bike_history_count > 0)
        {
            printf("\t(F)inalizar\n");
            printf("\t(L)istar\n");
            printf("\t(G)uardar\n");
        }

        printf("\t(V)oltar\n");
        printf("\t(S)air\n");
        printf("Selecione a opcao: ");

        scanf(" %c", opt);
        clear_buffer();

        *opt = tolower(*opt);

        if (*opt == 'r' ||
            *opt == 'v' || *opt == 's')
        {
            valid = 1;
        }

        if(bike_history_count > 0 && (*opt == 'f' || *opt == 'l' || *opt == 'g'))
        {
            valid = 1;
        }

    } while(valid != 1);
}

/**
 *  @brief  Shows the bike history insert menu and gets the desired action
 *
 *  @param  Option pointer
 */
void bike_history_insert_menu(char *opt)
{
    int valid = 0;
    do {
        printf("\n\t\t\tGestao de Bicicletas - Reportar avarias\n");
        printf("\t(F)icheiro (Ler dados do ficheiro 'bike_history.dat')\n");
        printf("\t(M)anualmente (Inserir dados manualmente)\n");
        printf("\t(V)oltar\n");
        printf("\t(S)air\n");
        printf("Selecione a opcao: ");

        scanf(" %c", opt);
        clear_buffer();

        *opt = tolower(*opt);

        if (*opt == 'f' || *opt == 'm' ||
            *opt == 'v' || *opt == 's')
        {
            valid = 1;
        }

    } while(valid != 1);
}

/**
 *  @brief  Shows the bike history menu and gets the desired action
 *
 *  @param  Option pointer
 */
void stats_menu(char *opt)
{
    int valid = 0;
    do {
        printf("\n\t\t\tGestao de Bicicletas - Estatisticas\n");
        printf("\t(G)eral\n");
        printf("\tQuantidade de (E)mprestimos efetuados entre duas datas\n");
        printf("\tQuantidade de (U)tentes que utilizaram certa bicicleta\n");
        printf("\tUltima (B)icicleta usada por certo utente\n");
        printf("\t(L)istar utentes por ordem decrescente\n");
        printf("\t(V)oltar\n");
        printf("\t(S)air\n");
        printf("Selecione a opcao: ");

        scanf(" %c", opt);
        clear_buffer();

        *opt = tolower(*opt);

        if (*opt == 'g' || *opt == 'e' || *opt == 'u' ||
            *opt == 'b' || *opt == 'l' ||
            *opt == 'v' || *opt == 's')
        {
            valid = 1;
        }

    } while(valid != 1);
}
