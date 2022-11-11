/**
  ******************************************************************************
  * @file    gt5xx.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   i2cµçÈİÆÁÇı¶¯º¯Êıgt9157Ğ¾Æ¬
  ******************************************************************************
  * @attention
  *
  * ÊµÑéÆ½Ì¨:Ò°»ğ  STM32 F407 ¿ª·¢°å 
  * ÂÛÌ³    :http://www.firebbs.cn
  * ÌÔ±¦    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */ 
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "./touch/gt5xx.h"
#include "./touch/bsp_i2c_touch.h"
#include "./lcd/bsp_nt35510_lcd.h"



volatile int16_t touchpad_x = 0;
volatile int16_t touchpad_y = 0;
volatile int16_t touchpad_is_down = 0;

int16_t Get_Touchpad_x(void)
{
	return touchpad_x;
}

int16_t Get_Touchpad_y(void)
{
	return touchpad_y;
}

int16_t Get_Touchpad_Is_Down(void)
{
	return touchpad_is_down;
}


// 4.5´çÆÁGT5688Çı¶¯ÅäÖÃ
const uint8_t CTP_CFG_GT5688[] =  {
			0x96,0xE0,0x01,0x56,0x03,0x05,0x35,0x00,0x01,0x00,
			0x00,0x05,0x50,0x3C,0x53,0x11,0x00,0x00,0x22,0x22,
			0x14,0x18,0x1A,0x1D,0x0A,0x04,0x00,0x00,0x00,0x00,
			0x00,0x00,0x53,0x00,0x14,0x00,0x00,0x84,0x00,0x00,
			0x3C,0x19,0x19,0x64,0x1E,0x28,0x88,0x29,0x0A,0x2D,
			0x2F,0x29,0x0C,0x20,0x33,0x60,0x13,0x02,0x24,0x00,
			0x00,0x20,0x3C,0xC0,0x14,0x02,0x00,0x00,0x54,0xAC,
			0x24,0x9C,0x29,0x8C,0x2D,0x80,0x32,0x77,0x37,0x6E,
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF0,0x50,0x3C,
			0xFF,0xFF,0x07,0x00,0x00,0x00,0x02,0x14,0x14,0x03,
			0x04,0x00,0x21,0x64,0x0A,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			0x32,0x20,0x50,0x3C,0x3C,0x00,0x00,0x00,0x00,0x00,
			0x0D,0x06,0x0C,0x05,0x0B,0x04,0x0A,0x03,0x09,0x02,
			0xFF,0xFF,0xFF,0xFF,0x00,0x01,0x02,0x03,0x04,0x05,
			0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,
			0x10,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x3C,0x00,0x05,0x1E,0x00,0x02,
			0x2A,0x1E,0x19,0x14,0x02,0x00,0x03,0x0A,0x05,0x00,
			0x00,0x00,0x00,0x00,0x00,0x00,0x01,0xFF,0xFF,0x86,
			0x22,0x03,0x00,0x00,0x33,0x00,0x0F,0x00,0x00,0x00,
			0x50,0x3C,0x50,0x00,0x00,0x00,0x1A,0x64,0x01

};


//uint8_t config[GTP_CONFIG_MAX_LENGTH + GTP_ADDR_LENGTH]
//                = {GTP_REG_CONFIG_DATA >> 8, GTP_REG_CONFIG_DATA & 0xff};

TOUCH_IC touchIC;								

static int8_t GTP_I2C_Test(void);

static void Delay(__IO uint32_t nCount)	 //¼òµ¥µÄÑÓÊ±º¯Êı
{
	for(; nCount != 0; nCount--);
}


/**
  * @brief   Ê¹ÓÃIIC½øĞĞÊı¾İ´«Êä
  * @param
  *		@arg i2c_msg:Êı¾İ´«Êä½á¹¹Ìå
  *		@arg num:Êı¾İ´«Êä½á¹¹ÌåµÄ¸öÊı
  * @retval  Õı³£Íê³ÉµÄ´«Êä½á¹¹¸öÊı£¬Èô²»Õı³££¬·µ»Ø0xff
  */
