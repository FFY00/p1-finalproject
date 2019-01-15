#ifndef STRUCTS_H_INCLUDED
#define STRUCTS_H_INCLUDED

/* Limits */

#define MAX_STRING  127
#define MAX_PHONE   9
#define MAX_BIKES   15

/* Auxiliar */

/** @struct t_time
 *  @brief  Holds date and time values
 */
typedef struct {
    int day, month, year;
    int secs, min, hour;
} t_time;

/* Main */

/** @struct t_bike
 *  @brief  Holds bike info
 *
 *  @var    t_bike::id
 *  Bike ID
 *
 *  @var    t_bike::model
 *  Bike model
 *
 *  @var    t_bike::state
 *  Bike state
 *  Errors:
 *      0  Available
 *      1  Unavailable
 *      2  Broken
 *
 *  @var    t_bike::campus
 *  Campus where it's parked
 *
 *  @var    t_bike::distance
 *  Total distance the bike was used
 */
typedef struct {
    char id[MAX_STRING];
    char model[MAX_STRING];
    int state;
    char campus[MAX_STRING];
} t_bike;

/** @struct t_user
 *  @brief  Holds user info
 *
 *  @var    t_user::id
 *  User ID
 *
 *  @var    t_user::name
 *  User name
 *
 *  @var     t_user::type
 *  User type
 *      -2  No type
 *      -1  Guest
 *      0   Student
 *      1   Docente
 *      2   Administrative
 *
 *  @var    t_user::phone
 *  User phone number
 *
 *  @var    t_user::last_bike
 *  Last bike used by the user
 */
typedef struct {
    int id;
    char name[MAX_STRING];
    char phone[MAX_PHONE];
    int type;
} t_user;

/** @struct t_loan
 *  @brief  Holds loan info
 *
 *  @var    t_loan::id
 *  Loan ID
 *
 *  @var    t_loan::user_id
 *  User the bike was loaned to
 *
 *  @var    t_loan::bike_id
 *  Bike that was loaned
 *
 *  @var    t_loan::origin
 *  Origin campus
 *
 *  @var    t_loan::destination
 *  Destination campus
 *
 *  @var    t_loan::pickup_time
 *  Bike pickup time
 *
 *  @var    t_loan::return_time
 *  Bike return time
 *
 *  @var    t_loan::distance
 *  Traveled distance
 *
 *  @var    t_loan::status
 *  Loan status:
 *      0   Pending
 *      1   In progress
 *      2   Finalized
 *      3   Cancelled
 *      4   Unknown
 */
typedef struct {
    int id;
    int user_id;
    int bike_id;
    char origin[MAX_STRING];
    char destination[MAX_STRING];
    t_time pickup_time;
    t_time return_time;
    double distance;
    int status;
} t_loan;

/** @struct t_bike_history
 *  @brief  Holds loan bike history info
 *
 *  @var    t_bike_history::id
 *  Issue ID
 *
 *  @var    t_bike_history::bike_id
 *  Bike ID
 *
 *  @var    t_bike_history::description
 *  Issue description
 */
typedef struct {
    int id;
    int bike_id;
    char description[MAX_STRING];
    t_time time;
} t_bike_history;

/** @struct t_campus
 *  @brief  Represents a campus and its coordenates
 *
 *  @var    t_campus::id
 *  Campus ID
 *
 *  @var    t_campus::x
 *  X coordenate value
 *
 *  @var    t_campus::y
 *  Y coordenate value
 *
 *  @var    t_campus::z
 *  Z coordenate value
 */
typedef struct {
    char id[MAX_STRING];
    double x, y, z;
} t_campus;

#endif /* STRUCTS_H_INCLUDED */
