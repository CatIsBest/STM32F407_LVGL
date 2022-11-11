/**
 * @file 		GUI.c
 * @author 	日常的阪本@csdn
 * @brief   通过slider组件控制RGB LED色彩
 */


/*********************
 *      INCLUDES
 *********************/
#include "GUI.h"

#include "BSP\tim.h"
/*********************
 *      DEFINES
 *********************/


 

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void APP1_Screen_Init(void);
static void APP2_Screen_Init(void);
 
static void slider_r_event_cb(lv_event_t * e);
static void slider_g_event_cb(lv_event_t * e);
static void slider_b_event_cb(lv_event_t * e);
static void chang_to_menu(lv_event_t * e);
static void chang_to_app1(lv_event_t * e);
static void chang_to_app2(lv_event_t * e);
static void app2_event_cb(lv_event_t * e);
static void close_rgb(lv_event_t * e);

//要添加APP界面的话在这里加屏幕比如 SCREEN_APP3
typedef enum
{
	SCREEN_MENU,
	SCREEN_APP1,
	SCREEN_APP2,
}Screen_Type;

struct{
	Screen_Type current_screen;
	Screen_Type next_screen;
}screen_state;

//菜单屏幕
static lv_obj_t * menu_scr;
//APP1屏幕
static lv_obj_t * app1_scr;
//APP2屏幕
static lv_obj_t * app2_scr;
////APP3屏幕
//static lv_obj_t * app3_scr;
//...
/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

/**
  * @brief  初始化菜单界面（桌面）
  */
void Menu_Init(void)
{
	lv_obj_t * menu_scr = lv_obj_create(NULL);
	lv_scr_load(menu_scr); 
	
	//设置背景
	lv_obj_t * bg_top;
	LV_IMG_DECLARE(nahida);
	bg_top = lv_img_create(lv_scr_act());
	lv_img_set_src(bg_top, &nahida );
	
	//创建APP1图标
	lv_obj_t * imgbtn1 = lv_imgbtn_create(lv_scr_act());
	lv_obj_set_size(imgbtn1,90,90);
	lv_obj_set_pos(imgbtn1,30,30);
	LV_IMG_DECLARE(icon_normal);
	LV_IMG_DECLARE(icon_press);
	lv_imgbtn_set_src(imgbtn1, LV_IMGBTN_STATE_RELEASED, NULL, &icon_normal, NULL);
	lv_imgbtn_set_src(imgbtn1, LV_IMGBTN_STATE_PRESSED, NULL, &icon_press, NULL);
	lv_obj_add_event_cb(imgbtn1, chang_to_app1, LV_EVENT_CLICKED , NULL);
	
	
	//创建APP2图标
	lv_obj_t * imgbtn2 = lv_imgbtn_create(lv_scr_act());
	lv_obj_set_size(imgbtn2,90,90);
	lv_obj_set_pos(imgbtn2,160,30);
	LV_IMG_DECLARE(icon2_normal);
	LV_IMG_DECLARE(icon2_press);
	lv_imgbtn_set_src(imgbtn2, LV_IMGBTN_STATE_RELEASED, NULL, &icon2_normal, NULL);
	lv_imgbtn_set_src(imgbtn2, LV_IMGBTN_STATE_PRESSED, NULL, &icon2_press, NULL);
	lv_obj_add_event_cb(imgbtn2, chang_to_app2, LV_EVENT_CLICKED , NULL);
}


/**
  * @brief  切换界面
  */
void Change_Screen(void)
{
	if(screen_state.next_screen != screen_state.current_screen)
	{
		//加载新屏幕
		if(screen_state.next_screen == SCREEN_MENU)
		{
			Menu_Init();
		}
		else if(screen_state.next_screen == SCREEN_APP1)
		{
			APP1_Screen_Init();			
		}
		else if(screen_state.next_screen == SCREEN_APP2)
		{
			APP2_Screen_Init();			
		}
		
		//删除原来的屏幕
		if(screen_state.current_screen == SCREEN_MENU)
		{
			lv_obj_del(menu_scr);
		}		
		else if(screen_state.current_screen == SCREEN_APP1)
		{
			lv_obj_del(app1_scr);
		}
		else if(screen_state.current_screen == SCREEN_APP2)
		{
			lv_obj_del(app2_scr);
		}
		
		screen_state.current_screen = screen_state.next_screen;
	}
}


/**
  * @brief  初始化APP1界面（RGB控制器）
  */