static int I2C_Transfer( struct i2c_msg *msgs,int num)
{
	int im = 0;
	int ret = 0;

	GTP_DEBUG_FUNC();

	for (im = 0; ret == 0 && im != num; im++)
	{
		if ((msgs[im].flags&I2C_M_RD))																//¸ù¾İflagÅĞ¶ÏÊÇ¶ÁÊı¾İ»¹ÊÇĞ´Êı¾İ
		{
			ret = I2C_ReadBytes(msgs[im].addr, msgs[im].buf, msgs[im].len);		//IIC¶ÁÈ¡Êı¾İ
		} else
		{
			ret = I2C_WriteBytes(msgs[im].addr,  msgs[im].buf, msgs[im].len);	//IICĞ´ÈëÊı¾İ
		}
	}

	if(ret)
		return ret;

	return im;   													//Õı³£Íê³ÉµÄ´«Êä½á¹¹¸öÊı
}

/**
  * @brief   ´ÓIICÉè±¸ÖĞ¶ÁÈ¡Êı¾İ
  * @param
  *		@arg client_addr:Éè±¸µØÖ·
  *		@arg  buf[0~1]: ¶ÁÈ¡Êı¾İ¼Ä´æÆ÷µÄÆğÊ¼µØÖ·
  *		@arg buf[2~len-1]: ´æ´¢¶Á³öÀ´Êı¾İµÄ»º³åbuffer
  *		@arg len:    GTP_ADDR_LENGTH + read bytes count£¨¼Ä´æÆ÷µØÖ·³¤¶È+¶ÁÈ¡µÄÊı¾İ×Ö½ÚÊı£©
  * @retval  i2c_msgs´«Êä½á¹¹ÌåµÄ¸öÊı£¬2Îª³É¹¦£¬ÆäËüÎªÊ§°Ü
  */
static int32_t GTP_I2C_Read(uint8_t client_addr, uint8_t *buf, int32_t len)
{
    struct i2c_msg msgs[2];
    int32_t ret=-1;
    int32_t retries = 0;

    GTP_DEBUG_FUNC();
    /*Ò»¸ö¶ÁÊı¾İµÄ¹ı³Ì¿ÉÒÔ·ÖÎªÁ½¸ö´«Êä¹ı³Ì:
     * 1. IIC  Ğ´Èë Òª¶ÁÈ¡µÄ¼Ä´æÆ÷µØÖ·
     * 2. IIC  ¶ÁÈ¡  Êı¾İ
     * */

    msgs[0].flags = !I2C_M_RD;					//Ğ´Èë
    msgs[0].addr  = client_addr;					//IICÉè±¸µØÖ·
    msgs[0].len   = GTP_ADDR_LENGTH;	//¼Ä´æÆ÷µØÖ·Îª2×Ö½Ú(¼´Ğ´ÈëÁ½×Ö½ÚµÄÊı¾İ)
    msgs[0].buf   = &buf[0];						//buf[0~1]´æ´¢µÄÊÇÒª¶ÁÈ¡µÄ¼Ä´æÆ÷µØÖ·
    
    msgs[1].flags = I2C_M_RD;					//¶ÁÈ¡
    msgs[1].addr  = client_addr;					//IICÉè±¸µØÖ·
    msgs[1].len   = len - GTP_ADDR_LENGTH;	//Òª¶ÁÈ¡µÄÊı¾İ³¤¶È
    msgs[1].buf   = &buf[GTP_ADDR_LENGTH];	//buf[GTP_ADDR_LENGTH]Ö®ºóµÄ»º³åÇø´æ´¢¶Á³öµÄÊı¾İ

    while(retries < 5)
    {
        ret = I2C_Transfer( msgs, 2);					//µ÷ÓÃIICÊı¾İ´«Êä¹ı³Ìº¯Êı£¬ÓĞ2¸ö´«Êä¹ı³Ì
        if(ret == 2)break;
        retries++;
    }
    if((retries >= 5))
    {
        GTP_ERROR("I2C Read: 0x%04X, %d bytes failed, errcode: %d! Process reset.", (((uint16_t)(buf[0] << 8)) | buf[1]), len-2, ret);
    }
    return ret;
}



