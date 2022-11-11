/**
 * @file GUI.h
 *
 */


#ifndef GUI_H
#define GUI_H

/*********************
 *      INCLUDES
 *********************/
#if defined(LV_LVGL_H_INCLUDE_SIMPLE)
#include "lvgl.h"
#else
#include "lvgl/lvgl.h"
#endif

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 * GLOBAL PROTOTYPES
 **********************/
void Menu_Init(void);
void Change_Screen(void);

/**********************
 *      MACROS
 **********************/



#endif /*GUI_H*/
