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
#include "commun.h"

typedef struct{
   uint8_t x_robot;
   uint8_t y_robot; 
}Coordonnees;


/**
 * @brief aloow to memorize the new positions values and display them in the main screen
 * 
 * @param data 
 */
extern void mapViewer_setData(DATA_from_pocket_t data);

/**
 * @brief Function able to initialize the static variable to false
 * 
 */
extern void mapViewer_calibrationSuccessful();

/**
 * @brief allow to prevent a new display of a new static map with lidar values
 * 
 */
extern void mapViewer_free();