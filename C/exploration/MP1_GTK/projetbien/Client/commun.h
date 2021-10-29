/**
 * @file commun.h
 * @author your name (you@domain.com)
 * @brief commun data file that we need to use in different files
 * @version 0.1
 * @date 2021-10-06
 * 
 * @copyright Copyright (c) 2021
 * 
 */

typedef enum
{
    PLAY = 0,
    PAUSE
} RemoteState;

/**
 * @brief Is the remote connected to the car ?
 * 
 */
typedef enum
{
    CONNECTED = 0,
    UNCONNECTED
} Connexion;

/**
 * @brief Car direction
 * 
 */
typedef enum
{
    LEFT = 0,
    RIGHT,
    FORWARD,
    MOVEBACK
} Direction;

typedef struct
{
    Direction dir;
} Data;