/**
 * @file map_viewer.h
 * @author Adrien LE ROUX (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2021-12-01
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <stdio.h>
#include <stdint.h>

typedef struct{
   uint8_t x_robot;
   uint8_t y_robot; 
}Coordonnees;
/**
 * @brief draw the static map on the UI
 * 
 */
extern void map_viewer_draw_map_static();


/**
 * @brief draw the dynamic map on the UI
 * 
 */
extern void map_viewer_draw_map_dynamic();