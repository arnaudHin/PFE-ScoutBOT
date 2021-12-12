/**
 * @file map_viewer.c
 * @author Adrien LE ROUX (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2021-12-01
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include "map_viewer.h"
#include "../ComJumpC/commun.h"

static Lidar_data_t lidarData;
static Coordonnees robotPosition;

/////////////////////////////////////////////////
///                                           ///
///                PROTOTYPES                 ///                  
///                                           ///
/////////////////////////////////////////////////

/**
 * @brief 
 * 
 */
static void refresh_data();

/**
 * @brief 
 * 
 */
static void adapt_map();
/////////////////////////////////////////////////
///                                           ///
///              PUBLIC METHODS               ///                  
///                                           ///
/////////////////////////////////////////////////

extern void map_viewer_draw_map_static() {

}


extern void map_viewer_draw_map_dynamic() {

}


/////////////////////////////////////////////////
///                                           ///
///              PRIVATE METHODS              ///                  
///                                           ///
/////////////////////////////////////////////////