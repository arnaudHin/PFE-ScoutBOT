/**
 * @file main_screen.h
 * @author Adrien LE ROUX (adrien.leroux@reseau.eseo.fr)
 * @brief 
 * @version 0.1
 * @date 2021-12-15
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <gtk/gtk.h>
#include "../ComJumpC/commun.h"
#include <stdbool.h>
typedef struct
{
    int x;
    int y;
    int next_x;
    int next_y;
} Point;

typedef enum
{
    STATIC = 0,
    DYNAMIC
} Mode_A;

typedef enum
{
    NONE_S = 0,
    STARTED_IN_PROGRESS,
    STARTED
} Static_Status;

/**
 * @brief 
 * 
 * @return GtkBuilder* 
 */
extern GtkBuilder *mainScreen_getBuilder();

/**
 * @brief 
 * 
 * @return GtkWidget** 
 */
extern GtkWidget **mainScreen_getWindows();

/**
 * @brief 
 * 
 */
extern void mainScreen_new();

/**
 * @brief 
 * 
 * @param data 
 */
extern void mainScreen_draw_static_refresh(DATA_from_pocket_t data);

/**
 * @brief 
 * 
 * @param data 
 */
extern void mainScreen_draw_dynamic_refresh(Lidar_data_t data);

/**
 * @brief Set the Static Started object
 * 
 * @param started 
 */
extern void setStaticStarted(Static_Status started);

/**
 * @brief Get the Static Started object
 * 
 * @return Static_Status 
 */
extern Static_Status getStaticStarted();


