/**
 * @file  map_raw.c
 * @author Szymanski Erwan & Montet Julien & Marbeuf Pierre
 * @version 2.1
 * @date 8/06/2020
 */

#include "map_raw.h"
#include <stdio.h>
#include <stdlib.h>
#include "../util.h"

#define NB_OBSTACLES 0   //to add a rectangular object, always begin by the bottom right corner in the trigonometric sense

#define WIDTH     46
#define HEIGHT    35


static Obstacle list_Obstacles[NB_OBSTACLES];
static char matrix_map[WIDTH][HEIGHT] = {0};
static void create_matrix();

/**
 * \fn extern void map_raw_new()
 * \brief create the map assiocated to the constant information that we have in static and constant (height,width,obstacles'corners)
 * 
 */
extern void map_raw_new(){

    // int a;
    // int b;

    // for(a=0;a<WIDTH;a++){
    //     for(b=0;b<HEIGHT;b++){
    //         matrix_map[a][b] = '0';
    //     }
    // }

    // int height = HEIGHT;
    // int width = WIDTH;

    // //------------------Obstacle 0

    // Obstacle obstacle_0;
    // obstacle_0.nb_corner = 4;
    // obstacle_0.coord = malloc (obstacle_0.nb_corner*sizeof(Coord));
    // obstacle_0.coord[0].x = 0;
    // obstacle_0.coord[0].y = 14;

    // obstacle_0.coord[1].x = 13;
    // obstacle_0.coord[1].y = 14;

    // obstacle_0.coord[2].x = 13;
    // obstacle_0.coord[2].y = HEIGHT;

    // obstacle_0.coord[3].x = 0;
	// obstacle_0.coord[3].y = HEIGHT;



	// //------------------Obstacle 1

	// Obstacle obstacle_1;
	// obstacle_1.nb_corner = 4;
	// obstacle_1.coord = malloc (obstacle_1.nb_corner*sizeof(Coord));
	// obstacle_1.coord[0].x = 26;
	// obstacle_1.coord[0].y = 0;

	// obstacle_1.coord[1].x = 36;
	// obstacle_1.coord[1].y = 0;

	// obstacle_1.coord[2].x = 36;
	// obstacle_1.coord[2].y = 22;

	// obstacle_1.coord[3].x = 26;
	// obstacle_1.coord[3].y = 22;


    // list_Obstacles[0] = obstacle_0;
    // list_Obstacles[1] = obstacle_1;

    // create_matrix();
}




/**
 * \fn static void create_matrix()
 * \brief create the matrix of the map (only work with rectangular obstacle perfectly straight) 
 */
static void create_matrix(){

//     TRACE("create matrix\n");
//     int i = 0;
//     int j = 0;
//     int k = 0;

//    for (i=0 ; i< WIDTH ; i++){
//         matrix_map[i][HEIGHT - 1] = '1';
//         matrix_map[i][0] = '1';
//     }

//      for (i=0 ; i< HEIGHT ; i++){
//         matrix_map[0][i] = '1';
//         matrix_map[WIDTH - 1][i] = '1';
//     }

//     //obstacles
//     for (k = 0;k<NB_OBSTACLES;k++){
//         for(i=list_Obstacles[k].coord[0].x; i<list_Obstacles[k].coord[2].x; i++){
//             for(j=list_Obstacles[k].coord[0].y ; j<list_Obstacles[k].coord[2].y; j++){
//                 matrix_map[i][j] = '1';
//             }
//         }
//     }
}

/**
 * \fn extern void map_raw_free()
 * \brief free obstacles in memory
 */
extern void map_raw_free(){
	// for (int i = 0; i<NB_OBSTACLES; i++){
	// 	free(list_Obstacles[i].coord);
	// }
	// TRACE("Map free");
}

/**
 * \fn extern int map_raw_get_width()
 * \brief getter on WIDTH
 * \return int return the width of the map 
 */
extern int map_raw_get_width(){
	return (int)WIDTH;
}

/**
 * \fn extern int map_raw_get_height()
 * \brief getter on HEIGHT
 * \return int return the height of the map 
 */
extern int map_raw_get_height(){
	return (int)HEIGHT;
}

/**
 * \fn extern int map_get_nb_obstacles()
 * \brief getter on NB_OBSTACLES
 * \return int return number of obstacles on the map
 */
extern int map_get_nb_obstacles(){
	return NB_OBSTACLES;
}

/**
 * \fn extern char* map_get_matrix()
 * \brief getter on the matrix of the map
 * \return char* return the matrix_map adress
 */
extern char* map_get_matrix(){
    return NULL /*(char*)matrix_map*/;
}

/**
 * \fn extern Obstacle * map_raw_list_obstacles()
 * \brief getter on the list of obstacles
 * \return Obstacle* return the adress of the list of obstacles
 */
extern Obstacle * map_raw_list_obstacles(){
    return NULL/*list_Obstacles*/;
}
