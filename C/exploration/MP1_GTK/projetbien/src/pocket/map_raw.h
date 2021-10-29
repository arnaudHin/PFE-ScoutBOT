/**
 * @file  map_raw.h
 * @author Szymanski Erwan
 * @version 2.0
 * @date 8/06/2020
 */

#ifndef SRC_POCKET_MAP_RAW_H_
#define SRC_POCKET_MAP_RAW_H_

#include <stdint.h>

#define MAX_CORNER 30
#define MAP_RATIO 40

typedef struct
{
	uint16_t x;
	uint16_t y;
}Coord;


typedef struct {

    int nb_corner;
    Coord * coord;
} Obstacle;

/**
 * \fn extern void map_raw_new()
 * \brief create the map assiocated to the constant information that we have in static and constant (height,width,obstacles'corners)
 * 
 */
extern void map_raw_new();
/**
 * \fn extern char* map_get_matrix()
 * \brief getter on the matrix of the map
 * \return char* return the matrix_map adress
 */
extern char* map_get_matrix();

/**
 * \fn extern Obstacle * map_raw_list_obstacles()
 * \brief getter on the list of obstacles
 * \return Obstacle* return the adress of the list of obstacles
 */
extern Obstacle * map_raw_list_obstacles();

/**
 * \fn extern void map_raw_free()
 * \brief free obstacles in memory
 */
extern void map_raw_free();

/**
 * \fn extern int map_get_nb_obstacles()
 * \brief getter on NB_OBSTACLES
 * \return int return number of obstacles on the map
 */
extern int map_get_nb_obstacles();

/**
 * \fn extern int map_raw_get_height()
 * \brief getter on HEIGHT
 * \return int return the height of the map 
 */
extern int map_raw_get_height();

/**
 * \fn extern int map_raw_get_width()
 * \brief getter on WIDTH
 * \return int return the width of the map 
 */
extern int map_raw_get_width();

#endif
