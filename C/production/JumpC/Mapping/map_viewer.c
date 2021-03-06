/**
 * @file mapViewer.c
 * @author Adrien LE ROUX (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2021-12-01
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include <stdbool.h>
#include <stdlib.h>
#include "map_viewer.h"
#include "commun.h"
#include "util.h"
#include "../App/main_screen.h"
#include "../App/notice_popup.h"
#include "stdio.h"

static DATA_from_pocket_t dataPos;
static uint8_t mapStatic = 0;
static uint16_t count = 0;
static Mode_Zoom mode_zoom = NONE_Z;
/////////////////////////////////////////////////
///                                           ///
///                PROTOTYPES                 ///
///                                           ///
/////////////////////////////////////////////////

/**
 * @brief draw the static map on the UI
 * 
 */
static void mapViewer_draw_map_static(void);

/**
 * @brief draw the dynamic map on the UI
 * 
 */
static void mapViewer_draw_map_dynamic(void);

/**
 * @brief Fonction able to order 1 element at the top of the binary tree
 * 
 * @param arr 
 * @param size 
 * @param i 
 */
static void heapify(int16_t arr[], int16_t size, int16_t i);

/**
 * @brief allow to permute 2 elements in a binary tree
 * 
 * @param a 
 * @param b 
 */
static void swap(int16_t *a, int16_t *b);

/**
 * @brief sort arrays elements in an increasing order. 
 * 
 * @param arr array to sort
 * @param n size of the array
 */
static void heapSort(int16_t arr[], int16_t n);

/////////////////////////////////////////////////
///                                           ///
///              PUBLIC METHODS               ///
///                                           ///
/////////////////////////////////////////////////

extern void mapViewer_setData(DATA_from_pocket_t data)
{
    Room_e room = NO_ROOM;
    dataPos = data;
    count = 0;
    room = dataPos.positionData.room;
    // ZOOM Mode
    // heapSort(data.lidarData.X_buffer, LIDAR_TOTAL_DEGREE);
    // heapSort(data.lidarData.Y_buffer, LIDAR_TOTAL_DEGREE);
    // uint8_t max_x = 0;
    // uint8_t max_y = 0;
    // if (dataPos.lidarData.X_buffer[0] != 0)
    // {
    //     max_x = abs(data.lidarData.X_buffer[0]) / 380;
    // }
    // else
    // {
    //     max_x = 190;
    // }
    // if (dataPos.lidarData.Y_buffer[0] != 0)
    // {
    //     max_y = abs(data.lidarData.Y_buffer[0]) / 280;
    // }
    // else
    // {
    //     max_y = 140;
    // }
    // for (size_t i = 0; i < LIDAR_TOTAL_DEGREE; i++)
    // {
    //     dataPos.lidarData.X_buffer[i] += abs(data.lidarData.X_buffer[0]);
    //     dataPos.lidarData.Y_buffer[i] += abs(data.lidarData.Y_buffer[0]);
    //     if (dataPos.lidarData.X_buffer[i] != 0)
    //     {
    //         dataPos.lidarData.X_buffer[i] = (uint16_t)(dataPos.lidarData.X_buffer[i] / max_x);
    //     }
    //     else
    //     {
    //         dataPos.lidarData.X_buffer[i] = 240;
    //     }
    //     if (dataPos.lidarData.Y_buffer[i] != 0)
    //     {
    //         dataPos.lidarData.Y_buffer[i] = (uint16_t)(dataPos.lidarData.Y_buffer[i] / max_y);
    //     }
    //     else
    //     {
    //         dataPos.lidarData.Y_buffer[i] = 143;
    //     }
    // }
    float widthRoomCoef = 0.0;
    float heightRoomCoef = 0.0;
    switch (mode_zoom)
    {
    case ZOOM:
        heapSort(data.lidarData.X_buffer, LIDAR_TOTAL_DEGREE);
        heapSort(data.lidarData.Y_buffer, LIDAR_TOTAL_DEGREE);
        uint8_t max_x = 0;
        uint8_t max_y = 0;
        if (dataPos.lidarData.X_buffer[0] != 0)
        {
            max_x = abs(data.lidarData.X_buffer[0]) > abs(data.lidarData.X_buffer[359]) ? (abs(data.lidarData.X_buffer[0]) / 380) : (abs(data.lidarData.X_buffer[359]) / 480);
        }
        else
        {
            max_x = 240;
        }
        if (dataPos.lidarData.Y_buffer[0] != 0)
        {
            max_y = abs(data.lidarData.Y_buffer[0]) > abs(data.lidarData.Y_buffer[359]) ? (abs(data.lidarData.Y_buffer[0]) / 280) : (abs(data.lidarData.Y_buffer[359]) / 283);
        }
        else
        {
            max_y = 143;
        }
        for (size_t i = 0; i < LIDAR_TOTAL_DEGREE; i++)
        {
            dataPos.lidarData.X_buffer[i] += abs(data.lidarData.X_buffer[0]);
            dataPos.lidarData.Y_buffer[i] += abs(data.lidarData.Y_buffer[0]);
            if (dataPos.lidarData.X_buffer[i] != 0)
            {
                dataPos.lidarData.X_buffer[i] = (uint16_t)(dataPos.lidarData.X_buffer[i] / max_x);
            }
            else
            {
                dataPos.lidarData.X_buffer[i] = 240;
            }
            if (dataPos.lidarData.Y_buffer[i] != 0)
            {
                dataPos.lidarData.Y_buffer[i] = (uint16_t)(dataPos.lidarData.Y_buffer[i] / max_y);
            }
            else
            {
                dataPos.lidarData.Y_buffer[i] = 143;
            }
        }
        break;
    case NONE_Z:

        switch (room)
        {
        case ROOM_A:
            heightRoomCoef = 6000 / 286;
            widthRoomCoef = 6500 / 480;
            break;
        case ROOM_B:
            heightRoomCoef = 6000 / 286;
            widthRoomCoef = 5500 / 480;
            break;
        default:
            heightRoomCoef = 6000 / 286;
            widthRoomCoef = 6500 / 480;
            break;
        }
        for (size_t i = 0; i < LIDAR_TOTAL_DEGREE; i++)
        {
            dataPos.lidarData.X_buffer[i] += 6000;
            dataPos.lidarData.Y_buffer[i] += 6000;
            if (dataPos.lidarData.X_buffer[i] != 6000)
            {
                dataPos.lidarData.X_buffer[i] = (uint16_t)(dataPos.lidarData.X_buffer[i] / widthRoomCoef);
            }
            else
            {
                dataPos.lidarData.X_buffer[i] = 240;
                count += 1;
            }
            if (dataPos.lidarData.Y_buffer[i] != 6000)
            {
                dataPos.lidarData.Y_buffer[i] = (uint16_t)(dataPos.lidarData.Y_buffer[i] / heightRoomCoef);
            }
            else
            {
                dataPos.lidarData.Y_buffer[i] = 143;
                count += 1;
            }
        }

        dataPos.positionData.x = (uint16_t)(dataPos.positionData.x / widthRoomCoef);
        dataPos.positionData.y = (uint16_t)(dataPos.positionData.y / heightRoomCoef);
        break;
    default:
        break;
    }

    // switch(dataPos.positionData.room){
    //     case ROOM_A :
    //     dataPos.positionData.x =
    //     break;
    //     case ROOM_B :

    //     break;

    //     default :
    //     break;
    // }
    // dataPos.positionData.x
    switch (mapStatic)
    {
    case 1:
        noticePopup_new(CAL_SUCCESS);
        mapStatic++;
        break;
    case 0:
        noticePopup_new(CAL_UNSUCCESS);
        break;
    default:
        break;
    }

    mapViewer_draw_map_static();
    mapViewer_draw_map_dynamic();
}