/**
  * @brief   ÏòIICÉè±¸Ğ´ÈëÊı¾İ
  * @param
  *		@arg client_addr:Éè±¸µØÖ·
  *		@arg  buf[0~1]: ÒªĞ´ÈëµÄÊı¾İ¼Ä´æÆ÷µÄÆğÊ¼µØÖ·
  *		@arg buf[2~len-1]: ÒªĞ´ÈëµÄÊı¾İ
  *		@arg len:    GTP_ADDR_LENGTH + write bytes count£¨¼Ä´æÆ÷µØÖ·³¤¶È+Ğ´ÈëµÄÊı¾İ×Ö½ÚÊı£©
  * @retval  i2c_msgs´«Êä½á¹¹ÌåµÄ¸öÊı£¬1Îª³É¹¦£¬ÆäËüÎªÊ§°Ü
  */
static int32_t GTP_I2C_Write(uint8_t client_addr,uint8_t *buf,int32_t len)
{
    struct i2c_msg msg;
    int32_t ret = -1;
    int32_t retries = 0;

    GTP_DEBUG_FUNC();
    /*Ò»¸öĞ´Êı¾İµÄ¹ı³ÌÖ»ĞèÒªÒ»¸ö´«Êä¹ı³Ì:
     * 1. IICÁ¬Ğø Ğ´Èë Êı¾İ¼Ä´æÆ÷µØÖ·¼°Êı¾İ
     * */
    msg.flags = !I2C_M_RD;			//Ğ´Èë
    msg.addr  = client_addr;			//´ÓÉè±¸µØÖ·
    msg.len   = len;							//³¤¶ÈÖ±½ÓµÈÓÚ(¼Ä´æÆ÷µØÖ·³¤¶È+Ğ´ÈëµÄÊı¾İ×Ö½ÚÊı)
    msg.buf   = buf;						//Ö±½ÓÁ¬ĞøĞ´Èë»º³åÇøÖĞµÄÊı¾İ(°üÀ¨ÁË¼Ä´æÆ÷µØÖ·)

    while(retries < 5)
    {
        ret = I2C_Transfer(&msg, 1);	//µ÷ÓÃIICÊı¾İ´«Êä¹ı³Ìº¯Êı£¬1¸ö´«Êä¹ı³Ì
        if (ret == 1)break;
        retries++;
    }
    if((retries >= 5))
    {

        GTP_ERROR("I2C Write: 0x%04X, %d bytes failed, errcode: %d! Process reset.", (((uint16_t)(buf[0] << 8)) | buf[1]), len-2, ret);

    }
    return ret;
}



/**
  * @brief   Ê¹ÓÃIIC¶ÁÈ¡ÔÙ´ÎÊı¾İ£¬¼ìÑéÊÇ·ñÕı³£
  * @param
  *		@arg client:Éè±¸µØÖ·
  *		@arg  addr: ¼Ä´æÆ÷µØÖ·
  *		@arg rxbuf: ´æ´¢¶Á³öµÄÊı¾İ
  *		@arg len:    ¶ÁÈ¡µÄ×Ö½ÚÊı
  * @retval
  * 	@arg FAIL
  * 	@arg SUCCESS
  */
 int32_t GTP_I2C_Read_dbl_check(uint8_t client_addr, uint16_t addr, uint8_t *rxbuf, int len)
{
    uint8_t buf[16] = {0};
    uint8_t confirm_buf[16] = {0};
    uint8_t retry = 0;
    
    GTP_DEBUG_FUNC();

    while (retry++ < 3)
    {
        memset(buf, 0xAA, 16);
        buf[0] = (uint8_t)(addr >> 8);
        buf[1] = (uint8_t)(addr & 0xFF);
        GTP_I2C_Read(client_addr, buf, len + 2);
        
        memset(confirm_buf, 0xAB, 16);
        confirm_buf[0] = (uint8_t)(addr >> 8);
        confirm_buf[1] = (uint8_t)(addr & 0xFF);
        GTP_I2C_Read(client_addr, confirm_buf, len + 2);

      
        if (!memcmp(buf, confirm_buf, len+2))
        {
            memcpy(rxbuf, confirm_buf+2, len);
            return SUCCESS;
        }
    }    
    GTP_ERROR("I2C read 0x%04X, %d bytes, double check failed!", addr, len);
    return FAIL;
}