static void APP1_Screen_Init(void)
{
		lv_obj_t * app1_scr = lv_obj_create(NULL);
		lv_scr_load(app1_scr); 
	
		/*RED LED*/
	  /*Create a slider of red LED*/
    lv_obj_t * slider_r = lv_slider_create(lv_scr_act());
    lv_obj_set_pos(slider_r,30,200);
    lv_obj_set_style_bg_color(slider_r,lv_palette_main(LV_PALETTE_RED),LV_PART_INDICATOR);
		lv_obj_set_style_bg_color(slider_r,lv_palette_main(LV_PALETTE_RED),LV_PART_KNOB);

    /*Create a label below the slider*/
		lv_obj_t * slider_label_r;
    slider_label_r = lv_label_create(lv_scr_act());
    lv_label_set_text(slider_label_r, "0%");
    lv_obj_align_to(slider_label_r, slider_r, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);
	
    lv_obj_add_event_cb(slider_r, slider_r_event_cb, LV_EVENT_VALUE_CHANGED, slider_label_r);	
	
		
		/*GREEN LED*/
	  /*Create a slider of green LED*/
    lv_obj_t * slider_g = lv_slider_create(lv_scr_act());
    lv_obj_set_pos(slider_g,30,400);
    lv_obj_set_style_bg_color(slider_g,lv_palette_main(LV_PALETTE_GREEN),LV_PART_INDICATOR);
		lv_obj_set_style_bg_color(slider_g,lv_palette_main(LV_PALETTE_GREEN),LV_PART_KNOB);

    /*Create a label below the slider*/
		lv_obj_t * slider_label_g;
    slider_label_g = lv_label_create(lv_scr_act());
    lv_label_set_text(slider_label_g, "0%");
    lv_obj_align_to(slider_label_g, slider_g, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);
	
    lv_obj_add_event_cb(slider_g, slider_g_event_cb, LV_EVENT_VALUE_CHANGED, slider_label_g);	
		
		
		/*BLUE LED*/
	  /*Create a slider of blue LED*/
    lv_obj_t * slider_b = lv_slider_create(lv_scr_act());
    lv_obj_set_pos(slider_b,30,600);
    lv_obj_set_style_bg_color(slider_b,lv_palette_main(LV_PALETTE_BLUE),LV_PART_INDICATOR);
		lv_obj_set_style_bg_color(slider_b,lv_palette_main(LV_PALETTE_BLUE),LV_PART_KNOB);

    /*Create a label below the slider*/
		lv_obj_t * slider_label_b;
    slider_label_b = lv_label_create(lv_scr_act());
    lv_label_set_text(slider_label_b, "0%");
    lv_obj_align_to(slider_label_b, slider_b, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);
	
    lv_obj_add_event_cb(slider_b, slider_b_event_cb, LV_EVENT_VALUE_CHANGED, slider_label_b);	
		
		/*Create a Title*/
		lv_obj_t * title_label;
    title_label = lv_label_create(lv_scr_act());
    lv_label_set_text(title_label, "RGB Controller");
    lv_obj_align_to(title_label, slider_r,LV_ALIGN_OUT_TOP_MID,0,-20);
		
		//创建退出按钮
		lv_obj_t * btn_esc = lv_btn_create(lv_scr_act());
		lv_obj_add_event_cb(btn_esc, chang_to_menu, LV_EVENT_CLICKED, NULL);
		lv_obj_add_event_cb(btn_esc, close_rgb, LV_EVENT_CLICKED, NULL);
		lv_obj_set_pos(btn_esc,10,10);
		
		lv_obj_t * label = lv_label_create(btn_esc);
    lv_label_set_text(label, "Esc");
    lv_obj_center(label);
}

/**
  * @brief  初始化APP2界面
  */
static void APP2_Screen_Init(void)
{
	lv_obj_t * app2_scr = lv_obj_create(NULL);
	lv_scr_load(app2_scr); 
	
	lv_obj_t * btn = lv_btn_create(lv_scr_act());
	lv_obj_set_size(btn, 100, 50);
	lv_obj_center(btn);
	lv_obj_add_event_cb(btn, app2_event_cb, LV_EVENT_CLICKED, NULL);

	lv_obj_t * num_label = lv_label_create(btn);
	lv_label_set_text(num_label, "Click me!");
	lv_obj_center(num_label);	
	
	//创建退出按钮
	lv_obj_t * btn_esc = lv_btn_create(lv_scr_act());
	lv_obj_add_event_cb(btn_esc, chang_to_menu, LV_EVENT_CLICKED, NULL);
	lv_obj_set_pos(btn_esc,10,10);
	
	lv_obj_t * label = lv_label_create(btn_esc);
	lv_label_set_text(label, "Esc");
	lv_obj_center(label);
}


static void slider_event_cb(lv_event_t * e)
{
    lv_obj_t * slider = lv_event_get_target(e);
		lv_obj_t * slider_label = lv_event_get_user_data(e);
    char buf[8];
    lv_snprintf(buf, sizeof(buf), "%d%%", (int)lv_slider_get_value(slider));
    lv_label_set_text(slider_label, buf);
    lv_obj_align_to(slider_label, slider, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);
}

static void slider_r_event_cb(lv_event_t * e)
{
		slider_event_cb(e);
		lv_obj_t * slider = lv_event_get_target(e);
		TIM10->CCR1 = lv_slider_get_value(slider) * 25;
}

static void slider_g_event_cb(lv_event_t * e)
{
		slider_event_cb(e);
		lv_obj_t * slider = lv_event_get_target(e);
		TIM11->CCR1 = lv_slider_get_value(slider) * 25;		
}
static void slider_b_event_cb(lv_event_t * e)
{
		slider_event_cb(e);
		lv_obj_t * slider = lv_event_get_target(e);
		TIM13->CCR1 = lv_slider_get_value(slider) * 25;		
}

static void chang_to_app1(lv_event_t * e)
{
		screen_state.next_screen = SCREEN_APP1;
}

static void chang_to_app2(lv_event_t * e)
{
		screen_state.next_screen = SCREEN_APP2;
}

static void chang_to_menu(lv_event_t * e)
{
		screen_state.next_screen = SCREEN_MENU;
}

static void app2_event_cb(lv_event_t * e)
{
    static uint32_t cnt = 1;
    lv_obj_t * btn = lv_event_get_target(e);
    lv_obj_t * label = lv_obj_get_child(btn, 0);
    lv_label_set_text_fmt(label, "%"LV_PRIu32, cnt);
    cnt++;
}

static void close_rgb(lv_event_t * e)
{
	TIM10->CCR1 = 0;
	TIM11->CCR1 = 0;
	TIM13->CCR1 = 0;
}