extern void mapViewer_calibrationSuccessful()
{
    mapStatic = 1;
}

extern void mapViewer_free()
{
    mapStatic = 0;
}

extern void mapViewer_setZoomDynamic(Mode_Zoom pmode_zoom)
{
    mode_zoom = pmode_zoom;
}

/////////////////////////////////////////////////
///                                           ///
///              PRIVATE METHODS              ///
///                                           ///
/////////////////////////////////////////////////

static void mapViewer_draw_map_static()
{
    if (NONE_S == getStaticStarted() && 500 > count)
    {
        setStaticStarted(STARTED_IN_PROGRESS);
    }
    mainScreen_draw_static_refresh(dataPos);
}

static void mapViewer_draw_map_dynamic()
{
    mainScreen_draw_dynamic_refresh(dataPos.lidarData);
}

static void heapify(int16_t arr[], int16_t size, int16_t i)
{
    int16_t largest = i;
    int16_t l = 2 * i + 1;
    int16_t r = 2 * i + 2;
    if (l < size && arr[l] > arr[largest])
        largest = l;
    if (r < size && arr[r] > arr[largest])
        largest = r;
    if (largest != i)
    {
        swap(&arr[i], &arr[largest]);
        heapify(arr, size, largest);
    }
}

static void heapSort(int16_t arr[], int16_t n)
{
    for (int16_t i = n / 2 - 1; i >= 0; i--)
    {
        heapify(arr, n, i);
    }

    for (int16_t i = n - 1; i >= 0; i--)
    {
        swap(&arr[0], &arr[i]);

        heapify(arr, i, 0);
    }
}

static void swap(int16_t *a, int16_t *b)
{
    int16_t temp = *b;
    *b = *a;
    *a = temp;
}
