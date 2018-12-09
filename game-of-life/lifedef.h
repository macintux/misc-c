#define MAXROW 25
#define MAXCOL 80
#define LIVE_CELL "•"
/* Microseconds */
#define SLEEP 500000

typedef enum status_tag {DEAD, ALIVE} Status_type;
typedef Status_type Grid_type[MAXROW][MAXCOL];
