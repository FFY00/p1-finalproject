#include "campus.h"

/**
 *  @brief  Popultes campus data
 */
t_campus* populate_campus_data(t_bike bikes[MAX_BIKES], t_campus *campus, int *campus_count)
{
    int i, ii, exists;
    for(i = 0; i < MAX_BIKES; i++) /* iterate over bikes array */
    {
        exists = 0;
        for(ii = 0; ii < *campus_count; ii++) /* iterate over campus array */
        {
            if(strcmp(bikes[i].campus, campus[ii].id) == 0) /* campus already exists */
            {
                exists = 1;
                ii = *campus_count; /* exit campus loop */
            }
        }

        if(exists == 0) /* doesn't exist, insert */
        {
            campus = realloc(campus, sizeof(t_campus) * ++(*campus_count));
            strncpy(campus[*campus_count-1].id, bikes[i].campus, MAX_STRING);

            printf("\n\t%s - Coordenadas\n", campus[*campus_count-1].id);
            read_double(&campus[*campus_count-1].x, -DBL_MAX, DBL_MAX, "X: ");
            read_double(&campus[*campus_count-1].y, -DBL_MAX, DBL_MAX, "Y: ");
            read_double(&campus[*campus_count-1].z, -DBL_MAX, DBL_MAX, "Z: ");
        }
    }

    return campus;
}

/**
 *  @brief  Calculates distance between two campus
 */
void calculate_campus_distance(double *distance, char id1[MAX_STRING], char id2[MAX_STRING], t_campus *campus, int campus_count)
{
    t_campus campus1, campus2;

    int i;
    for(i = 0; i < campus_count; i++)
    {
        if(strcmp(campus[i].id, id1) == 0)
        {
            campus1 = campus[i];
        }
        if(strcmp(campus[i].id, id2) == 0)
        {
            campus2 = campus[i];
        }
    }

    *distance = sqrt(   (campus1.x - campus2.x) * (campus1.x - campus2.x) +
                        (campus1.y - campus2.y) * (campus1.y - campus2.y) +
                        (campus1.z - campus2.z) * (campus1.z - campus2.z));
}
