/**
 * @file proxy_cartographer.h
 * @author Adrien LE ROUX (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2021-12-16
 * 
 * @copyright Copyright (c) 2021
 * 
 */

/**
 * @brief Ask Cartographer to send the new gpas positions which surround the car.
 * 
 */
extern void proxyCartographer_ask4data();

/**
 * @brief Ask cartographer to start the scan and the calibration
 * 
 */
extern void proxyCartographer_signal_start();

/**
 * @brief Ask Cartographer to stop the scan.
 * 
 */
extern void proxyCartographer_signal_stop();