/**
  * @brief   ¹Ø±ÕGT91xxÖĞ¶Ï
  * @param ÎŞ
  * @retval ÎŞ
  */
void GTP_IRQ_Disable(void)
{

    GTP_DEBUG_FUNC();

    I2C_GTP_IRQDisable();
}

/**
  * @brief   Ê¹ÄÜGT91xxÖĞ¶Ï
  * @param ÎŞ
  * @retval ÎŞ
  */
void GTP_IRQ_Enable(void)
{
    GTP_DEBUG_FUNC();
     
	  I2C_GTP_IRQEnable();    
}


/**
  * @brief   ÓÃÓÚ´¦Àí»ò±¨¸æ´¥ÆÁ¼ì²âµ½°´ÏÂ
  * @param
  *    @arg     id: ´¥ÃşË³ĞòtrackID
  *    @arg     x:  ´¥ÃşµÄ x ×ø±ê
  *    @arg     y:  ´¥ÃşµÄ y ×ø±ê
  *    @arg     w:  ´¥ÃşµÄ ´óĞ¡
  * @retval ÎŞ
  */
/*ÓÃÓÚ¼ÇÂ¼Á¬Ğø´¥ÃşÊ±(³¤°´)µÄÉÏÒ»´Î´¥ÃşÎ»ÖÃ£¬¸ºÊıÖµ±íÊ¾ÉÏÒ»´ÎÎŞ´¥Ãş°´ÏÂ*/
static int16_t pre_x[GTP_MAX_TOUCH] ={-1};
static int16_t pre_y[GTP_MAX_TOUCH] ={-1};


/**
	* @brief  åœ¨æŒ‰é”®æŒ‰ä¸‹æ—¶çš„ä¸­æ–­æœåŠ¡ä¸­è¢«è°ƒç”¨
  * @param 	void
  * @retval void
  */
