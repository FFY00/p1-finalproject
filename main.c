#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#include "bike.h"
#include "bike_history.h"
#include "campus.h"
#include "file.h"
#include "loan.h"
#include "menu.h"
#include "stats.h"
#include "structs.h"
#include "user.h"
#include "util.h"

int main()
{
    t_bike bikes[MAX_BIKES];
    int bike_info = 0; /* is bikes array populated? */

    t_campus *campus = NULL; /* campus dynamic array */
    int campus_count = 0;

    t_user *users = NULL; /* user dynamic array */
    int user_count = 0; /* user counter */

    t_loan *loans = NULL; /* loan dynamic array */
    int loan_count = 0; /* loan counter */
    int pending_loan_count = 0;

    double distance = 0;

    t_bike_history *bike_history = NULL; /* bike history dynamic array */
    int bike_history_count = 0; /* bike history counter */

    int loan_criteria = 0; /* Loan criteria:
                                0   By date
                                1   By user type
                                2   By distance */

    char opt; /* holds selected option when prompting a menu */

    /* helper variables */
    t_user user;
    int ret, i, ii, valid;
    double dtemp;
    char temp[MAX_STRING];

    int exit = 0; /* control variable to exit the loop */
    int menu = 0; /* Menu:
                    0 - Main
                        1 - Bike
                            11 - Bike Insert
                        2 - User
                            21 - User Insert
                        3 - Loans
                            31 - Loan Insert
                        4 - Bike history
                            41 - Bike history insert
                        5 - Stats */
    while(exit == 0)
    {

        switch(menu)
        {
            default:
            case 0: /* Main menu */
                main_menu(&opt, user_count, loan_count, distance, pending_loan_count, bike_info); /* Prompt main menu and get option */
                switch(opt)
                {
                    case 'b': /* Bicicletas */
                        menu = 1; /* Enter bike submenu */
                        break;

                    case 'u': /* Utentes */
                        menu = 2; /* Enter user submenu */
                        break;

                    case 'e': /* Emprestimos */
                        menu = 3; /* Enter loan submenu */
                        break;

                    case 'a': /* Avarias */
                        menu = 4; /* Enter bike history submenu */
                        break;

                    case 't': /* Estatisticas */
                        menu = 5; /* Enter stats submenu */
                        break;

                    case 's': /* Sair */
                        exit = 1;
                        break;
                }
                break;

            /* Submenus */

            case 1: /* Bike (submenu) */
                bike_menu(&opt, bike_info); /* Prompt bike menu and get otpion*/
                switch(opt)
                {
                    case 'i': /* Inserir */
                        menu = 11;
                        break;

                    case 'c': /* Consultar */
                        /* Get target bike ID */
                        printf("\nID da bicicleta: ");

                        char id[MAX_STRING];
                        read(id);
                        normalize_string(id);

                        /* Get target bike */
                        t_bike bike;
                        ret = get_bike(&bike, bikes, id);

                        if(ret == 0) /* Bike with specified ID exists */
                        {
                            char status_desc[MAX_STRING];
                            get_status_str(status_desc, bike.state); /* Get bike status string */
                            get_bike_id(&i, bikes, id);

                            printf("\nNome da bicicleta: %s\n", bike.id);
                            printf("Modelo da bicicleta: %s\n", bike.model);
                            printf("Estado da bicicleta: %s\n", status_desc);
                            printf("Campus: %s\n", bike.campus);

                            calculate_bike_loan_count(&ii, loans, loan_count, i);
                            printf("Numero total de emprestimos: %d\n", ii);

                            calculate_bike_distance(&ii, loans, loan_count, i);
                            printf("Distancia percorrida: %d\n", ii);
                        } else {
                            printf("\nNao existe nenhum bicicleta com esse ID.\n");
                        }

                        break;

                    case 'l': /* Listar */
                        printf("\n\tLista de Bicicletas\n");
                        for(i = 0; i < MAX_BIKES; i++)
                        {
                            char status_desc[MAX_STRING];
                            get_status_str(status_desc, bikes[i].state); /* Get bike status string */

                            printf("\nNome da bicicleta: %s\n", bikes[i].id);
                            printf("Modelo da bicicleta: %s\n", bikes[i].model);
                            printf("Estado da bicicleta: %s\n", status_desc);
                            printf("Campus: %s\n", bikes[i].campus);

                            calculate_bike_loan_count(&ii, loans, loan_count, i);
                            printf("Numero total de emprestimos: %d\n", ii);

                            calculate_bike_distance(&ii, loans, loan_count, i);
                            printf("Distancia percorrida: %d\n", ii);
                        }
                        break;

                    case 'g': /* Guardar */
                        save_bike_data(bikes, campus, campus_count); /* Saves bike info */
                        break;

                    case 'v': /* Voltar */
                        menu = 0; /* Return to main menu */
                        break;

                    case 's': /* Sair */
                        exit = 1; /* Exit program */
                        break;
                }
                break;

            case 2: /* User (submenu) */
                user_menu(&opt, user_count);
                switch(opt)
                {
                    case 'i': /* Inserir */
                        if(user_count > 0) /* we only allow to load the data from file if the array is empty */
                        {
                            users = insert_user(users, &user_count); /* manual read */
                            menu = 2;
                        } else {
                            menu = 21;
                        }
                        break;

                    case 'a': /* Alterar */
                        /* Get target user name */
                        printf("\nNome do utente: ");

                        read(temp);

                        /* Get target user */
                        ret = get_user(&user, users, user_count, temp);

                        if(ret == 0) /* User with specified name exists */
                        {
                            change_user_data(&user);
                            set_user(&user, users, user_count, temp);
                        } else {
                            printf("\nNao existe nenhum utente com esse nome.\n");
                        }
                        break;

                    case 'c': /* Consultar */
                        /* Get target user name */
                        printf("\nNome do utente: ");

                        read(temp);

                        /* Get target user */
                        t_user user;
                        ret = get_user(&user, users, user_count, temp);

                        if(ret == 0) /* User with specified name exists */
                        {
                            get_user_type_str(temp, user.type);

                            printf("\nNumero de idendificacao: %d\n", user.id);
                            printf("Nome do utente: %s\n", user.name);
                            printf("Numero de telfone: %s\n", user.phone);
                            printf("Tipo de utente: %s\n", temp);

                            calculate_user_loan_count(&i, loans, loan_count, user.id);
                            printf("Numero total de emprestimos: %d\n", i);

                            calculate_bike_distance(&i, loans, loan_count, user.id);
                            printf("Distancia percorrida: %d\n", i);
                        } else {
                            printf("\nNao existe nenhum utente com esse nome.\n");
                        }

                        break;

                    case 'l': /* Listar */
                        printf("\n\tLista de Utentes\n");

                        for(i = 0; i < user_count; i++)
                        {
                            get_user_type_str(temp, users[i].type);

                            printf("\nNumero de utilizador: %d\n", users[i].id);
                            printf("Nome do utilizador: %s\n", users[i].name);
                            printf("Numero de telfone: %s\n", users[i].phone);
                            printf("Tipo de utente: %s\n", temp);

                            calculate_user_loan_count(&ii, loans, loan_count, i);
                            printf("Numero total de emprestimos: %d\n", ii);

                            calculate_user_distance(&ii, loans, loan_count, i);
                            printf("Distancia percorrida: %d\n", ii);
                        }
                        break;

                    case 'g': /* Guardar */
                        save_user_data(users, user_count);
                        break;

                    case 'v': /* Voltar */
                        menu = 0; /* Return to main menu */
                        break;

                    case 's': /* Sair */
                        exit = 1; /* Exit program */
                        break;
                }
                break;

            case 3: /* Loan (submenu) */
                loan_menu(&opt, loan_count, user_count, bike_info);
                switch(opt)
                {
                    case 'p': /* Pedido */
                        if(loan_count > 0)
                        {
                            loans = insert_loan(loans, &loan_count, bikes, user_count, campus, campus_count);
                            resolve_loan_requests(loans, loan_count, loan_criteria, bikes, users);
                            calculate_pending_loans(&pending_loan_count, loans, loan_count);
                        } else {
                            menu = 31;
                        }
                        break;

                    case 'd': /* Devolucao */
                        printf("\n");

                        /* read loan id */
                        read_int(&i, 0, loan_count - 1, "ID do Emprestimo: ");

                        /* read distance */
                        calculate_campus_distance(&dtemp, loans[i].origin, loans[i].destination, campus, campus_count);
                        read_double(&loans[i].distance, dtemp, DBL_MAX, "Distancia: ");

                        get_loan_request_end_data(&loans[i], i);

                        bikes[loans[i].bike_id].state = 0; /* avaliable */
                        distance += loans[i].distance; /* update distance */

                        resolve_loan_requests(loans, loan_count, loan_criteria, bikes, users);
                        calculate_pending_loans(&pending_loan_count, loans, loan_count);
                        break;

                    case 'l': /* Lista de Espera */
                        printf("\n\tLista de Espera\n");
                        ret = 0; /* was at lest one loan printed? */
                        for(i = 0; i < loan_count; i++)
                        {
                            if(loans[i].status == 0) /* pending */
                            {
                                ret = 1;

                                printf("\nID do Emprestimo: %d\n", loans[i].id);
                                printf("ID do Utente: %d\n", loans[i].user_id);
                                printf("Origem: %s\n", loans[i].origin);
                                printf("Destino: %s\n", loans[i].destination);
                                printf("Data de inicio: %02d/%02d/%d %02d:%02d:%02d\n", loans[i].pickup_time.day, loans[i].pickup_time.month,
                                    loans[i].pickup_time.year, loans[i].pickup_time.hour, loans[i].pickup_time.min, loans[i].pickup_time.secs);
                            }
                        }
                        if(ret == 0) /* no loans were printed */
                        {
                            printf("Nao existem emprestimos em espera.\n");
                        }

                        break;

                    case 'e': /* Lista de Emprestimos */
                        printf("\n\tLista de Emprestimos\n");
                        for(i = 0; i < loan_count; i++)
                        {
                            get_loan_status_str(temp, loans[i].status);

                            printf("\nID do Emprestimo: %d\n", loans[i].id);
                            printf("ID do Utente: %d\n", loans[i].user_id);
                            printf("Origem: %s\n", loans[i].origin);
                            printf("Destino: %s\n", loans[i].destination);

                            if(loans[i].status == 1 || loans[i].status == 2) /* in progress or finalized */
                            {
                                printf("Bicicleta: %s\n", bikes[loans[i].bike_id].id);
                            }

                            printf("Data de inicio: %02d/%02d/%d %02d:%02d:%02d\n", loans[i].pickup_time.day, loans[i].pickup_time.month,
                                loans[i].pickup_time.year, loans[i].pickup_time.hour, loans[i].pickup_time.min, loans[i].pickup_time.secs);

                            if(loans[i].status == 2) /* finalized */
                            {
                                printf("Data de finalizacao: %02d/%02d/%d %02d:%02d:%02d\n", loans[i].return_time.day, loans[i].return_time.month,
                                    loans[i].return_time.year, loans[i].return_time.hour, loans[i].return_time.min, loans[i].return_time.secs);
                                printf("Distancia percorrida: %g\n", loans[i].distance);
                            }

                            printf("Estado: %s\n", temp);
                        }
                        break;

                    case 'c': /* Consultar */
                        printf("\n");
                        read_int(&i, 0, loan_count - 1, "ID do Emprestimo: ");

                        get_loan_status_str(temp, loans[i].status);

                        printf("\nID do Emprestimo: %d\n", loans[i].id);
                        printf("ID do Utente: %d\n", loans[i].user_id);
                        printf("Origem: %s\n", loans[i].origin);
                        printf("Destino: %s\n", loans[i].destination);

                        if(loans[i].status == 1 || loans[i].status == 2) /* in progress or finalized */
                        {
                            printf("Bicicleta: %s\n", bikes[loans[i].bike_id].id);
                        }

                        printf("Data de inicio: %02d/%02d/%d %02d:%02d:%02d\n", loans[i].pickup_time.day, loans[i].pickup_time.month,
                            loans[i].pickup_time.year, loans[i].pickup_time.hour, loans[i].pickup_time.min, loans[i].pickup_time.secs);

                        if(loans[i].status == 2) /* finalized */
                        {
                            printf("Data de finalizacao: %02d/%02d/%d %02d:%02d:%02d\n", loans[i].return_time.day, loans[i].return_time.month,
                                loans[i].return_time.year, loans[i].return_time.hour, loans[i].return_time.min, loans[i].return_time.secs);
                            printf("Distancia percorrida: %g\n", loans[i].distance);
                        }

                        printf("Estado: %s\n", temp);
                        break;

                    case 'a': /* Alterar Emprestimo */
                        printf("\n");
                        read_int(&i, 0, loan_count - 1, "ID do Emprestimo: ");

                        if(loans[i].status == 0)
                        {
                            valid = 0;
                            do {
                                printf("Campus de Destino: ");
                                read(loans[i].destination);
                                normalize_string(loans[i].destination);

                                for(ii = 0; ii < campus_count; ii++) /* iterate over campus array */
                                {
                                    if(strcmp(campus[ii].id, loans[ii].destination) == 0) /* destination campus is in array */
                                    {
                                        valid = 1;
                                        ii = campus_count; /* exit loop */
                                    }
                                }

                                if(valid == 0)
                                {
                                    printf("\nERRO: Campus indesponivel. Os disponiveis sao:\n");
                                    for(ii = 0; i < campus_count; ii++)
                                    {
                                        printf("\t- %s\n", campus[ii].id);
                                    }
                                    printf("\n");
                                }
                            } while(valid == 0);

                            resolve_loan_requests(loans, loan_count, loan_criteria, bikes, users);
                            calculate_pending_loans(&pending_loan_count, loans, loan_count);
                        } else {
                            printf("\nERRO: O emprestimo nao esta na lista de espera\n");
                        }
                        break;

                    case 'n': /* Cancelar Emprestimo */
                        printf("\n");
                        read_int(&i, 0, loan_count - 1, "ID do Emprestimo: ");

                        if(loans[i].status == 0)
                        {
                            loans[i].status = 3;

                            resolve_loan_requests(loans, loan_count, loan_criteria, bikes, users);
                            calculate_pending_loans(&pending_loan_count, loans, loan_count);
                        } else {
                            printf("\nERRO: O emprestimo nao esta na lista de espera\n");
                        }
                        break;

                    case 'h': /* Historico */
                        printf("\n");
                        read_int(&ii, 0, user_count - 1, "ID do Utente: ");

                        for(i = 0; i < loan_count; i++)
                        {
                            if(loans[i].user_id == ii)
                            {
                                get_loan_status_str(temp, loans[i].status);

                                printf("\nID do Emprestimo: %d\n", loans[i].id);
                                printf("ID do Utente: %d\n", loans[i].user_id);
                                printf("Origem: %s\n", loans[i].origin);
                                printf("Destino: %s\n", loans[i].destination);

                                if(loans[i].status == 1 || loans[i].status == 2) /* in progress or finalized */
                                {
                                    printf("Bicicleta: %s\n", bikes[loans[i].bike_id].id);
                                }

                                printf("Data de inicio: %02d/%02d/%d %02d:%02d:%02d\n", loans[i].pickup_time.day, loans[i].pickup_time.month,
                                    loans[i].pickup_time.year, loans[i].pickup_time.hour, loans[i].pickup_time.min, loans[i].pickup_time.secs);

                                if(loans[i].status == 2) /* finalized */
                                {
                                    printf("Data de finalizacao: %02d/%02d/%d %02d:%02d:%02d\n", loans[i].return_time.day, loans[i].return_time.month,
                                        loans[i].return_time.year, loans[i].return_time.hour, loans[i].return_time.min, loans[i].return_time.secs);
                                    printf("Distancia percorrida: %g\n", loans[i].distance);
                                }

                                printf("Estado: %s\n", temp);
                            }
                        }
                        break;

                    case 't': /* Criterio de selecao */
                        set_loan_criteria(&loan_criteria);
                        break;

                    case 'g': /* Guardar */
                        save_loan_data(loans, loan_count);
                        break;

                    case 'v': /* Voltar */
                        menu = 0; /* Return to main menu */
                        break;

                    case 's': /* Sair */
                        exit = 1; /* Exit program */
                        break;
                }
                break;

            case 4: /* Bike history (submenu) */
                bike_history_menu(&opt, bike_history_count);
                switch(opt)
                {
                    case 'r': /* Reportar */
                        if(bike_history_count > 0)
                        {
                            bike_history = insert_bike_history_report(bike_history, &bike_history_count, loans, loan_count, bikes);

                            int loan = -1;
                            for(i = 0; i < loan_count; i++)
                            {
                                if(loans[i].status == 1) /* in progress */
                                {
                                    if(strcmp(bikes[loans[i].bike_id].id, bikes[bike_history[bike_history_count-1].bike_id].id) == 0) /* loan in progress */
                                    {
                                        loan = i;
                                    }
                                }
                            }

                            if(loan != -1) /* loan in progress with target bike, finalize loan */
                            {
                                loans[i].status = 2; /* finalized */
                                resolve_loan_requests(loans, loan_count, loan_criteria, bikes, users);
                                calculate_pending_loans(&pending_loan_count, loans, loan_count);
                            }
                        } else {
                            menu = 41;
                        }
                        break;

                    case 'f': /* Finalizar */
                            printf("\nNome da bicicleta: ");
                            read(temp);
                            normalize_string(temp);

                            int i;
                            for(i = 0; i < MAX_BIKES; i++)
                            {
                                if(strcmp(temp, bikes[i].id) == 0)
                                {
                                    bikes[i].state = 0; /* avaliable */
                                }
                            }

                            write_finalize_report_log(bikes[i].id);
                            resolve_loan_requests(loans, loan_count, loan_criteria, bikes, users);
                            calculate_pending_loans(&pending_loan_count, loans, loan_count);
                        break;

                    case 'l': /* Listar */
                        printf("\n\tLista de Avarias\n");
                        for(i = 0; i < bike_history_count; i++)
                        {
                            printf("\nID da avaria: %d\n", bike_history[i].id);
                            printf("Bicicleta: %s\n", bikes[bike_history[i].bike_id].id);
                            printf("Descricao: %s\n", bike_history[i].description);
                        }
                        break;

                    case 'g': /* Guardar */
                        save_bike_history_data(bike_history, bike_history_count);
                        break;

                    case 'v': /* Voltar */
                        menu = 0; /* Return to main menu */
                        break;

                    case 's': /* Sair */
                        exit = 1; /* Exit program */
                        break;
                }
                break;

            case 5: /* Stats (submenu) */
                stats_menu(&opt);
                switch(opt)
                {
                    case 'g': /* Geral */
                        calculate_average_distance(&dtemp, loans, loan_count);
                        printf("\nDistancia media percorrida: %g\n", dtemp);

                        float stats[4];
                        calculate_user_type_loan_stats(stats, loans, loan_count, users, user_count);
                        printf("\nDistribuicao dos emprestimos por tipo de utente:\n");
                        printf("\tAluno\t\t\t%g%%\n", stats[0]);
                        printf("\tDocente\t\t\t%g%%\n", stats[1]);
                        printf("\tTecnico Administrativo\t%g%%\n", stats[2]);
                        printf("\tConvidado\t\t%g%%\n", stats[3]);

                        char **higher_campus = NULL;
                        int higher_campus_count = 0;
                        higher_campus = get_campus_with_higher_loan_count(higher_campus, &higher_campus_count, loans, loan_count);
                        printf("\nCampus de origem com maior numero de emprestimos:\n");
                        for(i = 0; i < higher_campus_count; i++)
                        {
                            printf("\t- %s\n", higher_campus[i]);
                            free(higher_campus[i]);
                        }
                        free(higher_campus);
                        break;

                    case 'e': /* Quantidade de emprestimos efetuados entre duas datas */
                        printf("\n\tData inicial\n");

                        t_time start_time, end_time;

                        read_int(&start_time.year, 0, INT_MAX, "Ano: ");
                        read_int(&start_time.month, 1, 12, "Mes: ");

                        /* calculate max days */
                        calculate_month_days(&ret, start_time.month, start_time.year);

                        read_int(&start_time.day, 1, ret, "Dia: ");

                        read_int(&start_time.hour, 0, 23, "Hora: ");
                        read_int(&start_time.min, 0, 59, "Minuto: ");
                        read_int(&start_time.secs, 0, 59, "Segundo: ");

                        printf("\n\tData final\n");
                        read_int(&end_time.year, 0, INT_MAX, "Ano: ");
                        read_int(&end_time.month, 1, 12, "Mes: ");

                        /* calculate max days */
                        calculate_month_days(&ret, end_time.month, end_time.year);

                        read_int(&end_time.day, 1, ret, "Dia: ");

                        read_int(&end_time.hour, 0, 23, "Hora: ");
                        read_int(&end_time.min, 0, 59, "Minuto: ");
                        read_int(&end_time.secs, 0, 59, "Segundo: ");

                        ii = 0;
                        for(i = 0; i < loan_count; i++)
                        {
                            if(compare_time(loans[i].pickup_time, start_time) >= 0 &&
                                compare_time(loans[i].pickup_time, end_time) <= 0)
                            {
                                ii++;
                            }
                        }

                        printf("\n%d emprestimos foram realizados entre as duas datas\n", ii);
                        break;

                    case 'u': /* Quantidade de utentes que utilllizaram certa bicicleta */
                        printf("\nNome da bicicleta: ");
                        read(temp);
                        normalize_string(temp);

                        calculate_bike_user_count(&i, loans, loan_count, temp, bikes);

                        printf("\n%d utentes usaram esta biciclleta.\n", i);
                        break;

                    case 'b': /* Ultima bicicleta usada por certo utente */
                        printf("\n");
                        read_int(&ii, 0, user_count - 1, "ID do Utente: ");

                        for(i = loan_count-1; i >= 0; i--)
                        {
                            if(loans[i].user_id == ii)
                            {
                                get_status_str(temp, bikes[loans[i].bike_id].state); /* Get bike status string */

                                printf("\nNome da bicicleta: %s\n", bikes[loans[i].bike_id].id);
                                printf("Modelo da bicicleta: %s\n", bikes[loans[i].bike_id].model);
                                printf("Estado da bicicleta: %s\n", temp);
                                printf("Campus: %s\n", bikes[loans[i].bike_id].campus);

                                calculate_bike_loan_count(&ii, loans, loan_count, i);
                                printf("Numero total de emprestimos: %d\n", ii);

                                calculate_bike_distance(&ii, loans, loan_count, i);
                                printf("Distancia percorrida: %d\n", ii);

                                ret = i;

                                i = -1; /* exit loop */
                            }
                        }

                        printf("\n\tHistorico de emprestimos do utilizador\n");

                        for(i = 0; i < loan_count; i++)
                        {
                            if(loans[i].user_id == loans[ret].user_id)
                            {
                                get_loan_status_str(temp, loans[i].status);

                                printf("\nID do Emprestimo: %d\n", loans[i].id);
                                printf("ID do Utente: %d\n", loans[i].user_id);
                                printf("Origem: %s\n", loans[i].origin);
                                printf("Destino: %s\n", loans[i].destination);

                                if(loans[i].status == 1 || loans[i].status == 2) /* in progress or finalized */
                                {
                                    printf("Bicicleta: %s\n", bikes[loans[i].bike_id].id);
                                }

                                printf("Data de inicio: %02d/%02d/%d %02d:%02d:%02d\n", loans[i].pickup_time.day, loans[i].pickup_time.month,
                                    loans[i].pickup_time.year, loans[i].pickup_time.hour, loans[i].pickup_time.min, loans[i].pickup_time.secs);

                                if(loans[i].status == 2) /* finalized */
                                {
                                    printf("Data de finalizacao: %02d/%02d/%d %02d:%02d:%02d\n", loans[i].return_time.day, loans[i].return_time.month,
                                        loans[i].return_time.year, loans[i].return_time.hour, loans[i].return_time.min, loans[i].return_time.secs);
                                    printf("Distancia percorrida: %g\n", loans[i].distance);
                                }

                                printf("Estado: %s\n", temp);
                            }
                        }

                        break;

                    case 'l': /* Listar utentes por ordem decrescente */
                        printf("\nLista ordenada por emprestimos:");
                        t_user *sort = sort_users_by_loan(users, user_count, loans, loan_count);
                        for(i = user_count; i >= 0; i--)
                        {
                            printf("\t%s\n", sort[i].name);
                        }
                        break;

                    case 'v': /* Voltar */
                        menu = 0; /* Return to main menu */
                        break;

                    case 's': /* Sair */
                        exit = 1; /* Exit program */
                        break;
                }
                break;

            /* Sub-submenus */

            case 11: /* Bike insert */
                bike_insert_menu(&opt); /* Prompt bike insert menu and get otpion*/
                switch(opt)
                {
                    case 'f': /* Ficheiro */
                        /* Populate bikes array (reads from file or asks the user) */
                        campus = read_bike_data(bikes, campus, &campus_count);
                        bike_info = 1;
                        menu = 1; /* Return to bike menu */
                        break;

                    case 'm': /* Manualmente */
                        populate_bike_data(bikes);
                        bike_info = 1;
                        campus = populate_campus_data(bikes, campus, &campus_count);
                        menu = 1; /* Return to bike menu */
                        break;

                    case 'v': /* Voltar */
                        menu = 1; /* Return to bike menu */
                        break;

                    case 's': /* Sair */
                        exit = 1; /* Exit program */
                        break;
                }
                break;

            case 21: /* User insert */
                user_insert_menu(&opt); /* Prompt user insert menu and get otpion*/
                switch(opt)
                {
                    case 'f': /* Ficheiro */
                        users = read_user_data(users, &user_count);
                        menu = 2; /* Return to user menu */
                        break;

                    case 'm': /* Manualmente */
                        users = insert_user(users, &user_count);
                        menu = 2; /* Return to user menu */
                        break;

                    case 'v': /* Voltar */
                        menu = 2; /* Return to user menu */
                        break;

                    case 's': /* Sair */
                        exit = 1; /* Exit program */
                        break;
                }
                break;

            case 31: /* Loan insert */
                loan_insert_menu(&opt); /* Prompt loan insert menu and get otpion*/
                switch(opt)
                {
                    case 'f': /* Ficheiro */
                        loans = read_loan_data(loans, &loan_count);

                        /* calculate distance */
                        for(i = 0; i < loan_count; i++)
                        {
                            distance += loans[i].distance;
                        }

                        calculate_pending_loans(&pending_loan_count, loans, loan_count);

                        menu = 3; /* Return to loan menu */
                        break;

                    case 'm': /* Manualmente */
                        loans = insert_loan(loans, &loan_count, bikes, user_count, campus, campus_count);
                        resolve_loan_requests(loans, loan_count, loan_criteria, bikes, users);
                        calculate_pending_loans(&pending_loan_count, loans, loan_count);
                        menu = 3; /* Return to loan menu */
                        break;

                    case 'v': /* Voltar */
                        menu = 3; /* Return to loan menu */
                        break;

                    case 's': /* Sair */
                        exit = 1; /* Exit program */
                        break;
                }
                break;

            case 41: /* Bike history insert */
                bike_history_insert_menu(&opt); /* Prompt bike history insert menu and get otpion*/
                switch(opt)
                {
                    case 'f': /* Ficheiro */
                        bike_history = read_bike_history_data(bike_history, &bike_history_count);
                        menu = 4; /* Return to the bike history menu */
                        break;

                    case 'm': /* Manualmente */
                        bike_history = insert_bike_history_report(bike_history, &bike_history_count, loans, loan_count, bikes);

                        int loan = -1;
                        for(i = 0; i < loan_count; i++)
                        {
                            if(loans[i].status == 1) /* in progress */
                            {
                                if(strcmp(bikes[loans[i].bike_id].id, bikes[bike_history[bike_history_count-1].bike_id].id) == 0)
                                {
                                    loan = i;
                                }
                            }
                        }

                        if(loan != -1) /* loan in progress with target bike, finalize loan */
                        {
                            loans[i].status = 2; /* finalized */
                            resolve_loan_requests(loans, loan_count, loan_criteria, bikes, users);
                            calculate_pending_loans(&pending_loan_count, loans, loan_count);
                        }
                        menu = 4; /* Return to the bike history menu */
                        break;

                    case 'v': /* Voltar */
                        menu = 4; /* Return to the bike history menu */
                        break;

                    case 's': /* Sair */
                        exit = 1; /* Exit program */
                        break;
                }
                break;
        }

    }

    if(bike_info == 1 || user_count > 0){
        printf("Dseja guardar os dados? (Y/n) ");
        scanf("%c", &opt); /* Get option from user */
        opt = tolower(opt);

        if(opt == 'y' || opt == '\n')  /* Save all the info */
        {
            if(bike_info == 1)
            {
                save_bike_data(bikes, campus, campus_count); /* Save bike info */
            }

            if(users != NULL)
            {
                save_user_data(users, user_count);
            }

            if(loans != NULL)
            {
                save_loan_data(loans, loan_count);
            }

            if(bike_history != NULL)
            {
                save_bike_history_data(bike_history, bike_history_count);
            }
        }
    }

    free(campus);
    free(loans);
    free(users);
    free(bike_history);

    return 0;
}
