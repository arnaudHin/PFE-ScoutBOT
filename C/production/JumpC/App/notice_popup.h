/**
 * @file notice_popup.h
 * @author Adrien LE ROUX (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2021-12-17
 * 
 * @copyright Copyright (c) 2021
 * 
 */

typedef enum
{
    CAL_SUCCESS = 0,
    CAL_UNSUCCESS,
    CON_SUCCESS,
    CON_UNSUCCESS
} Choice;

/**
 * @brief display the popup screen
 * 
 */
extern void noticePopup_new(Choice choice);
