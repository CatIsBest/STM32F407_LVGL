#ifndef __I2C_TOUCH_H
#define	__I2C_TOUCH_H

#include "stm32f4xx.h"

/*�趨ʹ�õĵ�����IIC�豸��ַ*/
#define GTP_ADDRESS            0xBA

#define I2CT_FLAG_TIMEOUT         ((uint32_t)0x1000)
#define I2CT_LONG_TIMEOUT         ((uint32_t)(10 * I2CT_FLAG_TIMEOUT))

#define GTP_I2C_SCL_CLK()          __GPIOD_CLK_ENABLE()
#define GTP_I2C_SDA_CLK()          __GPIOD_CLK_ENABLE()
#define GTP_I2C_RST_CLK()          __GPIOD_CLK_ENABLE()
#define GTP_I2C_GTP_CLK()          __GPIOG_CLK_ENABLE()

/*I2C����*/
#define GTP_I2C_SCL_PIN                  GPIO_PIN_7                 
#define GTP_I2C_SCL_GPIO_PORT            GPIOD                       

#define GTP_I2C_SDA_PIN                  GPIO_PIN_3                 
#define GTP_I2C_SDA_GPIO_PORT            GPIOD                     

/*��λ����*/
#define GTP_RST_GPIO_PORT                GPIOD
#define GTP_RST_GPIO_PIN                 GPIO_PIN_6
/*�ж�����*/
#define GTP_INT_GPIO_PORT                GPIOG
#define GTP_INT_GPIO_PIN                 GPIO_PIN_8
#define GTP_INT_EXTI_IRQ                 EXTI9_5_IRQn
/*�жϷ�����*/
#define GTP_IRQHandler                   EXTI9_5_IRQHandler

#define	digitalHigh(p,i)			{p->BSRR=i;}			  //����Ϊ�ߵ�ƽ		
#define digitalLow(p,i)			  {p->BSRR=(uint32_t)i << 16;}				//����͵�ƽ

//���IICʹ�õĺ�
/* SCL = 1 */
#define I2C_SCL_1()  digitalHigh(GTP_I2C_SCL_GPIO_PORT, GTP_I2C_SCL_PIN)
/* SCL = 0 */
#define I2C_SCL_0()  digitalLow(GTP_I2C_SCL_GPIO_PORT, GTP_I2C_SCL_PIN)		
/* SDA = 1 */
#define I2C_SDA_1()  digitalHigh(GTP_I2C_SDA_GPIO_PORT, GTP_I2C_SDA_PIN)		
/* SDA = 0 */
#define I2C_SDA_0()  digitalLow(GTP_I2C_SDA_GPIO_PORT, GTP_I2C_SDA_PIN)		
/* ��SDA����״̬ */
#define I2C_SDA_READ()   HAL_GPIO_ReadPin(GTP_I2C_SDA_GPIO_PORT, GTP_I2C_SDA_PIN)	

//�����ӿ�
void I2C_Touch_Init(void);
uint32_t I2C_WriteBytes(uint8_t ClientAddr,uint8_t* pBuffer,  uint8_t NumByteToWrite);
uint32_t I2C_ReadBytes(uint8_t ClientAddr,uint8_t* pBuffer, uint16_t NumByteToRead);
void I2C_ResetChip(void);
void I2C_GTP_IRQDisable(void);
void I2C_GTP_IRQEnable(void);

#endif /* __I2C_TOUCH_H */