static void Goodix_TS_Work_Func(void)
{
    uint8_t  end_cmd[3] = {GTP_READ_COOR_ADDR >> 8, GTP_READ_COOR_ADDR & 0xFF, 0};
    uint8_t  point_data[2 + 1 + 8 * GTP_MAX_TOUCH + 1]={GTP_READ_COOR_ADDR >> 8, GTP_READ_COOR_ADDR & 0xFF};
    uint8_t  touch_num = 0;
    uint8_t  finger = 0;
    static uint16_t pre_touch = 0;
    static uint8_t pre_id[GTP_MAX_TOUCH] = {0};

    uint8_t client_addr=GTP_ADDRESS;
    uint8_t* coor_data = NULL;
    int32_t input_x = 0;
    int32_t input_y = 0;
    int32_t input_w = 0;
    uint8_t id = 0;
 
    int32_t i  = 0;
    int32_t ret = -1;

    GTP_DEBUG_FUNC();

    ret = GTP_I2C_Read(client_addr, point_data, 12);//10å­—èŠ‚å¯„å­˜å™¨åŠ 2å­—èŠ‚åœ°å€
    if (ret < 0)
    {
        GTP_ERROR("I2C transfer error. errno:%d\n ", ret);

        return;
    }
    
    finger = point_data[GTP_ADDR_LENGTH];//çŠ¶æ€å¯„å­˜å™¨æ•°æ®

    if (finger == 0x00)		//æ²¡æœ‰æ•°æ®ï¼Œé€€å‡º
    {
        return;
    }

    if((finger & 0x80) == 0)//åˆ¤æ–­buffer statusä½
    {
        goto exit_work_func;//åæ ‡æœªå°±ç»ªï¼Œæ•°æ®æ— æ•ˆ
    }

    touch_num = finger & 0x0f;//åæ ‡ç‚¹æ•°
    if (touch_num > GTP_MAX_TOUCH)
    {
        goto exit_work_func;//å¤§äºæœ€å¤§æ”¯æŒç‚¹æ•°ï¼Œé”™è¯¯é€€å‡º
    }

    if (touch_num)
    {
        for (i = 0; i < touch_num; i++)						//ä¸€ä¸ªç‚¹ä¸€ä¸ªç‚¹å¤„ç†
        {
            coor_data = &point_data[i * 8 + 3];

            id = coor_data[0] & 0x0F;									//track id
            pre_id[i] = id;

            input_x  = coor_data[1] | (coor_data[2] << 8);	//xåæ ‡
            input_y  = coor_data[3] | (coor_data[4] << 8);	//yåæ ‡
            input_w  = coor_data[5] | (coor_data[6] << 8);	//size
        
            {
#if 0
										/*æ ¹æ®æ‰«ææ¨¡å¼æ›´æ­£X/Yèµ·å§‹æ–¹å‘*/
								switch(LCD_SCAN_MODE)
								{
									case 0:case 7:
										input_y  = LCD_Y_LENGTH - input_y;
										break;
									
									case 2:case 3: 
										input_x  = LCD_X_LENGTH - input_x;
										input_y  = LCD_Y_LENGTH - input_y;
										break;
									
									case 1:case 6:
										input_x  = LCD_X_LENGTH - input_x;
										break;	
									
									default:
									break;
								}
#endif
            }
        }
				touchpad_x = input_y;
				touchpad_y = NT35510_MORE_PIXEL - input_x;
				touchpad_is_down = 1;
    }
    else if (pre_touch)		//touch_ num=0 ä¸”pre_touchï¼=0
    {
			touchpad_is_down = 0;
    }

    pre_touch = touch_num;


exit_work_func:
    {
        ret = GTP_I2C_Write(client_addr, end_cmd, 3);
        if (ret < 0)
        {
            GTP_INFO("I2C write end_cmd error!");
        }
    }

}


/**
  * @brief   ¸ø´¥ÆÁĞ¾Æ¬ÖØĞÂ¸´Î»
  * @param ÎŞ
  * @retval ÎŞ
  */
 int8_t GTP_Reset_Guitar(void)
{
    GTP_DEBUG_FUNC();
#if 1
    I2C_ResetChip();
    return 0;
#else 		//Èí¼ş¸´Î»
    int8_t ret = -1;
    int8_t retry = 0;
    uint8_t reset_command[3]={(uint8_t)GTP_REG_COMMAND>>8,(uint8_t)GTP_REG_COMMAND&0xFF,2};

    //Ğ´Èë¸´Î»ÃüÁî
    while(retry++ < 5)
    {
        ret = GTP_I2C_Write(GTP_ADDRESS, reset_command, 3);
        if (ret > 0)
        {
            GTP_INFO("GTP enter sleep!");

            return ret;
        }

    }
    GTP_ERROR("GTP send sleep cmd failed.");
    return ret;
#endif

}



 /**
   * @brief   ½øÈëË¯ÃßÄ£Ê½
   * @param ÎŞ
   * @retval 1Îª³É¹¦£¬ÆäËüÎªÊ§°Ü
   */
//int8_t GTP_Enter_Sleep(void)
//{
//    int8_t ret = -1;
//    int8_t retry = 0;
//    uint8_t reset_comment[3] = {(uint8_t)(GTP_REG_COMMENT >> 8), (uint8_t)GTP_REG_COMMENT&0xFF, 5};//5
//
//    GTP_DEBUG_FUNC();
//
//    while(retry++ < 5)
//    {
//        ret = GTP_I2C_Write(GTP_ADDRESS, reset_comment, 3);
//        if (ret > 0)
//        {
//            GTP_INFO("GTP enter sleep!");
//
//            return ret;
//        }
//
//    }
//    GTP_ERROR("GTP send sleep cmd failed.");
//    return ret;
//}


