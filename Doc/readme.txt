/*********************************************************************************************/

【*】 程序简介 
-工程名称：lvgl v8.3移植及组件使用
-实验平台: 野火STM32 F407 开发板
-MDK版本：5.32
-ST固件库版本：1.8.0

【 ！】功能简介：
显示一个UI界面，并编写了简单应用

【 ！】实验操作：
连接好配套的4.3寸液晶屏，下载程序后复位开发板即可，


【*】 引脚分配

电容屏（GT5688）：	
电容触摸屏接口与STM32的普通IO口相连，使用中断方式读取触摸点数据。	
本硬件连接不支持硬件IIC。

		RSTN	<--->PD6
		INT		<--->PG8
		SDA		<--->PD3
		SCL		<--->PD7
		NC		<--->空引脚，不需要连接

液晶屏：
液晶屏接口与STM32的FSMC接口相连，支565格式，
使用FSMC产生8080时序与液晶屏驱动芯片ILI9806G通讯.


		/*液晶控制信号线*/
		NT35510_CS 	<--->	PG10      	//片选，选择NOR/SRAM块 BANK1_NOR/SRAM3
		NT35510_DC  	<---> 	PF0			//PF0为FSMC_A0,本引脚决定了访问LCD时使用的地址
		NT35510_WR 	<---> 	PD5			//写使能
		NT35510_RD  	<---> 	PD4			//读使能
		NT35510_RST	<---> 	PF11		//复位引脚
		NT35510_BK 	<---> 	PF9 		//背光引脚，高电平亮
		
	数据信号线省略,本实验没有驱动触摸屏，详看触摸画板实验。
	
/*********************************************************************************************/

【*】 时钟

A.晶振：
-外部高速晶振：25MHz
-RTC晶振：32.768KHz

B.各总线运行时钟：
-系统时钟 = SYCCLK = AHB1 = 168MHz
-APB2 = 84MHz 
-APB1 = 42MHz


-博客地址    :https://blog.csdn.net/weixin_45209978/article/details/127374115
-代码地址    :https://github.com/CatIsBest/STM32F407_LVGL

/*********************************************************************************************/