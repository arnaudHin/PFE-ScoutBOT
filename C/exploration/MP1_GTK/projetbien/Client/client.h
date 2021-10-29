/**
 * @file client.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2021-10-06
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include "commun.h"

/**
 * @brief 
 * 
 * @return int 
 */
extern int client_new();

/**
 * @brief 
 * 
 * @return int 
 */
extern int client_start();

/**
 * @brief 
 * 
 */
extern void client_stop();

/**
 * @brief 
 * 
 * @param direction 
 */
extern void client_run_write(Direction direction);

/**
 * @brief 
 * 
 */
extern void client_run_read();