int8_t GTP_Send_Command(uint8_t command)
{
    int8_t ret = -1;
    int8_t retry = 0;
    uint8_t command_buf[3] = {(uint8_t)(GTP_REG_COMMAND >> 8), (uint8_t)GTP_REG_COMMAND&0xFF, GTP_COMMAND_READSTATUS};

    GTP_DEBUG_FUNC();

    while(retry++ < 5)
    {
        ret = GTP_I2C_Write(GTP_ADDRESS, command_buf, 3);
        if (ret > 0)
        {
            GTP_INFO("send command success!");

            return ret;
        }

    }
    GTP_ERROR("send command fail!");
    return ret;
}

/**
  * @brief   »½ĞÑ´¥ÃşÆÁ
  * @param ÎŞ
  * @retval 0Îª³É¹¦£¬ÆäËüÎªÊ§°Ü
  */
int8_t GTP_WakeUp_Sleep(void)
{
    uint8_t retry = 0;
    int8_t ret = -1;

    GTP_DEBUG_FUNC();

    while(retry++ < 10)
    {
        ret = GTP_I2C_Test();
        if (ret > 0)
        {
            GTP_INFO("GTP wakeup sleep.");
            return ret;
        }
        GTP_Reset_Guitar();
    }

    GTP_ERROR("GTP wakeup sleep failed.");
    return ret;
}

static int32_t GTP_Get_Info(void)
{
    uint8_t opr_buf[10] = {0};
    int32_t ret = 0;

    uint16_t abs_x_max = GTP_MAX_WIDTH;
    uint16_t abs_y_max = GTP_MAX_HEIGHT;
    uint8_t int_trigger_type = GTP_INT_TRIGGER;
        
    opr_buf[0] = (uint8_t)((GTP_REG_CONFIG_DATA+1) >> 8);
    opr_buf[1] = (uint8_t)((GTP_REG_CONFIG_DATA+1) & 0xFF);
    
    ret = GTP_I2C_Read(GTP_ADDRESS, opr_buf, 10);
    if (ret < 0)
    {
        return FAIL;
    }
    
    abs_x_max = (opr_buf[3] << 8) + opr_buf[2];
    abs_y_max = (opr_buf[5] << 8) + opr_buf[4];
		GTP_DEBUG("RES");   
		GTP_DEBUG_ARRAY(&opr_buf[0],10);

    opr_buf[0] = (uint8_t)((GTP_REG_CONFIG_DATA+6) >> 8);
    opr_buf[1] = (uint8_t)((GTP_REG_CONFIG_DATA+6) & 0xFF);
    ret = GTP_I2C_Read(GTP_ADDRESS, opr_buf, 3);
    if (ret < 0)
    {
        return FAIL;
    }
    int_trigger_type = opr_buf[2] & 0x03;
    
    GTP_INFO("X_MAX = %d, Y_MAX = %d, TRIGGER = 0x%02x",
            abs_x_max,abs_y_max,int_trigger_type);
    
    return SUCCESS;    
}

/*******************************************************
Function:
    Initialize gtp.
Input:
    ts: goodix private data
Output:
    Executive outcomes.
        0: succeed, otherwise: failed
*******************************************************/
 int32_t GTP_Init_Panel(void)
{
    int32_t ret = -1;

    int32_t i = 0;
    uint16_t check_sum = 0;
    int32_t retry = 0;

    const uint8_t* cfg_info;
    uint8_t cfg_info_len  ;
		uint8_t* config;

    uint8_t cfg_num =0 ;		//ĞèÒªÅäÖÃµÄ¼Ä´æÆ÷¸öÊı

    GTP_DEBUG_FUNC();
	
//uint8_t config[GTP_CONFIG_MAX_LENGTH + GTP_ADDR_LENGTH]
//                = {GTP_REG_CONFIG_DATA >> 8, GTP_REG_CONFIG_DATA & 0xff};
	
    I2C_Touch_Init();

    ret = GTP_I2C_Test();
    if (ret < 0)
    {
        GTP_ERROR("I2C communication ERROR!");
				return ret;
    } 
		
		//»ñÈ¡´¥ÃşICµÄĞÍºÅ
    GTP_Read_Version(); 
    
#if UPDATE_CONFIG
    
    config = (uint8_t *)malloc (GTP_CONFIG_MAX_LENGTH + GTP_ADDR_LENGTH);

		config[0] = GTP_REG_CONFIG_DATA >> 8;
		config[1] =  GTP_REG_CONFIG_DATA & 0xff;
		
		//¸ù¾İICµÄĞÍºÅÖ¸Ïò²»Í¬µÄÅäÖÃ
    	if(touchIC == GT5688)
		{
			cfg_info =  CTP_CFG_GT5688; //Ö¸Ïò¼Ä´æÆ÷ÅäÖÃ
			cfg_info_len = CFG_GROUP_LEN(CTP_CFG_GT5688);//¼ÆËãÅäÖÃ±íµÄ´óĞ¡
		}
		
    memset(&config[GTP_ADDR_LENGTH], 0, GTP_CONFIG_MAX_LENGTH);
    memcpy(&config[GTP_ADDR_LENGTH], cfg_info, cfg_info_len);
		

		cfg_num = cfg_info_len;
		
		GTP_DEBUG("cfg_info_len = %d ",cfg_info_len);
		GTP_DEBUG("cfg_num = %d ",cfg_num);
		GTP_DEBUG_ARRAY(config,6);
		
		/*¸ù¾İLCDµÄÉ¨Ãè·½ÏòÉèÖÃ·Ö±æÂÊ*/
		config[GTP_ADDR_LENGTH+1] = LCD_X_LENGTH & 0xFF;
		config[GTP_ADDR_LENGTH+2] = LCD_X_LENGTH >> 8;
		config[GTP_ADDR_LENGTH+3] = LCD_Y_LENGTH & 0xFF;
		config[GTP_ADDR_LENGTH+4] = LCD_Y_LENGTH >> 8;
		
		/*¸ù¾İÉ¨ÃèÄ£Ê½ÉèÖÃX2Y½»»»*/
		switch(LCD_SCAN_MODE)
		{
			case 0:case 2:case 4: case 6:
				config[GTP_ADDR_LENGTH+6] &= ~(X2Y_LOC);
				break;
			
			case 1:case 3:case 5: case 7:
				config[GTP_ADDR_LENGTH+6] |= (X2Y_LOC);
				break;		
		}

    //¼ÆËãÒªĞ´Èëchecksum¼Ä´æÆ÷µÄÖµ
    check_sum = 0;
		
		for (i = GTP_ADDR_LENGTH; i < (cfg_num+GTP_ADDR_LENGTH -3); i += 2) 
		{
			check_sum += (config[i] << 8) + config[i + 1];
		}
		
		check_sum = 0 - check_sum;
		GTP_DEBUG("Config checksum: 0x%04X", check_sum);
		//¸üĞÂchecksum
		config[(cfg_num+GTP_ADDR_LENGTH -3)] = (check_sum >> 8) & 0xFF;
		config[(cfg_num+GTP_ADDR_LENGTH -2)] = check_sum & 0xFF;
		config[(cfg_num+GTP_ADDR_LENGTH -1)] = 0x01;
		

    //Ğ´ÈëÅäÖÃĞÅÏ¢
    for (retry = 0; retry < 5; retry++)
    {
        ret = GTP_I2C_Write(GTP_ADDRESS, config , cfg_num + GTP_ADDR_LENGTH+2);
        if (ret > 0)
        {
            break;
        }
    }
    Delay(0xfffff);				//ÑÓ³ÙµÈ´ıĞ¾Æ¬¸üĞÂ
		

		
#if 0	//¶Á³öĞ´ÈëµÄÊı¾İ£¬¼ì²éÊÇ·ñÕı³£Ğ´Èë
    //¼ìÑé¶Á³öµÄÊı¾İÓëĞ´ÈëµÄÊÇ·ñÏàÍ¬
	{
    	    uint16_t i;
    	    uint8_t buf[300];
    	     buf[0] = config[0];
    	     buf[1] =config[1];    //¼Ä´æÆ÷µØÖ·

    	    GTP_DEBUG_FUNC();

    	    ret = GTP_I2C_Read(GTP_ADDRESS, buf, sizeof(buf));
			   
					GTP_DEBUG("read ");

					GTP_DEBUG_ARRAY(buf,cfg_num);
		
			    GTP_DEBUG("write ");

					GTP_DEBUG_ARRAY(config,cfg_num);

					//²»¶Ô±È°æ±¾ºÅ
    	    for(i=1;i<cfg_num+GTP_ADDR_LENGTH-3;i++)
    	    {

    	    	if(config[i] != buf[i])
    	    	{
    	    		GTP_ERROR("Config fail ! i = %d ",i);
							free(config);
    	    		return -1;
    	    	}
    	    }
    	    if(i==cfg_num+GTP_ADDR_LENGTH-3)
	    		GTP_DEBUG("Config success ! i = %d ",i);
	}
#endif
	free(config);
#endif
	 /*Ê¹ÄÜÖĞ¶Ï£¬ÕâÑù²ÅÄÜ¼ì²â´¥ÃşÊı¾İ*/
		I2C_GTP_IRQEnable();
	
    GTP_Get_Info();

    return 0;
}


/*******************************************************
Function:
    Read chip version.
Input:
    client:  i2c device
    version: buffer to keep ic firmware version
Output:
    read operation return.
        2: succeed, otherwise: failed
*******************************************************/
int32_t GTP_Read_Version(void)
{
    int32_t ret = -1;
    uint8_t buf[8] = {GTP_REG_VERSION >> 8, GTP_REG_VERSION & 0xff};    //¼Ä´æÆ÷µØÖ·

    GTP_DEBUG_FUNC();

    ret = GTP_I2C_Read(GTP_ADDRESS, buf, sizeof(buf));
    if (ret < 0)
    {
        GTP_ERROR("GTP read version failed");
        return ret;
    }
    if (buf[2] == '5')
    {
        GTP_INFO("IC1 Version: %c%c%c%c_%02x%02x", buf[2], buf[3], buf[4], buf[5], buf[7], buf[6]);
				
				//GT5688Ğ¾Æ¬
				if(buf[2] == '5' && buf[3] == '6' && buf[4] == '8'&& buf[5] == '8')
					touchIC = GT5688;
    }        
    else if (buf[5] == 0x00)
    {
        GTP_INFO("IC2 Version: %c%c%c_%02x%02x", buf[2], buf[3], buf[4], buf[7], buf[6]);
				
				//GT911Ğ¾Æ¬
				if(buf[2] == '9' && buf[3] == '1' && buf[4] == '1')
					touchIC = GT911;
    }
    else
    {
        GTP_INFO("IC3 Version: %c%c%c%c_%02x%02x", buf[2], buf[3], buf[4], buf[5], buf[7], buf[6]);
				
				//GT9157Ğ¾Æ¬
				if(buf[2] == '9' && buf[3] == '1' && buf[4] == '5' && buf[5] == '7')
					touchIC = GT9157;
		}
    return ret;
}

/*******************************************************
Function:
    I2c test Function.
Input:
    client:i2c client.
Output:
    Executive outcomes.
        2: succeed, otherwise failed.
*******************************************************/
static int8_t GTP_I2C_Test( void)
{
    uint8_t test[3] = {GTP_REG_CONFIG_DATA >> 8, GTP_REG_CONFIG_DATA & 0xff};
    uint8_t retry = 0;
    int8_t ret = -1;

    GTP_DEBUG_FUNC();
  
    while(retry++ < 5)
    {
        ret = GTP_I2C_Read(GTP_ADDRESS, test, 3);
        if (ret > 0)
        {
            return ret;
        }
        GTP_ERROR("GTP i2c test failed time %d.",retry);
    }
    return ret;
}

//¼ì²âµ½´¥ÃşÖĞ¶ÏÊ±µ÷ÓÃ£¬
void GTP_TouchProcess(void)
{
  Goodix_TS_Work_Func();
}






//MODULE_DESCRIPTION("GTP Series Driver");
//MODULE_LICENSE("GPL");
